from sys import exit
import os
import time
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)

GPIO.setup(16, GPIO.IN,pull_up_down=GPIO.PUD_DOWN)

while 1:
    if GPIO.input(16):
        os.system("fswebcam -r 1280x720 --no-banner image1.jpg")
        time.sleep(2)
        os.system("./pm.sh")
        time.sleep(5)
        os.system("rm image1.jpg")
        break
    else:
        continue

GPIO.cleanup()
exit()
