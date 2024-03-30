#include <stdio.h>
#include <stdlib.h>


    int *foo(int n){
        int *ptr = malloc(n*sizeof(int));
        int *tmp = ptr;
        for(int i=0;i<n;i++){
            *tmp = i;
            tmp++;
        }
        return ptr;
    }

    void bar(int *ptr,int n){
        for(int i=0;i<n;i++){
            if(i%2 ==0){
                *ptr=0;
            }
            ptr++;
        }
    }


    int main(int argc,char **argv){
        int n = 9;
        int *ptr = foo(n);
        bar(ptr,n);
        for(int i=0;i<n;i++){
            printf("%d\n",ptr[i]);
        }
        return 0;
    }



