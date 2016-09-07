#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <linux/joystick.h>

int main(){
	int fd, buttonState = 0, prevButtonState = 0;
	struct js_event ev;

	if((fd = open("/dev/input/js0", O_RDONLY)) < 0){
		perror("Couldnt open file");
	exit(EXIT_FAILURE);
	}

	while(1){
		read(fd, &ev, sizeof(ev));
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
