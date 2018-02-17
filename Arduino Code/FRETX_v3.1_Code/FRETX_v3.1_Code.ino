#include "pins.h"
                                 
/* initialize the pins to control the matrix */
int     stringTab[] = {S1, S2, S3, S4, S5, S6, -1};
int     stringiTab[] = {S6, S5, S4, S3, S2, S1, -1};
int     fretTab[] = {F0, F1, F2, F3, F4, F5, -1};

String note = "zzzzzzzzzzz";
int povDelayMs;
int pos = 0;
int status_pin = 13;
int vertical_sync = 0;
int horizontal_sync = 0;
int just_con = 0;
int data_received = 1;
int con_status = 0;
int once = 0;
int string_no = 0;
int h_delay = 25;

void setup() {
  //initialize the serial communication with HC-05
  Serial.begin(9600);
  while(Serial == NULL);
  Serial.setTimeout(100);
  pinMode(status_pin, INPUT);
  //TEST PURPOSE ONLY
  povDelayMs = 1000 / FPS / note.length();
  pos = 0;
}

void loop() {
  while(digitalRead(status_pin) == LOW)
  {
    vertical_show();
    delay(400);
    data_received = 1;
    horizontal_sync = 0;
  }
  if(just_con == 1)
  {
    //switch off all the LEDs
    int i;
    for(i = 0; i < 6; i++)
    {
      pinMode(stringTab[i], OUTPUT);
      digitalWrite(stringTab[i], LOW);
    }
    int x;
    for(x = 0; x < 6; x++)
    {
      pinMode(fretTab[x], OUTPUT);
      digitalWrite(fretTab[x], HIGH);
    }
    just_con = 0;
    vertical_sync = 0;
  }
  
  if(data_received > 0)
  {
    if(digitalRead(status_pin) == HIGH)
    {
      //do horizontal show
      horizontal_show();
      //delay(400);
    }
    else
    {
      data_received = 0;
      just_con = 0;
      vertical_sync = 0;
    }
  }
  static int pos = 0;
  int fret;
  int string;
  
  //go back to the beginning
  if (note[pos] == '\0')
    {
      pos = 0;
      return;
    }
  
  //extracting fret and string posisiton
  fret = note[pos] / 10;
  string = note[pos] % 10;
  
//  Serial.print("string:");
//  Serial.print(string);
//  Serial.print(", fret:");
//  Serial.println(fret);
  
  //LED ON
  pinMode(fretTab[fret], OUTPUT);
  digitalWrite(fretTab[fret], LOW);    
  pinMode(stringTab[6-string], OUTPUT);
  digitalWrite(stringTab[6-string], HIGH);  
  
  //POV
  delay(povDelayMs);
  
  //LED OFF
  pinMode(fretTab[fret], INPUT);
  pinMode(stringTab[6-string], INPUT);
  
  //go to the next led
  pos++;
}

void vertical_show(void)
{
  if(vertical_sync == 5)
  {
    vertical_sync = 0;
    pinMode(fretTab[4], OUTPUT);
    digitalWrite(fretTab[4], HIGH);
  }
  pinMode(fretTab[vertical_sync], OUTPUT);
  digitalWrite(fretTab[vertical_sync], LOW);
  if(vertical_sync != 0)
  {
    pinMode(fretTab[vertical_sync-1], OUTPUT);
    digitalWrite(fretTab[vertical_sync-1], HIGH);
  }
  pinMode(stringTab[0], OUTPUT);
  digitalWrite(stringTab[0], HIGH);
  pinMode(stringTab[1], OUTPUT);
  digitalWrite(stringTab[1], HIGH);
  pinMode(stringTab[2], OUTPUT);
  digitalWrite(stringTab[2], HIGH);
  pinMode(stringTab[3], OUTPUT);
  digitalWrite(stringTab[3], HIGH);
  pinMode(stringTab[4], OUTPUT);
  digitalWrite(stringTab[4], HIGH);
  pinMode(stringTab[5], OUTPUT);
  digitalWrite(stringTab[5], HIGH);
  vertical_sync++;
  just_con = 1;
}

void horizontal_show(void)
{
    if(string_no != 6)
    {
      //switch on the string 1 red
      if(string_no != 0)
      {
        pinMode(stringiTab[string_no - 1], OUTPUT);
        digitalWrite(stringiTab[string_no - 1], LOW);
      }
      else
      {
        pinMode(stringiTab[5], OUTPUT);
        digitalWrite(stringiTab[5], LOW);
      }
      pinMode(stringiTab[string_no], OUTPUT);
      digitalWrite(stringiTab[string_no], HIGH);
      
    }
    else
    {
      //switch on the string 1 red
      pinMode(stringiTab[5], OUTPUT);
      digitalWrite(stringiTab[5], LOW);
      pinMode(stringiTab[0], OUTPUT);
      digitalWrite(stringiTab[0], HIGH);
      string_no = 0;
    }
  long time1 = 150;
  long time2 = 150;
  while(time1 > 0)
  {
    //red lights on
    //delay(1);
    pinMode(fretTab[0], OUTPUT);
      digitalWrite(fretTab[0], HIGH);
      pinMode(fretTab[1], OUTPUT);
      digitalWrite(fretTab[1], LOW);
      pinMode(fretTab[2], OUTPUT);
      digitalWrite(fretTab[2], LOW);
      pinMode(fretTab[3], OUTPUT);
      digitalWrite(fretTab[3], LOW);
      pinMode(fretTab[4], OUTPUT);
      digitalWrite(fretTab[4], LOW);
      time1--;
  }
      //delay(1);
      while(time2 > 0)
      {
        //blue light on
        pinMode(fretTab[0], OUTPUT);
      digitalWrite(fretTab[0], LOW);
    pinMode(fretTab[1], OUTPUT);
      digitalWrite(fretTab[1], HIGH);
      pinMode(fretTab[2], OUTPUT);
      digitalWrite(fretTab[2], HIGH);
      pinMode(fretTab[3], OUTPUT);
      digitalWrite(fretTab[3], HIGH);
      pinMode(fretTab[4], OUTPUT);
      digitalWrite(fretTab[4], HIGH);
    time2--;
  }
  if(h_delay == 0)
  {
    h_delay = 30;
    string_no++;
  }
  else
  {
    h_delay--;
  }
  //switch off all the LEDs
    int i;
    for(i = 0; i < 6; i++)
    {
      pinMode(stringTab[i], INPUT);
    }
    int x;
    for(x = 0; x < 6; x++)
    {
      pinMode(fretTab[x], INPUT);
    }
} 


void serialEvent(){
  note = Serial.readString();
  povDelayMs = 1000 / FPS / note.length();
  pos = 0;
  if(data_received > 0)
  {
    data_received--;
  } 
}
