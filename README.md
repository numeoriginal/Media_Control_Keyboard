# Media_Control_Keyboard
https://ocw.cs.pub.ro/courses/pm/prj2021/dbrigalda/tastatura_multimedia?&amp;#software_design

Media Controll Keyboard based on Arduino Uno.
In the main loop we are reading the data from potentiometre maping the values to a smaller range for a easier manipulation.
Based on the data we change the intensity of the led to indicate the volume, and we send a signal either "Up" or "Down"
through the serial. We read in the python the signal and press volume_up/volume_down.

Also in main loop we are reading the data from joystick, also maping the values and sending a signal through serial
in the form of text "next" x-axis-right, "prev" x-axis-left, "win" y-axis-down, "stop" y-axis-up.

There are to attached intrerrupts on 2 buttons one is for pause one for resume, with deboucing, also sending through
serial a signal were it is received in python script and the corresponding action is taken.

There is a manual interrupt set on the joystick button for mute.
