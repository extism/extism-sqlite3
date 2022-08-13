#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef int32_t ExtismPlugin;

typedef uint32_t ExtismSize;

ExtismPlugin extism_plugin_register(const uint8_t *wasm, ExtismSize wasm_size);

int32_t extism_call(ExtismPlugin plugin,
                    const char *func_name,
                    const uint8_t *data,
                    ExtismSize data_len);

const char *extism_error(ExtismPlugin plugin);

ExtismSize extism_output_length(ExtismPlugin plugin);

void extism_output_get(ExtismPlugin plugin, uint8_t *buf, ExtismSize len);
