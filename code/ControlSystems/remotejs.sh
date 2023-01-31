#!/bin/bash

port=/dev/ttyUSB1
baudrate=9600
x=1

echo "Setting up Serial link at a baudrate of $baudrate"
stty -F $port $baudrate -hupcl -echo cs8 cread min 0 time 10
echo "reading serial line"

while [ $x = 1 ]; do
#cat joystickdata.txt > $port
#cat rightjsdata.txt > $port
#cat rightjsdata.txt 
#sleep .01
cat leftjsdata.txt > $port
cat leftjsdata.txt 
sleep .05
#cat joystickdata.txt
#sleep 0.1
done
