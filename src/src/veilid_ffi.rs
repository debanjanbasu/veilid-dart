use anyhow::{Ok, Result};
use flutter_rust_bridge::frb;
use log::info;
use once_cell::sync::Lazy;
use std::{path::Path, sync::Arc};
use tokio::sync::Mutex as AsyncMutex;
use veilid_core::{
    api_startup_config, VeilidAPI,
    VeilidAPIError::{AlreadyInitialized, NotInitialized},
    VeilidAPIResult, VeilidConfigBlockStore, VeilidConfigInner, VeilidConfigProtectedStore,
    VeilidConfigTableStore,
    VeilidUpdate::{self, AppMessage, Network},
};

static VEILID_API: Lazy<Arc<AsyncMutex<Option<VeilidAPI>>>> =
    Lazy::new(|| Arc::new(AsyncMutex::new(None)));

fn update_callback() -> Arc<impl Fn(VeilidUpdate) + Send + Sync + 'static> {
    Arc::new(move |update| {
        match update {
            AppMessage(msg) => {
                info!("Message: {}", String::from_utf8_lossy(msg.message()));
            }
            Network(msg) => {
                info!(
                    "Network: Peers {}, bytes/sec [{} up] [{} down]",
                    msg.peers.len(),
                    msg.bps_up,
                    msg.bps_down
                );
            }
            _ => info!("{:?}", update),
        };
    })
}

fn get_directory(app_support_dir: &str, join_path: &str) -> String {
    Path::new(app_support_dir)
        .join(join_path)
        .to_string_lossy()
        .to_string()
}

fn get_veilid_config(app_support_dir: &str) -> VeilidConfigInner {
    VeilidConfigInner {
        program_name: "spareshare".into(),
        namespace: "quanti".into(),
        protected_store: VeilidConfigProtectedStore {
            directory: get_directory(app_support_dir, "protected_store"),
            ..Default::default()
        },
        block_store: VeilidConfigBlockStore {
            directory: get_directory(app_support_dir, "block_store"),
            ..Default::default()
        },
        table_store: VeilidConfigTableStore {
            directory: get_directory(app_support_dir, "table_store"),
            ..Default::default()
        },
        ..Default::default()
    }
}

async fn is_veilid_initialized() -> bool {
    VEILID_API.lock().await.is_some()
}

// Public Veilid Initializer function to be called when flutter is loaded
pub async fn veilid_init(app_support_dir: String) -> Result<()> {
    // Get the api lock from mutex
    if is_veilid_initialized().await {
        // Return AlreadyInitialized error if trying to initialize again
        Err(AlreadyInitialized)?
    }

    // Initializing Veilid Core
    info!("⏳ Initilizing Veilid...");

    // Initialize veilid api
    let veilid = api_startup_config(update_callback(), get_veilid_config(&app_support_dir)).await?;
    info!(
        "🚀 Node Initialized - Node ID: {}",
        veilid
            .config()?
            .get_veilid_state()
            .config
            .network
            .routing_table
            .node_id
    );

    // Waiting for processing - Node is ready to process requests
    veilid.attach().await?;

    // Point the VEILID_API to veilid API - for thread-safe op
    *VEILID_API.lock().await = Some(veilid);

    // Anyhow Ok
    Ok(())
}

// This is the most popular function to get the Veilid API when to use it
#[frb(ignore)]
pub async fn get_veilid_api() -> VeilidAPIResult<VeilidAPI> {
    // If the API is not initialized
    // This is only needed for Android to work because of the way the Flutter engine works - two VMs
    #[cfg(target_os = "android")]
    if !is_veilid_initialized().await {
        // Initialize the API first - return error
        VeilidAPIResult::Err(NotInitialized)?
    }
    VEILID_API
        .lock()
        .await
        .as_ref()
        .cloned()
        .ok_or(NotInitialized)
}

// Taking is risky, and leaves a None inside the Option
// Thus only used before shutdown
async fn take_veilid_api() -> VeilidAPIResult<VeilidAPI> {
    VEILID_API.lock().await.take().ok_or(NotInitialized)
}

// Kill Switch - to shutdown your own node
#[frb(ignore)]
pub async fn veilid_shutdown() -> Result<()> {
    info!("⏳ Shutting down Veilid...");
    // Taking the API, and store None in the main Option
    take_veilid_api().await?.shutdown().await;
    info!("🚀 Veilid Shutdown");
    Ok(())
}

pub async fn get_veilid_node_id() -> Result<String> {
    Ok(get_veilid_api()
        .await?
        .config()?
        .get_veilid_state()
        .config
        .network
        .routing_table
        .node_id
        .to_string())
}
