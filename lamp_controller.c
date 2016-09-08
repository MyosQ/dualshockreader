#include "dsreader.h"

#define TRY_OPEN_MAX_TIME 100
#define TRY_OPEN_SLEEP_TIME 5
#define TRY_OPEN_MAX_LOOPS TRY_OPEN_MAX_TIME/TRY_OPEN_SLEEP_TIME
#define DEVICE_PATH "/dev/input/js0"

int tryOpen(){
   int fd, a;
   for(a = 0; a < TRY_OPEN_MAX_LOOPS; a++){
      if((fd = open(DEVICE_PATH, O_RDONLY)) > 0)
         return fd;
      sleep(TRY_OPEN_SLEEP_TIME);
   }
   exit(EXIT_FAILURE);
}

int main(){
	int fd, buttonState = 0, prevButtonState = 0;
	struct js_event ev;

	/* Try to open the input file */
	fd = tryOpen();

	while(1){
		/* Read into event struct */
		if(read(fd, &ev, sizeof(ev)) < 0)
			fd = tryOpen();

		/* Update buttonState */
		if((ev.type & ~JS_EVENT_INIT) == JS_EVENT_BUTTON){
			if(ev.value)
				buttonState |= (1 << ev.number);
			else
				buttonState &= ~(1 << ev.number);
		}

		/* Check if EX or RING is pressed */
		if(ev.value && (buttonState == 1 << DS_CROSS) && (prevButtonState == 0)){
			system("gpio -g mode 24 out && gpio -g write 24 1");
//			system("gpio -g write 24 1");
		}
		if(ev.value && (buttonState == 1 << DS_RING) && (prevButtonState == 0)){
			system("gpio -g mode 24 out");
			system("gpio -g write 24 0");
		}

		/* Update prevButtonState */
		prevButtonState = buttonState;
	}
}
