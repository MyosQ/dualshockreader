#include <stdio.h>

int main(){
   int fd;
   if((fd = open("/dev/input/js0", O_RDONLY | O_NONBLOCK)) < 0){
      perror("Couldnt open file");
      exit(EXIT_FAILURE);
   }


}
