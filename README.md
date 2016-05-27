# Smiley-Pack
An animated backpack for gags

Hardware Requirments: 
- Maxbotix Ultrasonic Rangefinder (EZ0 was my choice)
- Arduino (Pro Mini used here)
- Micro SD Card reader (pjrc adapter used here)
- Adafruit 8x8 Neopixel Matrix
- A Micro SD Card

Purpose:
This project is to be mounted on a backpack with both the rangefinder and the Matrix pointed backwards. The project will then make faces at people it detects walking behind you, optimal places to use it are places such as the mall, universities and workplaces.

Loading Images:
Use the python script provided to create the img.dat file which will be placed at the root of the sd card. The script takes paths to images, starting from closest to the sensor readings and getting farther away. The arduino will automatically detect the number of images loaded and scale its range to the appropriate values.

Notes:
I have been getting varing range values based on device and voltage supplied. I'm looking into making the scale dynamic.
