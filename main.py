import pyautogui as keyboard
from serial import Serial

arduino = Serial(port='COM4', baudrate=9600, timeout=.1)
arduino.flushInput()
arduino.flushOutput()
pause = 0
play = 0


def read():
    global pause
    global play
    arduino.write(bytes('s'.encode('utf-8')))
    data = arduino.readline().decode('utf-8').rstrip()

    if data:
        if data == "Up":
            keyboard.press('volumeup')
            pause = 0
            play = 0
        if data == "Down":
            keyboard.press('volumedown')
            pause = 0
            play = 0
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


if __name__ == '__main__':
    while True:
        read()
       
