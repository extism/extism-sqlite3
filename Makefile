lib:
	$(CC) -shared -fPIC -o libextism_sqlite3.so extism_sqlite3.c -lextism -L.

clean:
	rm -f ./libextism_sqlite3.so