all:
	#gcc -c -Wall emojinet.c -fpic -s
	#gcc -shared -o libemojinet.so emojinet.o -s
	#strip --strip-all --discard-all libemojinet.so
	#sudo cp libemojinet.so /lib/x86_64-linux-gnu/
	gcc -Wall -o victim victim.c -lemojinet -s
	gcc -Wall -o attacker attacker.c -lemojinet -s
	#rm *.o
