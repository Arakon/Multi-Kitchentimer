# Multi-Kitchentimer

I got tired quickly of needing 3+ mechanical kitchen timers that either didn't keep proper time or just downright failed when you so much as looked at them.
This is the result.

- 3 adjustable timers, two with up to 60, one with up to 99 minutes
- 2 preset tea timers for my girlfriend and me that set the time to 3 and 8 minutes respectively, independant of the main timers. LED ring around the button lights up   while the timer is active.
- Alarm tones are MP3 files played by a DFPlayer Mini
- PAM8403 amplifier is used to ensure enough volume
- Running an Arduino Nano and an Arduino Pro Mini, since I ran out of I/O pins.
- Pro Mini running the tea timers and timer #3 sends the MP3 number to play to the Nano via I2c

# I do NOT have a circuit diagram. Connections can be traced out from the code, but so far I haven't had the time or energy to create one.

To do:

~~- Currently waiting for an audio bluetooth transmitter that will enable multi-room alarms with a basic bluetooth speaker~~

Added BT transmitter (KCX_BT_EMITTER). Sadly, it turns out its range is so bad, it can't even get the audio signal to a speaker one room over, let alone down the hallway.

Video demo (volume warning):
https://youtu.be/tqgynjzURug

![PXL_20210426_073124266](https://user-images.githubusercontent.com/12016360/116048688-b69c1980-a675-11eb-9a17-f1ec69afe43d.jpg)
