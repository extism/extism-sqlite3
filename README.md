# extism-sqlite

A `sqlite3` extension for loading extism plugins

## Building

`libextism.so` (or `libextism.dylib`) should be installed already or copied into the root of the project, then:

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
sqlite> SELECT extism_load("code.wasm");
0
```

The return value of `extism_load` is the plugin handle that should be passed to `extism_call`:

```sh
sqlite> SELECT json_extract(extism_call(0, "count_vowels", "abcdefg"), "$.count");
2
```
