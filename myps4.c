#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <time.h>
#include <linux/joystick.h>
/*
struct js_event {
	unsigned int time;
	short value;
	unsigned char type;
	unsigned char number;
};
*/



int main(){
	int ps4_fd, numButtons = 0, a;
	struct js_event ev;


	/* Open controller */
	if((ps4_fd = open("/dev/input/js0", O_RDONLY)) < 0){
		perror("Couldnt open controller");
		exit(EXIT_FAILURE);
	}

	ioctl(ps4_fd, JSIOCGBUTTONS, &numButtons);
	printf("Controller detected: %d buttons\n", numButtons);

	while(1){
		a = read(ps4_fd, &ev, sizeof(struct js_event));
		if(a != sizeof(struct js_event)){
			perror("read error");
			exit(1);
		}
		else
			printf("%d, %d, %d, %d\n", ev.time, ev.value, ev.type, ev.number);

		sleep(4);
	}

	close(ps4_fd);

	return 0;
}
