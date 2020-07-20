#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#define VAR_OFFSET		0x0b0 

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

void flush(char **list){
  volatile unsigned long tmp;
  for (int i=0;i<8;i++){
  	tmp=*(list[i]+VAR_OFFSET);
  }

}
//0x00000000004040b0 - > ptr, use avoid to flush it
int main()
{
    char *pages[8];
    pages[0]=(char *)createNullPage((void *)0x4004000,0x1000);
    pages[1]=(char *)createNullPage((void *)0x4005000,0x1000);
    pages[2]=(char *)createNullPage((void *)0x4006000,0x1000);
    pages[3]=(char *)createNullPage((void *)0x4007000,0x1000);
    pages[4]=(char *)createNullPage((void *)0x4008000,0x1000);
    pages[5]=(char *)createNullPage((void *)0x4009000,0x1000);
    pages[6]=(char *)createNullPage((void *)0x400a000,0x1000);
    pages[7]=(char *)createNullPage((void *)0x400b000,0x1000);
    //ComputeThreshold();
    while (1){
	flush(pages);
    }
    return 0;
}


