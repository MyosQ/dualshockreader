#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <time.h>
#include <linux/joystick.h>

#define MIN_TIME_BETWEEN_CLICKS 0.2


int main(){
	int ps4_fd, a, version = 0;
	char numButtons = 0, numAxes = 0;
	char name[128];
	struct js_event ev;
	char internalBuff[1024];
	clock_t start = -1, lastEvent;

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


	char* buttons = calloc(numButtons, sizeof(char));
	int* axes = calloc(numAxes, sizeof(int));

	setvbuf(stdout, internalBuff, _IOFBF, 1024);
	while(1){
		read(ps4_fd, &ev, sizeof(struct js_event));

		switch(ev.type & ~JS_EVENT_INIT){
			case JS_EVENT_BUTTON:
				buttons[ev.number] = ev.value;
				break;
			case JS_EVENT_AXIS:
				axes[ev.number] = ev.value;
				break;
		}

		lastEvent = clock();
		for(a = 0; a < numButtons; a++)
			if(buttons[a] && (((double)lastEvent - (double)start)/CLOCKS_PER_SEC) > MIN_TIME_BETWEEN_CLICKS){
				printf("button %d ", a);
				printf("Axis0: %d\n", axes[0]);
				start = clock();
			}

		fflush(stdout);

		if(buttons[9]){
			close(ps4_fd);
			exit(EXIT_SUCCESS);
		}
	}

	close(ps4_fd);

	return 0;
}
