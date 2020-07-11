/* This timer has 1 rotary encoder timer and 2 buttons with pre-set alarm times. Sending the number of the MP3 to play via I2c.*/
 
 
#include <Wire.h>
#include <TM1637.h>
#include <Debounce.h>

#define d1_DIO A0
#define d1_CLK A1

TM1637 display1(d1_CLK, d1_DIO);

int Tealight1 = 6; //yellow
int Tealight2 = 7; // blue

int index1;
volatile int direction1;
int InterruptPin1 = 2;
int RotaryEncoder1 = 5;
int PushButton1 = 4;

int Tea1 = 8;
int Tea2 = 9;
int TeaChris = 180;
int TeaSascha = 480;

Debounce tea1(Tea1);
Debounce tea2(Tea2);

int x; // Number of MP3 file to request over I2c

void setup() {
  Wire.begin();
  Serial.begin(115200);

  // Set up pins

  pinMode(InterruptPin1, INPUT);
  pinMode(RotaryEncoder1, INPUT);
  pinMode(PushButton1, INPUT);

  pinMode(Tea1, INPUT);
  pinMode(Tea2, INPUT);

  pinMode(Tealight1, OUTPUT);
  pinMode(Tealight2, OUTPUT);

  attachInterrupt(0, Direction1, RISING); // 0/1 means pin 2/3


  // Initial direction is undefined
  direction1 = 0;

  //Set up display

  display1.init();                              // Initializes the display
  display1.setBrightness(4);
  display1.switchColon();

}

void Direction1()
{
  // This function is triggered by an interrupt from the rotary encoder on pin "InterruptPin1"

  // Interrupt has occurred, now check the other rotary encoder pin to establish direction
  if (digitalRead(RotaryEncoder1))direction1 = +1;
  else                            direction1 = -1;
  return;
}

void loop()
{
  // Pre-defined starting times in seconds, up to 99 min for Timer #3

  int Times1[] = {0, 60, 120, 180, 240, 300, 360, 420, 480, 540, 600, 660, 720, 780, 840, 900, 960, 1020, 1080, 1140, 1200, 1260, 1320, 1380, 1440, 1500, 1560, 1620, 1680, 1740, 1800, 1860, 1920, 1980, 2040,
                  2100, 2160, 2220, 2280, 2340, 2400, 2460, 2520, 2580, 2640, 2700, 2760, 2820, 2880, 2940, 3000, 3060, 3120, 3180, 3240, 3300, 3360, 3420, 3480, 3540, 3600, 3660, 3720, 3780, 3840, 3900, 3960, 4020, 4080,
                  4140, 4200, 4260, 4320, 4380, 4440, 4500, 4560, 4620, 4680, 4740, 4800, 4860, 4920, 4980, 5040, 5100, 5160, 5220, 5280, 5340, 5400, 5460, 5520, 5580, 5640, 5700, 5760, 5820, 5880, 5940
                 };
  int nTimes1, StartCounting1 = 0, Time1, Count1, CountDown1, Reset1;
  long Delta1;

  int StartCountingTea1 = 0, TimeTea1, CountDownTea1;
  long DeltaTea1;
  int StartCountingTea2 = 0, TimeTea2, CountDownTea2;
  long DeltaTea2;


  // Calculate how many values in times[] array and subtract to find highest address
  nTimes1 = sizeof(Times1) / sizeof(Times1[0]) - 1;
  Serial.print("Number of times Timer 1: ");
  Serial.println(nTimes1);


  // Start at 00:00
  index1 = 0;
  Time1 = Times1[index1];
  StartCounting1 = 0;
  CountDown1 = 0;

  TimeTea1 = TeaChris;
  StartCountingTea1 = 0;
  CountDownTea1 = 0;

  TimeTea2 = TeaSascha;
  StartCountingTea2 = 0;
  CountDownTea2 = 0;


  // No direction set
  direction1 = 0;

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
    // Reset, otherwise we increment or decrement continuously
    direction1 = 0;

    // Display current time
    WriteTime1(Time1);

    // Check for HIGH on pin Pushbutton1 - this is the switch to start, pause and reset the timer
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

    if (tea1.read() == HIGH)
    {
      StartCountingTea1 = 1; // Toggles start, removed -StartCountingTea1; for pause
      CountDownTea1 = -1;
      DeltaTea1 = millis() / 1000;
    }
    if (tea2.read() == HIGH)
    {
      StartCountingTea2 = 1; // Toggles start, removed -StartCountingTea2;
      CountDownTea2 = -1;
      DeltaTea2 = millis() / 1000;
    }

    if (StartCounting1 == 1)
    { if (millis() / 1000 != Delta1)
      { Time1 = Time1 + CountDown1;
        if (Time1 == 0)
        { CountDown1 = 0;
          WriteTime1(Time1);
          index1 = 0;
          //Alarm now (Send data)
          x = 3;
          Wire.beginTransmission(9);  //9 here is the address of the slave board
          Wire.write(x);  //Transfers the MP3 number
          Wire.endTransmission();
          StartCounting1 = 0;
        }
        Delta1 = millis() / 1000;
      }
    } // END OF IF(STARTCOUNTING...

    if (StartCountingTea1 == 1)
    { if (millis() / 1000 != DeltaTea1)
      { TimeTea1 = TimeTea1 + CountDownTea1;
        digitalWrite(Tealight1, HIGH);
        if (TimeTea1 == 0)
        { CountDownTea1 = 0;
          TimeTea1 = TeaChris;
          //Alarm now (Send data)
          x = 4;
          Wire.beginTransmission(9);  //9 here is the address of the slave board
          Wire.write(x);  //Send the MP3 number over I2c
          Wire.endTransmission();
          StartCountingTea1 = 0;
          digitalWrite(Tealight1, LOW);
        }
        DeltaTea1 = millis() / 1000;
      }
    } // END OF IF(STARTCOUNTING...

    if (StartCountingTea2 == 1)
    { if (millis() / 1000 != DeltaTea2)
      { TimeTea2 = TimeTea2 + CountDownTea2;
        digitalWrite(Tealight2, HIGH);
        if (TimeTea2 == 0)
        { CountDownTea2 = 0;
          //indexTea2=0;
          TimeTea1 = TeaSascha;
          //Alarm now (Send Data)
          x = 5;
          Wire.beginTransmission(9);  //9 here is the address of the slave board
          Wire.write(x); //Send the MP3 number over I2c
          Wire.endTransmission();
          StartCountingTea2 = 0;
          digitalWrite(Tealight2, LOW);
        }
        DeltaTea2 = millis() / 1000;
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
