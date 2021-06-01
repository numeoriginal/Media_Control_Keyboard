# Carp Constantin Media_Control_Keyboard 2021
import pyautogui as keyboard
from serial import Serial

arduino = Serial(port='COM4', baudrate=9600, timeout=.1)
arduino.flushInput()
arduino.flushOutput()
pause = 0
play = 0

def map_volume(value, left_min, left_max, right_min, right_max):
    left_span = left_max - left_min
    right_span = right_max - right_min

    value_scaled = float(value - left_min) / float(left_span)

    return right_min + (value_scaled * right_span)


def volume_set(value):
    global last_volume
    map_value = map_volume(value, 0, 100, 0, 65535)
    print(f'Current system volume: {value}')
    if last_volume != map_value:
        os.system("nircmd.exe setsysvolume %d" % map_value)
        last_volume = map_value


def read():
    global pause
    global play
    arduino.write(bytes('s'.encode('utf-8')))
    data = arduino.readline().decode('utf-8').rstrip()

    if data:
        if data == "prev":
            keyboard.press('prevtrack')
            pause = 0
            play = 0
        if data == "next":
            keyboard.press('nexttrack')
            pause = 0
            play = 0
        if data == "pause" and pause != 1:
            pause = 1
            play = 0
            keyboard.press('playpause')
        if data == "resume" and play != 1:
            keyboard.press('playpause')
            pause = 0
            play = 1
        if data == "mute":
            keyboard.press('volumemute')
            pause = 0
            play = 0
        if data == "stop":
            keyboard.press('stop')
            pause = 0
            play = 0
        if data == 'win':
            keyboard.press('win')
            pause = 0
            play = 0
        if data.isnumeric():
            volume_set(int(data))


if __name__ == '__main__':
    while True:
        read()
       
