#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <time.h>
#include <linux/joystick.h>
#include <pthread.h>

#define MIN_TIME_BETWEEN_CLICKS 0.2

/* Buttons */
#define DS_SQAURE	0
#define DS_CROSS	1
#define DS_RING		2
#define DS_TRIANGLE	3
#define DS_L1		4
#define DS_R1		5
#define DS_L2		6
#define DS_R2		7
#define DS_SHARE	8
#define DS_OPTIONS	9
#define DS_LEFT_JS	10
#define DS_RIGHT_JS	11
#define DS_PS4		12
#define DS_TRACKPAD	13

/* Axes */
#define DS_AXES_LEFT_X		0
#define DS_AXES_LEFT_Y		1
#define DS_AXES_RIGHT_X		2
#define DS_AXES_L2		3
#define DS_AXES_R2		4
#define DS_AXES_RIGHT_Y		5
#define DS_AXES_ARROWS_LEFT	6
#define DS_AXES_ARROWS_DOWN	7
// 8,9,10 something with acceleration of the controller //
#define DS_AXES_ROLL		11
#define DS_AXES_YAW		12
#define DS_AXES_PITCH		13
#define DS_AXES_TRACKPAD_X	15
#define DS_AXES_TRACKPAD_Y	16
//--- 17: something with the trackpad, different values when tapped and released ---//

/* Struct for thread arguments */
struct eventTypes{
	int* buttons;
	int* ax;
};

/*--- Used in lampcontroller ---*/
#define TRY_OPEN_MAX_TIME 100
#define TRY_OPEN_SLEEP_TIME 5
#define TRY_OPEN_MAX_LOOPS TRY_OPEN_MAX_TIME/TRY_OPEN_SLEEP_TIME
#define DEVICE_PATH "/dev/input/js0"
