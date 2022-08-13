lib:
	$(CC) -shared -fPIC -o libextism_sqlite.so extism_sqlite.c -lextism -L.

clean:
	rm -f ./libextism_sqlite.so