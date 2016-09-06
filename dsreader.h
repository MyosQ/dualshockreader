#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <time.h>
#include <linux/joystick.h>

#define MIN_TIME_BETWEEN_CLICKS 0.2

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


#define DS_AXES_LEFT_X		0
#define DS_AXES_LEFT_Y		1
#define DS_AXES_RIGHT_X		2
#define DS_AXES_L2		3
#define DS_AXES_R2		4
#define DS_AXES_RIGHT_Y		5

