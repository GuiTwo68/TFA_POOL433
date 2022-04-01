# TFA_POOL433
Arduino TFA 433 for TFA dostmann 30.3240.10

TFA 433
Use your Arduino to receive temperature data from TFA POLL remote sensor 30.3240.10.

Info
Hardware
There are several 433 MHz receiver available in many places. I used a cheap type and it works perfectly. You should use Arduino's interrupt enabled digital input pins (on Uno/Nano: D2 or D3) since this library built on top of that feature.

Usage
Use at your own risk! Truly! There is no warranty!

In the examples directory you can find a simple way of usage.

Protocol

Inspiration

This is an adaptation of Denxhum thanks to him.
https://github.com/denxhun/TFA433
