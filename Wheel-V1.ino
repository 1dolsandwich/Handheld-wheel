#include <Joystick.h>

#define DLeftPin 3 // D-Pad mapping
#define DRightPin 5 
#define DUpPin 2
#define DDownPin 4

#define APin 14  // A,B,X,Y button mapping
#define BPin 10
#define XPin 15
#define YPin 16

#define X1Pin A0 // joystick pins mapping
#define Y1Pin A1
#define S1Pin 1  
#define X2Pin A2
#define Y2Pin A3
#define S2Pin 0

#define StrPin 9  // steering inputs mapping 
#define AccPin A7
#define BrkPin 8

const bool initAutoSendMode = false; // tell joystick library not to update automatically
const bool debug = true;
const int range = 1023;

int StrRaw = 0;
int AccRaw = 0;
int BrkRaw = 0;
int X1Raw  = 0;
int Y1Raw  = 0;
int X2Raw  = 0;
int Y2Raw  = 0;
int DpadDirection = 0;

int StrMax = 0;
int AccMax = 0;
int BrkMax = 0;

int StrAvg = 0;
int StrPre = 0;
int AccAvg = 0;
int AccPre = 0;
int BrkAvg = 0;
int BrkPre = 0;

int StrMin = range;
int AccMin = range;
int BrkMin = range;

int StrOut = 0;
int AccOut = 0;
int BrkOut = 0;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  10, 0,                  // Button Count, Hat Switch Count
  true, true, false,   // Yes X, Y, No Z Axis
  true, true, false,   // Yes Rx, Ry, No Rz
  false, false,          // No rudder or throttle
  true, true, true);     // Yes accelerator, brake, and steering

void setup() {
  pinMode(StrPin, INPUT);// initialize inputs
  pinMode(AccPin, INPUT);
  pinMode(BrkPin, INPUT);
  pinMode(DLeftPin, INPUT_PULLUP);
  pinMode(DRightPin, INPUT_PULLUP);
  pinMode(DDownPin, INPUT_PULLUP);
  pinMode(DUpPin, INPUT_PULLUP);
  pinMode(APin, INPUT_PULLUP);
  pinMode(BPin, INPUT_PULLUP);
  pinMode(XPin, INPUT_PULLUP);
  pinMode(YPin, INPUT_PULLUP);
  pinMode(S1Pin, INPUT_PULLUP);
  pinMode(S2Pin, INPUT_PULLUP);

  Serial.begin(9600);    // begin all communications
  Joystick.begin();
}

void loop() {
  StrRaw = analogRead(StrPin); // read raw analog values
  AccRaw = analogRead(AccPin);
  BrkRaw = analogRead(BrkPin);
  StrRaw = range - StrRaw ;    // uncomment the axes you want to invert
//AccRaw = range - AccRaw ;
//BrkRaw = range - BrkRaw ;

  X1Raw  = analogRead(X1Pin);
  Y1Raw  = analogRead(Y1Pin);
  X2Raw  = analogRead(X2Pin);
  Y2Raw  = analogRead(Y2Pin);
  X1Raw  = range - X1Raw ;     // uncomment the axes you want to invert
//Y1Raw  = range - Y1Raw ;
//X2Raw  = range - X2Raw ;
//Y2Raw  = range - Y2Raw ;

  if (StrRaw > StrMax) {    // check if theres a new max or a new min value for all the axes 
    StrMax = StrRaw;
  } else if (StrRaw < StrMin) {
    StrMin = StrRaw;
  }

  if (AccRaw > AccMax) {
    AccMax = AccRaw;
  } else if (AccRaw < AccMin) {
    AccMin = AccRaw;
  }

  if (BrkRaw > BrkMax) {
    BrkMax = BrkRaw;
  } else if (BrkRaw < BrkMin) {
    BrkMin = BrkRaw;
  }

  StrOut = map(StrRaw, StrMin, StrMax, 0, range); // map the output from min registered to max registered this session
  AccOut = map(AccRaw, AccMin, AccMax, 0, range);
  BrkOut = map(BrkRaw, BrkMin, BrkMax, 0, range);



int   StrAvg = (0.25 * StrOut) + (0.75 * StrPre); //smoooth the output
int   StrPre = StrAvg;
int   AccAvg = (0.25 * AccOut) + (0.75 * AccPre); 
int   AccPre = AccAvg;
int   BrkAvg = (0.25 * BrkOut) + (0.75 * BrkPre); 
int   BrkPre = BrkAvg;

// HERE LIES A BETTER, BUT NOT FUNCTIONAL D-PAD APPROACH AS A ROTATION AXIS (360 DEGREES), INSTEAD OF FOUR BUTTONS
/*
  int x = 0;          // update the x and y based on button presses
  int y = 0;
  if (digitalRead(DUpPin))     { x -= 1; }
  if (digitalRead(DDownPin))   { x += 1; }
  if (digitalRead(DLeftPin))   { y += 1; }
  if (digitalRead(DRightPin))  { y -= 1; }  

  if (x == 0 && y == 0) {
    // No button pressed: release hat switch
    Joystick.setHatSwitch(0, JOYSTICK_HATSWITCH_RELEASE);
  } else {
    // Calculate the angle in degrees (0°-359°) using the standard Cartesian system.
  DpadDirection = atan2(y, x) * 180.0 / PI;
  if(DpadDirection < 0) {
    DpadDirection += 360;
  }
  if (DpadDirection >= 360) {
    DpadDirection = 0;
  } }
*/

  if (debug == true) {
  Serial.print("Dpad: ");       // print debug info to serial port
  Serial.print(DpadDirection);
  Serial.print(" | Y1: ");
  Serial.print(Y1Raw);
  Serial.print(" | X: ");
  Serial.println(digitalRead(DLeftPin));
  }

  Joystick.setSteering(StrAvg); //set all button/axes states and send them out
  Joystick.setAccelerator(AccAvg);
  Joystick.setBrake(BrkAvg);
  Joystick.setXAxis(X2Raw);
  Joystick.setYAxis(Y2Raw);
  Joystick.setRxAxis(X1Raw);
  Joystick.setRyAxis(Y1Raw);
//  Joystick.setHatSwitch(0, DpadDirection);
  Joystick.setButton(0, ~digitalRead(APin)+2);
  Joystick.setButton(1, ~digitalRead(BPin)+2);
  Joystick.setButton(2, ~digitalRead(XPin)+2);
  Joystick.setButton(3, ~digitalRead(YPin)+2);
  Joystick.setButton(4, ~digitalRead(DLeftPin)+2);
  Joystick.setButton(5, ~digitalRead(DRightPin)+2);
  Joystick.setButton(6, ~digitalRead(DUpPin)+2);
  Joystick.setButton(7, ~digitalRead(DDownPin)+2);
  Joystick.setButton(8, ~digitalRead(S1Pin)+2);
  Joystick.setButton(9, ~digitalRead(S2Pin)+2);
 
  Joystick.sendState();
  delay(25);
}
