#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "emojis.h"


char unused1[4096]={1,23,66};
unsigned char emojiLimit=26;
char unused2[4096]={77,11,22,44};


int printTranslate(char text,unsigned granularity){

	wchar_t wordp;	
	unsigned translated=(unsigned)(text-'-');
	if (text<'-' || text > '}'){
	  return -1;
	}
	if (  translated*granularity>LEN_LIB   ){
	  return -2;
	}
		
	
	wordp=emojiLib[translated * granularity];
	setlocale(LC_ALL, "en_US.utf8");
	printf("%lc",wordp);
	return 0;
}
wchar_t *getEmojis(){
	return emojiLib;
}
unsigned char *getLimit(){
	return &emojiLimit;
}
