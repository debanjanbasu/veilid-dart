import 'dart:developer';

import 'package:native_toolchain_c/native_toolchain_c.dart';
import 'package:logging/logging.dart';
import 'package:native_assets_cli/native_assets_cli.dart';

const packageName = 'veilid_dart';

void main(List<String> args) async {
  await build(args, (config, output) async {
    final packageName = config.packageName;
    final cbuilder = CBuilder.library(
      name: packageName,
      assetName: '${packageName}_bindings_generated.dart',
      sources: [
        'src/$packageName.c',
      ],
      dartBuildFiles: ['hook/build.dart'],
    );
    await cbuilder.run(
      buildConfig: config,
      buildOutput: output,
      logger: Logger('')
        ..level = Level.ALL
        ..onRecord.listen((record) => log(record.message)),
    );
  });
}
