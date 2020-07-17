/* Two timers with rotary selectors, along with connection to DFPlayer Mini for alarm sounds*/

#include <Wire.h>
#include <DFPlayer_Mini_Mp3.h>

SoftwareSerial dfplayerserial(10, 11); // RX, TX

#include <TM1637.h>
#define d1_DIO A0
#define d1_CLK A1
#define d2_DIO A2
#define d2_CLK A3

TM1637 display1(d1_CLK, d1_DIO);
TM1637 display2(d2_CLK, d2_DIO);

int x; //MP3 number received from Pro Mini

int index1;
volatile int direction1;
int InterruptPin1 = 2;
int RotaryEncoder1 = 5;
int PushButton1 = 4;

int index2;
volatile int direction2;
int InterruptPin2 = 3;
int RotaryEncoder2 = 6;
int PushButton2 = 7;

void setup() {

  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  dfplayerserial.begin (9600);
  Serial.println("Timer V1.0");
  mp3_set_serial (dfplayerserial);  //set softwareSerial for DFPlayer-mini mp3 module
  mp3_set_volume (30);
  mp3_play (1);


  // Set up pins

  pinMode(InterruptPin1, INPUT);
  pinMode(RotaryEncoder1, INPUT);
  pinMode(PushButton1, INPUT);

  pinMode(InterruptPin2, INPUT);
  pinMode(RotaryEncoder2, INPUT);
  pinMode(PushButton2, INPUT);

  attachInterrupt(0, Direction1, CHANGE); // 0/1 means pin 2/3
  attachInterrupt(1, Direction2, CHANGE); // 0/1 means pin 2/3

  // Initial direction is undefined
  direction1 = 0;
  direction2 = 0;

  //Set up displays

  display1.init();                              // Initializes the display
  display1.setBrightness(4);
  display1.switchColon();
  display2.init();                              // Initializes the display
  display2.setBrightness(4);
  display2.switchColon();
  digitalWrite (InterruptPin1, HIGH);
  digitalWrite (RotaryEncoder1, HIGH);
  digitalWrite (InterruptPin2, HIGH);
  digitalWrite (RotaryEncoder2, HIGH);
}
void receiveEvent(int bytes) {
  x = Wire.read();  //Receive value from master board
  Serial.print(x);
  mp3_play (x);
}

void Direction1() {
  if (digitalRead(InterruptPin1) == HIGH)                        // found a low-to-high on channel A
  {
    if (digitalRead(RotaryEncoder1) == LOW)                  // check channel B to see which way encoder is turning
    {
      direction1 = -1;      // CCW
    }
    else
    {
      direction1 = +1;      // CW
    }
  }
  else                                          // found a high-to-low on channel A
  {
    if (digitalRead(RotaryEncoder1) == LOW)
    {
      direction1 = +1;      // CW
    }
    else
    {
      direction1 = -1;      // CCW
    }
  }
}

void Direction2() {
  if (digitalRead(InterruptPin2) == HIGH)                        // found a low-to-high on channel A
  {
    if (digitalRead(RotaryEncoder2) == LOW)                  // check channel B to see which way encoder is turning
    {
      direction2 = -1;      // CCW
    }
    else
    {
      direction2 = +1;      // CW
    }
  }
  else                                          // found a high-to-low on channel A
  {
    if (digitalRead(RotaryEncoder2) == LOW)
    {
      direction2 = +1;      // CW
    }
    else
    {
      direction2 = -1;      // CCW
    }
  }
}

