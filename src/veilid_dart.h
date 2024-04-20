#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

struct VeilidVersion {
  uint32_t major;
  uint32_t minor;
  uint32_t patch;
};

extern "C" {

void initialize_veilid_flutter(DartPostCObjectFnType dart_post_c_object_ptr, FfiStr crash_path);

/// C-compatible FFI Functions
void initialize_veilid_core(FfiStr platform_config);

void change_log_level(FfiStr layer, FfiStr log_level);

void change_log_ignore(FfiStr layer, FfiStr log_ignore);

void startup_veilid_core(int64_t port, int64_t stream_port, FfiStr config);

void get_veilid_state(int64_t port);

void attach(int64_t port);

void detach(int64_t port);

void shutdown_veilid_core(int64_t port);

void routing_context(int64_t port);

int32_t release_routing_context(uint32_t id);

uint32_t routing_context_with_default_safety(uint32_t id);

uint32_t routing_context_with_safety(uint32_t id, FfiStr safety_selection);

uint32_t routing_context_with_sequencing(uint32_t id, FfiStr sequencing);

void routing_context_safety(int64_t port, uint32_t id);

void routing_context_app_call(int64_t port, uint32_t id, FfiStr target, FfiStr request);

void routing_context_app_message(int64_t port, uint32_t id, FfiStr target, FfiStr message);

void routing_context_create_dht_record(int64_t port, uint32_t id, FfiStr schema, uint32_t kind);

void routing_context_open_dht_record(int64_t port, uint32_t id, FfiStr key, FfiStr writer);

void routing_context_close_dht_record(int64_t port, uint32_t id, FfiStr key);

void routing_context_delete_dht_record(int64_t port, uint32_t id, FfiStr key);

void routing_context_get_dht_value(int64_t port,
                                   uint32_t id,
                                   FfiStr key,
                                   uint32_t subkey,
                                   bool force_refresh);

void routing_context_set_dht_value(int64_t port,
                                   uint32_t id,
                                   FfiStr key,
                                   uint32_t subkey,
                                   FfiStr data,
                                   FfiStr writer);

void routing_context_watch_dht_values(int64_t port,
                                      uint32_t id,
                                      FfiStr key,
                                      FfiStr subkeys,
                                      uint64_t expiration,
                                      uint32_t count);

void routing_context_cancel_dht_watch(int64_t port, uint32_t id, FfiStr key, FfiStr subkeys);

void routing_context_inspect_dht_record(int64_t port,
                                        uint32_t id,
                                        FfiStr key,
                                        FfiStr subkeys,
                                        FfiStr scope);

void new_private_route(int64_t port);

void new_custom_private_route(int64_t port, FfiStr stability, FfiStr sequencing);

void import_remote_private_route(int64_t port, FfiStr blob);

void release_private_route(int64_t port, FfiStr route_id);

void app_call_reply(int64_t port, FfiStr call_id, FfiStr message);

void open_table_db(int64_t port, FfiStr name, uint32_t column_count);

int32_t release_table_db(uint32_t id);

void delete_table_db(int64_t port, FfiStr name);

uint32_t table_db_get_column_count(uint32_t id);

void table_db_get_keys(int64_t port, uint32_t id, uint32_t col);

uint32_t table_db_transact(uint32_t id);

int32_t release_table_db_transaction(uint32_t id);

void table_db_transaction_commit(int64_t port, uint32_t id);

void table_db_transaction_rollback(int64_t port, uint32_t id);

void table_db_transaction_store(int64_t port, uint32_t id, uint32_t col, FfiStr key, FfiStr value);

void table_db_transaction_delete(int64_t port, uint32_t id, uint32_t col, FfiStr key);

void table_db_store(int64_t port, uint32_t id, uint32_t col, FfiStr key, FfiStr value);

void table_db_load(int64_t port, uint32_t id, uint32_t col, FfiStr key);

void table_db_delete(int64_t port, uint32_t id, uint32_t col, FfiStr key);

char *valid_crypto_kinds();

uint32_t best_crypto_kind();

void verify_signatures(int64_t port, FfiStr node_ids, FfiStr data, FfiStr signatures);

void generate_signatures(int64_t port, FfiStr data, FfiStr key_pairs);

void generate_key_pair(int64_t port, uint32_t kind);

void crypto_cached_dh(int64_t port, uint32_t kind, FfiStr key, FfiStr secret);

void crypto_compute_dh(int64_t port, uint32_t kind, FfiStr key, FfiStr secret);

void crypto_generate_shared_secret(int64_t port,
                                   uint32_t kind,
                                   FfiStr key,
                                   FfiStr secret,
                                   FfiStr domain);

void crypto_random_bytes(int64_t port, uint32_t kind, uint32_t len);

void crypto_default_salt_length(int64_t port, uint32_t kind);

void crypto_hash_password(int64_t port, uint32_t kind, FfiStr password, FfiStr salt);

void crypto_verify_password(int64_t port, uint32_t kind, FfiStr password, FfiStr password_hash);

void crypto_derive_shared_secret(int64_t port, uint32_t kind, FfiStr password, FfiStr salt);

void crypto_random_nonce(int64_t port, uint32_t kind);

void crypto_random_shared_secret(int64_t port, uint32_t kind);

void crypto_generate_key_pair(int64_t port, uint32_t kind);

void crypto_generate_hash(int64_t port, uint32_t kind, FfiStr data);

void crypto_validate_key_pair(int64_t port, uint32_t kind, FfiStr key, FfiStr secret);

void crypto_validate_hash(int64_t port, uint32_t kind, FfiStr data, FfiStr hash);

void crypto_distance(int64_t port, uint32_t kind, FfiStr key1, FfiStr key2);

void crypto_sign(int64_t port, uint32_t kind, FfiStr key, FfiStr secret, FfiStr data);

void crypto_verify(int64_t port, uint32_t kind, FfiStr key, FfiStr data, FfiStr signature);

void crypto_aead_overhead(int64_t port, uint32_t kind);

void crypto_decrypt_aead(int64_t port,
                         uint32_t kind,
                         FfiStr body,
                         FfiStr nonce,
                         FfiStr shared_secret,
                         FfiStr associated_data);

void crypto_encrypt_aead(int64_t port,
                         uint32_t kind,
                         FfiStr body,
                         FfiStr nonce,
                         FfiStr shared_secret,
                         FfiStr associated_data);

void crypto_crypt_no_auth(int64_t port,
                          uint32_t kind,
                          FfiStr body,
                          FfiStr nonce,
                          FfiStr shared_secret);

uint64_t now();

void debug(int64_t port, FfiStr command);

char *veilid_version_string();

VeilidVersion veilid_version();

char *default_veilid_config();

} // extern "C"
