#include <stdio.h>
#include <unistd.h>



char unused2[4096]={1,2,3,6,7};
char unused1[4096]={1,2,3,6,7};


void vuln(int);
void *ptr=vuln;
void execute(void(),int);

void pad1(){
	asm __volatile__(".rept 0x39\n"
			"nop\n"
			".endr\n"
			
			);

}



void execute(void f(),int a) // receive address of print
{
    f(a);
}


void vuln(int a){

	return;
}



int main()
{
    //ComputeThreshold();
    while (1){
    	execute(ptr,1); // sends address of print
    }
    return 0;
}


