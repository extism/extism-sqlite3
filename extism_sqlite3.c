#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extism.h"

#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT1;

/*
int sqlite3_create_function_v2(
  sqlite3 *db,
  const char *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
  void (*xStep)(sqlite3_context*,int,sqlite3_value**),
  void (*xFinal)(sqlite3_context*),
  void(*xDestroy)(void*)
);*/

uint8_t *read_file(const char *filename, size_t *len) {

  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  size_t length = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  uint8_t *data = malloc(length);
  if (data == NULL) {
    return NULL;
  }

  fread(data, 1, length, fp);
  fclose(fp);

  *len = length;
  return data;
}

static void ext_load(sqlite3_context *ctx, int nargs, sqlite3_value **args) {
  if (nargs < 1) {
    sqlite3_result_error(ctx, "Expected 1 argument, got 0", -1);
    return;
  }

  ExtismContext *context = sqlite3_user_data(ctx);
  const char *file = (const char *)sqlite3_value_text(args[0]);
  size_t len = 0;
  uint8_t *wasm = read_file(file, &len);
  if (wasm == NULL) {
    sqlite3_result_error(ctx, "Plugin file not found", -1);
    return;
  }

  ExtismPlugin plugin = extism_plugin_new(context, wasm, len, false);
  free(wasm);
  if (plugin < 0) {
    sqlite3_result_error(ctx, "Unable to load plugin", -1);
    return;
  }

  sqlite3_result_int(ctx, plugin);
}

static void ext_call(sqlite3_context *ctx, int nargs, sqlite3_value **args) {
  if (nargs < 3) {
    sqlite3_result_error(ctx, "Expected 3 arguments", -1);
    return;
  }

  ExtismContext *context = sqlite3_user_data(ctx);

  ExtismPlugin plugin = sqlite3_value_int(args[0]);
  const char *name = (const char *)sqlite3_value_text(args[1]);
  const uint8_t *input = sqlite3_value_text(args[2]);
  int32_t rc = extism_plugin_call(context, plugin, name, input,
                                  strlen((const char *)input));
  if (rc != 0) {
    const char *err = extism_error(context, plugin);
    if (err != NULL) {
      sqlite3_result_error(ctx, err, -1);
      return;
    }

    sqlite3_result_error(ctx, "extism_call failed", -1);
    return;
  }

  ExtismSize out_len = extism_plugin_output_length(context, plugin);
  const uint8_t *out = extism_plugin_output_data(context, plugin);
  sqlite3_result_text(ctx, (const char *)out, out_len, SQLITE_TRANSIENT);
}

int sqlite3_extension_init(sqlite3 *db, char **pzErrMsg,
                           const sqlite3_api_routines *pApi) {
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
  ExtismContext *context = extism_context_new();
  sqlite3_create_function_v2(db, "extism_load", 1, SQLITE_UTF8, context,
                             ext_load, NULL, NULL, (void *)extism_context_free);
  sqlite3_create_function_v2(db, "extism_call", 3, SQLITE_UTF8, context,
                             ext_call, NULL, NULL, NULL);
  return rc;
}
