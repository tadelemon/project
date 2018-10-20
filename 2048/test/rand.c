#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

void printRand(){
    srand(time(0));
    for(int i=0;i<4;i++){
        printf("%d\n",rand());
    }
}
void rr(){

    for(int i=0;i<4;i++){
        printf("%d\n",rand());
    }
}
int main(int argc, char const *argv[])
{   

    printRand();

    printf("\n\n");
    // rr();
    
    while(1){
        char cha = getchar();
        if(cha == 'a' || cha == '\n'){
            rr();
            printf("\n");
        }else{
            break;
        }
    }

    // printf("%ld",time(0));
    return 0;
}
