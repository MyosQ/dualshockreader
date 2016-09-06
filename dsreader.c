#include "myps4.h"

int main(){
	int ps4_fd, a, version = 0, buttonState = 0;
	char numButtons = 0, numAxes = 0;
	char name[128];
	struct js_event ev;
	char internalBuff[1024];
	clock_t start = -1, lastEvent;

	setvbuf(stdout, internalBuff, _IOFBF, 1024);

	/* Open controller */
	if((ps4_fd = open("/dev/input/js0", O_RDONLY | O_NONBLOCK)) < 0){
		perror("Couldnt open controller");
		exit(EXIT_FAILURE);
	}

	/* Get info of controller */
	ioctl(ps4_fd, JSIOCGBUTTONS, &numButtons);
	ioctl(ps4_fd, JSIOCGAXES, &numAxes);
	ioctl(ps4_fd, JSIOCGVERSION, &version);
	if(ioctl(ps4_fd, JSIOCGNAME(sizeof(name)), name) < 0)
		strncpy(name, "Unknown", sizeof(name));
	printf("Controller detected!\n");
	printf("Version: %d\nName: %s\nNumber of buttons: %d\nNumber of axes: %d\n",version, name, (int)numButtons, (int)numAxes);
	fflush(stdout);

	char* buttons = calloc(numButtons, sizeof(char));
	int* axes = calloc(numAxes, sizeof(int));

	while(1){
		read(ps4_fd, &ev, sizeof(struct js_event));

		switch(ev.type & ~JS_EVENT_INIT){
			case JS_EVENT_BUTTON:
				buttons[ev.number] = ev.value;
				if(ev.value)
					buttonState |= (1 << ev.number);
				else
					buttonState &= ~(1 << ev.number);
				break;
			case JS_EVENT_AXIS:
				axes[ev.number] = ev.value;
				break;
		}

		lastEvent = clock();
		if((((double)lastEvent - (double)start)/CLOCKS_PER_SEC) > MIN_TIME_BETWEEN_CLICKS){
//			if(buttonState & 1){
//				printf("square  ");
				printf("buttonState: %6d ", buttonState);
				printf("Axis 0: %6d ", axes[0]);
				printf("Axis 1: %6d ", axes[1]);
				printf("Axis 2: %6d ", axes[2]);
				printf("Axis 3: %6d ", axes[3]);
				printf("Axis 4: %6d ", axes[4]);
				printf("Axis 5: %6d ", axes[5]);
//				printf("Axis 4: %d", axes[4]);
//				printf("Axis 5: %d", axes[5]);
//				printf("Axis 6: %d", axes[6]);
//				printf("Axis 7: %d", axes[7]);
//				printf("Axis 8: %d", axes[8]);
				printf("\n");
				start = clock();

//			}
		}

		fflush(stdout);


		/* Exit with OPTIONS-button */
		if(buttons[9]){
			close(ps4_fd);
			exit(EXIT_SUCCESS);
		}
	}

	return 0;
}
