#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <wchar.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>


#include "emojiserver.h"
#define PORT 8080

wchar_t *emojis;
unsigned char *limit;

char *flag="CTF-BR{tried_flush+reload}";
char *flag2="CTF-BR{nice_spectre_bro}";

void Vuln(unsigned long x){


	volatile int tmp;
	if(x<*limit){
	  tmp=emojis[(flag[x]-'-')*160];
	}

}


void main_loop(int socket){
	unsigned long index;
	size_t len;
	while (1){
		if (read(socket,&index,sizeof(index))!=sizeof(index)){
		  break;
		}
		Vuln(index);
	}
	

}
int createServer(){
    int server_fd; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    return server_fd;
}
int resetConn(int socket){
    int new_socket;
    struct sockaddr_in address; 
    int addrlen = sizeof(address); 
    if ((new_socket = accept(socket, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    return new_socket; 

}
int main() 
{ 
    int server;
    int client;
    emojis=getEmojis();
    limit=getLimit();
    printf("limit is %i no index above will be utilized\n",*limit);
    server=createServer();
    while (1){
      client=resetConn(server);
      main_loop(client); 
    
    }
    return 0; 
} 
