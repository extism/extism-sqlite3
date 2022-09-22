-- This example shows how to integrate an Extism plugin used to make an HTTP request with SQLite3. 
-- It uses an Extism WebAssembly plugin to fetch a JSON object then converts that JSON object into 
-- an SQLite table.

-- Load extism_sqlite extension
SELECT load_extension('./libextism_sqlite3');

-- Create temporary table for storing intermediate values
CREATE TEMP TABLE IF NOT EXISTS tmp (key TEXT PRIMARY KEY, value TEXT);

-- Load plugin from disk
INSERT INTO tmp VALUES ('plugin', (SELECT extism_load('./plugins/http.wasm')));

-- URL to download JSON data from
INSERT INTO tmp VALUES ('url', 'https://api.github.com/emojis');

-- Fetch JSON data
INSERT INTO tmp VALUES ('data', (SELECT extism_call((SELECT value FROM tmp WHERE key = 'plugin'), 'http_get', (SELECT value from tmp WHERE key = 'url'))));

-- Create table to store information from JSON dataset
CREATE TABLE IF NOT EXISTS github_emojis (name TEXT PRIMARY KEY, url TEXT NOT NULL);

-- Convert our JSON payload into a table
INSERT INTO github_emojis (name, url) SELECT key, value FROM json_each((SELECT value FROM tmp where key = 'data'));

-- Show emojis where the name contains the word 'smile'
SELECT name from github_emojis WHERE name LIKE '%smile%';
