#include "dsreader.h"

/* Open controller */
int ds_open(){
	int fd;
	if((fd = open("/dev/input/js0", O_RDONLY | O_NONBLOCK)) < 0){
		perror("Couldnt open controller");
		exit(EXIT_FAILURE);
	}
	return fd;
}

/* Get info of controller */
void ds_getInfo(int fd, char* numButtons, char* numAxes){
	int version;
	char name[128];
	ioctl(fd, JSIOCGBUTTONS, numButtons);
	ioctl(fd, JSIOCGAXES, numAxes);
	ioctl(fd, JSIOCGVERSION, &version);
	if(ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0)
		strncpy(name, "Unknown", sizeof(name));
	printf("Controller detected!\n");
	printf("Version: %d\nName: %s\nNumber of buttons: %d\nNumber of axes: %d\n",version, name, *numButtons, *numAxes);
}

/* Thread1 executes this */
void* printEvents(void* arguments){

	struct eventTypes* args = (struct eventTypes*)arguments;
	int* axes = args->ax;
	int* buttonState = args->buttons;

	clock_t start, lastEvent = 0;
	while(1){
		start = clock();
		if((((double)start - (double)lastEvent)/CLOCKS_PER_SEC) > MIN_TIME_BETWEEN_CLICKS){
			printf("Buttonstate: %d  Axis 0: %d",*buttonState, axes[0]);
			printf("\n");
			lastEvent = clock();
		}
	}
}

int main(){
	int ds_fd, a, buttonState = 0, *axes, ret;
	char numButtons = 0, numAxes = 0;
	struct js_event ev;
	pthread_t thread1;
	struct eventTypes* argsThread1 = malloc(sizeof(struct eventTypes));

	/* Open controller */
	ds_fd = ds_open();

	/* Get info of controller */
	ds_getInfo(ds_fd, &numButtons, &numAxes);

	/* Allocate for axes values */
	axes = calloc(numAxes, sizeof(int));

	/* Create thread for handling events */
	argsThread1->buttons = &buttonState;
	argsThread1->ax = axes;
	ret = pthread_create(&thread1, NULL, printEvents, (void*)argsThread1);
	if(ret != 0){
		fprintf(stderr,"thread_create error. Code: %d", ret);
		exit(EXIT_FAILURE);
	}

	/* Infinite Loop reading */
	while(1){
		read(ds_fd, &ev, sizeof(struct js_event));

		switch(ev.type & ~JS_EVENT_INIT){
			case JS_EVENT_BUTTON:
				if(ev.value)
					buttonState |= (1 << ev.number);
				else
					buttonState &= ~(1 << ev.number);
				break;
			case JS_EVENT_AXIS:
				axes[ev.number] = ev.value;
				break;
		}


		/* Exit with OPTIONS-button */
		if(buttonState & 1 << DS_OPTIONS){
			close(ds_fd);
			exit(EXIT_SUCCESS);
		}
	}

	return 0;
}
