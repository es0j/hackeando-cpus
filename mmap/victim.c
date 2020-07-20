#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "emojis.h"


int main(){
	char flag[200];
	volatile wchar_t tmp;
	int i;
  	FILE *f;
  	
	
	f=fopen("flag.txt","r");
  	if (f!=NULL){
    	  fgets(flag,100,f);
  	}
  	else{
    		printf("cant open file\n");
  	}
	fclose(f);

	while (1){
		for (i=0;flag[i]!='\n';i++){
		  tmp=emojiLib[(flag[i]-'-')*160];
		  usleep(100);
		}
	
	}

}

