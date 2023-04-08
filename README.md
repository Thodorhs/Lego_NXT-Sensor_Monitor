# Lego_NXT-Sensor_Monitor
Custom Lego NXT firmware for using and monitoring I/O Sensors, Buttons.

Implemented I2C transfers between ARM and AVR (co-processor) using TWI controller.

Essentialy the transfers are implented using a state machine for sending and receiving.

DISLAIMER for testing the sensor you need to arrange the sensors on the brick like this:

OUT PORTS A B C : 
only for motors

IN PORTS 1 2 3 4 :

port 1 on brick :: button

port 2 on brick :: microphone

port 3 on brick :: lightsensor

port 4 on brick :: reserved

# Test
USE sensor button to drive the mototors, 

USE flash to scare the NXT backwards,

SPEAK and watch the led react,

POWER OFF the nxt using the exit button,

REPROGRAM the NXT using the enter button.


# Prerequisites
- make
- (ubuntu easier setup)
- Lego toolchain repo

# How to run
In the makefile change the PREFIX (first line) to the lego toolchange dir e.g PREFIX=../lego-toolchain_23-master

- make clean
- make
- set NXT on re-program mode (one way to do this is by the pin hidden on the back of the brick)
- make download
