all:
	#gcc -c -Wall emojiserver.c -fpic -s
	#gcc -shared -o libemojiserver.so emojiserver.o 
	#strip --strip-all --discard-all libemojiserver.so
	#sudo cp libemojiserver.so /lib/x86_64-linux-gnu/
	gcc -o server server.c -no-pie -lemojiserver
	gcc -o attacker attacker.c -no-pie -lemojiserver -O0
	gcc -o polute polute.c -no-pie 
	gcc -o flusher flusher.c -no-pie -O0
	#rm *.o
