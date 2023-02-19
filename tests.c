
#include <stdio.h>

#include "resource_cacher.h"

int main(void){
    printf("test1 start\n");
    if(rc_test()){
        return 1;
    }
    printf("test1 end\n");
    return 0;
}