void loop()
{
  // Pre-defined starting times in seconds, up to an hour for Timer #1 and #2
  int Times1[] = {0, 60, 120, 180, 240, 300, 360, 420, 480, 540, 600, 660, 720, 780, 840, 900, 960, 1020, 1080, 1140, 1200, 1260,
                  1320, 1380, 1440, 1500, 1560, 1620, 1680, 1740, 1800, 1860, 1920, 1980, 2040, 2100, 2160, 2220, 2280, 2340, 2400, 2460, 2520,
                  2580, 2640, 2700, 2760, 2820, 2880, 2940, 3000, 3060, 3120, 3180, 3240, 3300, 3360, 3420, 3480, 3540, 3600
                 };
  int Times2[] = {0, 60, 120, 180, 240, 300, 360, 420, 480, 540, 600, 660, 720, 780, 840, 900, 960, 1020, 1080, 1140, 1200, 1260,
                  1320, 1380, 1440, 1500, 1560, 1620, 1680, 1740, 1800, 1860, 1920, 1980, 2040, 2100, 2160, 2220, 2280, 2340, 2400, 2460, 2520,
                  2580, 2640, 2700, 2760, 2820, 2880, 2940, 3000, 3060, 3120, 3180, 3240, 3300, 3360, 3420, 3480, 3540, 3600
                 };

  int nTimes1, StartCounting1 = 0, Time1, Count1, CountDown1, Reset1;
  long Delta1;
  int nTimes2, StartCounting2 = 0, Time2, Count2, CountDown2, Reset2;
  long Delta2;

  // Calculate how many values in times[] array and subtract to find highest address
  nTimes1 = sizeof(Times1) / sizeof(Times1[0]) - 1;
  Serial.print("Number of times Timer 1: ");
  Serial.println(nTimes1);

  nTimes2 = sizeof(Times2) / sizeof(Times2[0]) - 1;
  Serial.print("Number of times Timer 2: ");
  Serial.println(nTimes2);

  // Start at 00:00
  index1 = 0;
  Time1 = Times1[index1];
  StartCounting1 = 0;
  CountDown1 = 0;

  index2 = 0;
  Time2 = Times2[index2];
  StartCounting2 = 0;
  CountDown2 = 0;

  // No direction set
  direction1 = 0;
  direction2 = 0;



  // Wait for input and display seconds requested
  while (1)
  {
    // Add +1 or -1 to prevent change of rotation being delayed
    if (direction1 != 0)
    { index1 = index1 + direction1;

      // Make sure we do not go beyond array bounds
      if (index1 < 0)index1 = nTimes1;
      if (index1 > nTimes1)index1 = 0;
      // Set the new time
      Time1 = Times1[index1];
      StartCounting1 = 0;
    }

    // Add +1 or -1 to prevent change of rotation being delayed
    if (direction2 != 0)
    { index2 = index2 + direction2;
      // Make sure we do not go beyond array bounds
      if (index2 < 0)index2 = nTimes2;
      if (index2 > nTimes2)index2 = 0;
      // Set the new time
      Time2 = Times2[index2];
      StartCounting2 = 0;
    }

    // Reset, otherwise we increment or decrement continuously
    direction1 = 0;
    direction2 = 0;

    // Display current time
    WriteTime1(Time1);
   
    WriteTime2(Time2);
 
    // Check for HIGH on pin PushButton1 - this is the switch to start, pause and reset the timer
    // A short press toggles start/pause, a longer press resets to zero
    if (digitalRead(PushButton1) == HIGH)
    {
      // Button has been pressed now wait until button is released
      Count1 = 0; // Count number of 10ms intervals that button is pressed
      Reset1 = 0; // Was it a reset or start/pause
      while (digitalRead(PushButton1) == HIGH)
      { delay(10);
        Count1++;
        // Allow 500ms (10ms x 50) to pass with button pressed to trigger a reset
        if (Count1 > 50)
        { index1 = 0;
          Time1 = 0;
          StartCounting1 = 0; // Paused
          WriteTime1(Time1);

          CountDown1 = 0;   // Counting neither up nor down
          Reset1 = 1;       // So we can skip next block of code
          while (digitalRead(PushButton1) == HIGH); // Loop until button is released
          break;
        }
      }

      // Just a quick press so start or pause
      if (Reset1 == 0)
      { StartCounting1 = 1 - StartCounting1; // Toggles start/pause
        if (index1 == 0)CountDown1 = +1; // Decide which way to count
        else        CountDown1 = -1;
      }

      Delta1 = millis() / 1000;
    }

    // Check for HIGH on pin PushButton2 - this is the switch to start, pause and reset the timer
    // A short press toggles start/pause, a longer press resets to zero
    if (digitalRead(PushButton2) == HIGH)
    {
      // Button has been pressed now wait until button is released
      Count2 = 0; // Count number of 10ms intervals that button is pressed
      Reset2 = 0; // Was it a reset or start/pause
      while (digitalRead(PushButton2) == HIGH)
      { delay(10);
        Count2++;
        // Allow 500ms (10ms x 50) to pass with button pressed to trigger a reset
        if (Count2 > 50)
        { index2 = 0;
          Time2 = 0;
          StartCounting2 = 0; // Paused
          WriteTime2(Time2); // Write the time so that the display clears to 0:00 while the button is pressed
          CountDown2 = 0;   // Counting neither up nor down
          Reset2 = 1;       // So we can skip next block of code
          while (digitalRead(PushButton2) == HIGH); // Loop until button is released
          break;
        }
      }

      // Just a quick press so start or pause
      if (Reset2 == 0)
      { StartCounting2 = 1 - StartCounting2; // Toggles start/pause
        if (index2 == 0)CountDown2 = +1; // Decide which way to count
        else        CountDown2 = -1;
      }

      Delta2 = millis() / 1000;
    }


    // Start counting - CountDown gives the direction
    if (StartCounting1 == 1)
    { if (millis() / 1000 != Delta1)
      { Time1 = Time1 + CountDown1;
        if (Time1 == 0)
        { CountDown1 = 0;
          WriteTime1(Time1);
          index1 = 0;
          //Alarm now (DFPLayer)
          mp3_play (1);
          StartCounting1 = 0; // stop counting down
        }
        Delta1 = millis() / 1000;
      }
    } // END OF IF(STARTCOUNTING...

    if (StartCounting2 == 1)
    { if (millis() / 1000 != Delta2)
      { Time2 = Time2 + CountDown2;
        if (Time2 == 0)
        { CountDown2 = 0;
          WriteTime2(Time2);
          index2 = 0;
          //Alarm now (DFPLayer)
          mp3_play (2);
          StartCounting2 = 0; // stop counting down
        }
        Delta2 = millis() / 1000;
      }
    } // END OF IF(STARTCOUNTING...

  } // END OF WHILE(1)

} // END OF loop()

void WriteTime1(int time)
{ int second, minute;
  int d1, d2, d3, d4; // Digits on display 0-9 plus "space"
  second = time % 60;
  minute = time / 60;

  // Set dx to the value of each digit
  d1 = 0;
  if (minute >= 10)d1 = minute / 10;
  d2 = minute % 10;
  d3 = second / 10;
  d4 = second % 10;
  display1.display((String)d1 + d2 + d3 + d4);

  return;
}

void WriteTime2(int time)
{ int second, minute;
  int d1, d2, d3, d4; // Digits on display 0-9 plus "space"
  second = time % 60;
  minute = time / 60;

  // Set dx to the value of each digit
  d1 = 0;
  if (minute >= 10)d1 = minute / 10;
  d2 = minute % 10;
  d3 = second / 10;
  d4 = second % 10;
  display2.display((String)d1 + d2 + d3 + d4);

  return;
}
