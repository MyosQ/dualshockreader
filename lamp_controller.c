#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <linux/joystick.h>
#define TRY_OPEN_MAX_TIME 100
#define TRY_OPEN_SLEEP_TIME 5
#define MAX_LOOPS TRY_OPEN_MAX_TIME/TRY_OPEN_SLEEP_TIME

int tryOpen(){
   int fd, a;
   for(a = 0; a < MAX_LOOPS; a++){
      if((fd = open("/dev/input/js0", O_RDONLY)) > 0)
         return fd;
      sleep(TRY_OPEN_SLEEP_TIME);
   }

   exit(EXIT_FAILURE);
}

int main(){
	int fd, buttonState = 0, prevButtonState = 0;
	struct js_event ev;

	if((fd = open("/dev/input/js0", O_RDONLY)) < 0){
		perror("Couldnt open file");
	exit(EXIT_FAILURE);
	}

	while(1){
		//read(fd, &ev, sizeof(ev));
      if(read(fd, &ev, sizeof(ev)) < 0){

         if(errno == EBADF)/*File doesnt exist */
            fd = tryOpen();
      }

		if((ev.type & ~JS_EVENT_INIT) == JS_EVENT_BUTTON){
			if(ev.value)
				buttonState |= (1 << ev.number);
			else
				buttonState &= ~(1 << ev.number);
		}

		if(ev.value && (buttonState == 2) && (prevButtonState == 0)){
			printf("EX\n");
			system("gpio -g mode 24 out");
			system("gpio -g write 24 1");
		}
		if(ev.value && (buttonState == 4) && (prevButtonState == 0)){
			printf("RING\n");
			system("gpio -g mode 24 out");
			system("gpio -g write 24 0");
		}
		prevButtonState = buttonState;

	}
}
