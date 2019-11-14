randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) randlibsw.c -fPIC -shared -o randlibsw.so
randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) randlibhw.c -fPIC -shared -o randlibhw.so
randmain: randmain.c randcpuid.c
	$(CC) $(CFLAGS) randmain.c randcpuid.c -ldl -Wl,-rpath=$PWD -o randmain