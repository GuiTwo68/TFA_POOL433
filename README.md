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
433 MHz wether station receiver library for so called TFA

Pulse should look like this:
     __       START         __   0   __     1            __STOP __
   _|  |___________________|  |_____|  |________________|  |___|  |_

Based on my measurement pulse lengths are the following:
STOP: 950 - 2000 us
0: 2000 - 3000 us
1: 3500 - 9000 us
START: 9000 - 9500 us

Pulses shorter than 950 and longer than 9500 are skipped.
Before the first data bit arrived usually 8-16 "S" pulsey are coming.
Senders are usually repeats the message 3-6 times. 

//fixed id: 0 till 3
//rolling code: 4 till 13

//Channel: 14 till 15
//Temperature: 16 till 27
//Battery


Inspiration

This is an adaptation of Denxhum thanks to him.
https://github.com/denxhun/TFA433
