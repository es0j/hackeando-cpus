#include <stdio.h>
#include <unistd.h>
#include <wchar.h>

#include "emojiserver.h"

#define 	SECRET			"CTF-BR{NOT_CALLED}"
#define 	SECRET_LEN		18
#define 	REPEAT			100
void print(int);
void execute(void(),int);
void *ptr=print;



unsigned char unused[80]={64, 32, 59, 72, 34, 7, 43};

wchar_t *myEmojiLib;

void execute(void f(),int a) // receive address of print
{
    f(a);
}

void vuln(int index){

	char flag[]=SECRET;
	volatile int tmp;
	tmp=myEmojiLib[(flag[index]-'-')*160];

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
void print(int a)
{
    //printf("Hello!");
    volatile int tmp;
    tmp=a*a;
}

void flushAll(wchar_t *emoji){
	/*if no emoji is accessed in your program something goes wrong : ( , maybe linker doesent link  emojilist into your program? ?*/
	printf("%i",myEmojiLib[160]);
}



int main()
{
    //ComputeThreshold();
    int a=0;
    int c=0;
    myEmojiLib=getEmojis();
    flushAll(myEmojiLib);
    while (1){
	flush((char *)&ptr);
    	execute(ptr,(a%SECRET_LEN)); 
	usleep(200);
	if(c%REPEAT==0){
		a++;
	}
	c++;
    }
    return 0;
}


