# Media_Control_Keyboard
https://ocw.cs.pub.ro/courses/pm/prj2021/dbrigalda/tastatura_multimedia

Media Controll Keyboard based on Arduino Uno.
Python:

-map_volume(value, left_min, left_max, right_min, right_max) -- maps the potentiometre value map(value_to_be_mapped,0, 100, 0, 65535) for the nircmd which use 0-65535 range.
-volume_set(value) -- sets the system to the value given as parameter
-read() -- reads data from the serial and prelucrates the data.

Read the data continuously in a loop.

Arduino:

First value sent through the serial is the current potentiometre value to syncronize with windows volume.

In the main loop we are reading the data from potentiometre maping the values to a smaller range(0,1023,0,100) for a easier manipulation.
Based on the mapped_pot_value we change the intensity of the led to indicate the volume, and we send the mapped value
through the serial. We read in the python the value and set the windows volume.

Also in main loop we are reading the data from joystick, also maping the values(0, 1023, 1, 4) and sending a signal through serial
in the form of text:
    "next" x-axis-right,
    "prev" x-axis-left,
    "win" y-axis-down,
    "stop" y-axis-up.

There are two attached intrerrupts on two buttons one is for pause one for resume, with deboucing, also sending through
serial a signal were it is received in python script and the corresponding action is taken.

There is a manual interrupt set on the joystick button for mute.
