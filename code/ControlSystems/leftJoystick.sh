#!/bin/bash
# contains and uses this version of Jason White's joystick.c at bottom
# https://gist.github.com/jasonwhite/c5b2048c15993d285130/eb3bcd6e1cd88002d21764b9965d075eceaf4b83
#
# requires SNES joystick connected to Raspberry USB, pigpio library installed
#
#contains serial communication code, hardcoded to use ttyUSB0


js=/tmp/leftjoystick
if [ ! -f $js ]; then sed -n "/^\/\*\*$/,\$p" $0 | gcc -x c - -o $js; fi

g=1500
l=1500
u=600
p=1500
d=100
calib=0

#port=/dev/ttyUSB0
#echo "Setting up serial link to Arduino"
#stty -F $port 115200 -hupcl cs8 cread min 0 time 10
#stty -F $port 230400 -hupcl -echo cs8 cread min 0 time 10
echo "Ready"

while IFS= read -r line; do


echo $line
#echo $line > $port
echo $line > leftjsdata.txt
#cat joystickdata.txt 
#sleep .1
done < <($js)

exit
/**
 * Author: Jason White
 *
 * Description:
 * Reads joystick/gamepad events and displays them.
 *
 * Compile:
 * gcc joystick.c -o joystick
 *
 * Run:
 * ./joystick [/dev/input/jsX]
 *
 * See also:
 * https://www.kernel.org/doc/Documentation/input/joystick-api.txt
 */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>


/**
 * Reads a joystick event from the joystick device.
 *
 * Returns 0 on success. Otherwise -1 is returned.
 */
int read_event(int fd, struct js_event *event)
{
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    /* Error, could not read full event. */
    return -1;
}

/**
 * Returns the number of axes on the controller or 0 if an error occurs.
 */
size_t get_axis_count(int fd)
{
    __u8 axes;

    if (ioctl(fd, JSIOCGAXES, &axes) == -1)
        return 0;

    return axes;
}

/**
 * Returns the number of buttons on the controller or 0 if an error occurs.
 */
size_t get_button_count(int fd)
{
    __u8 buttons;
    if (ioctl(fd, JSIOCGBUTTONS, &buttons) == -1)
        return 0;

    return buttons;
}

/**
 * Current state of an axis.
 */
struct axis_state {
    short x, y;
};

/**
 * Keeps track of the current axis state.
 *
 * NOTE: This function assumes that axes are numbered starting from 0, and that
 * the X axis is an even number, and the Y axis is an odd number. However, this
 * is usually a safe assumption.
 *
 * Returns the axis that the event indicated.
 */
size_t get_axis_state(struct js_event *event, struct axis_state axes[3])
{
    size_t axis = event->number / 2;

    if (axis < 3)
    {
        if (event->number % 2 == 0)
            axes[axis].x = event->value;
        else
            axes[axis].y = event->value;
    }

    return axis;
}

int main(int argc, char *argv[])
{
    const char *device;
    int js;
    struct js_event event;
    struct axis_state axes[3] = {0};
    size_t axis;

    if (argc > 1)
        device = argv[1];
    else
        device = "/dev/input/js0";

    js = open(device, O_RDONLY);

    if (js == -1)
        perror("Could not open joystick");

    /* This loop will exit if the controller is unplugged. */
    while (read_event(js, &event) == 0)
    {
        switch (event.type)
        {
            case JS_EVENT_BUTTON:
                printf("Button %u %s\n", event.number, event.value ? "pressed" : "released");
                break;
            case JS_EVENT_AXIS:
                axis = get_axis_state(&event, axes);
                if (axis < 1)
//                    printf("Axis %zu at (%6d, %6d)\n", axis, axes[axis].x, axes[axis].y);
                    //printf("%6d\n", (axes[axis].y)/128);
                    printf("%6dL\n", axes[axis].y);
					//sleep(1);
                break;
            default:
                /* Ignore init events. */
                break;
        }
        fflush(stdout);
    }

    close(js);
    return 0;
}
