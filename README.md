# extism-sqlite

A `sqlite3` extension for loading [Extism](https://github.com/extism/extism) plugins

## Building

`libextism.so` (or `libextism.dylib`) should be installed then:

```sh
$ make
```

## Usage

Load the extism_sqlite3 plugin:

```sh
sqlite> .load ./libextism_sqlite3
```

Load an extism plugin:

```sh
sqlite> SELECT extism_load("plugins/http.wasm");
0
```

The return value of `extism_load` is the plugin handle that should be passed to `extism_call`:

```sh
sqlite> SELECT extism_call(0, "http_get", "http://example.com");
...
```
