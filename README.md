# Multi-Kitchentimer

I got tired quickly of needing 3+ mechanical kitchen timers that either didn't keep proper time or just downright failed when you so much as looked at them.
This is the result.

- 3 adjustable timers, two with up to 60, one with up to 99 minutes
- 2 preset tea timers for my girlfriend and me that set the time to 3 and 8 minutes respectively, independant of the main timers. LED ring around the button lights up   while the timer is active.
- Alarm tones are MP3 files played by a DFPlayer Mini
- PAM8403 amplifier is used to ensure enough volume
- Running an Arduino Nano and an Arduino Pro Mini, since I ran out of I/O pins.
- Pro Mini running the tea timers and timer #3 sends the MP3 number to play to the Nano via I2c


To do:

- Currently waiting for an audio bluetooth transmitter that will enable multi-room alarms with a basic bluetooth speaker

Video demo (volume warning):
https://youtu.be/tqgynjzURug

![Image](/IMG_20200718_173309.jpg)
