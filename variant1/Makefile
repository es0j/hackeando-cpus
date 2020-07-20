all:
	#gcc -c -Wall emojiserver.c -fpic -s
	#gcc -shared -o libemojiserver.so emojiserver.o 
	#strip --strip-all --discard-all libemojiserver.so
	#sudo cp libemojiserver.so /lib/x86_64-linux-gnu/
	gcc -o attacker attacker.c -no-pie -lemojiserver -O0
	gcc -o server server.c -no-pie -lemojiserver
	#rm *.o
