#include <Wire.h>
#include <Zumo32U4.h>
#include "HackathonMotors.h"
#include "HackathonIRSensors.h"

Zumo32U4LCD lcd;

Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4Buzzer buzzer;

#define MOTOR_FORWARD_SPEED 150
#define MOTOR_FORWARD_MAX_SPEED 400
#define MOTOR_REVERSE_SPEED 200
#define MOTOR_REVERSE_MAX_SPEED 400

#define MOTOR_TURN_SPEED 150
#define MOTOR_TURN_MAX_SPEED 400
#define MOTOR_ACQ_TURN_SPEED 300
#define MOTOR_ACQ_TURN_MAX_SPEED 400
#define MOTOR_ACQ_TURN_ANGLE 20

#define BORDER_DETECT_MIN_TURN_ANGLE 75  //Degrees
#define BORDER_DETECT_MAX_TURN_ANGLE 175 //Degrees

#define MOTOR_REVERSE_DURATION 300 // ms
// #define MOTOR_TURN_DURATION     200 // ms

#define LINE_SENSORS_THRESHOLD 1000 // microseconds

#define PROX_FRONT_SENSORS_SCAN_THRESHOLD 2
#define PROX_SIDE_SENSORS_SCAN_THRESHOLD 2

#define TARGET_ACQUIRED_SENSIBILITY 1

int right_threshold;
int left_threshold;
HackathonMotors hackathonMotors;
HackathonIRSensors hackathonIRSensors(LINE_SENSORS_THRESHOLD, PROX_FRONT_SENSORS_SCAN_THRESHOLD, PROX_SIDE_SENSORS_SCAN_THRESHOLD, TARGET_ACQUIRED_SENSIBILITY);

//const char sound_effect[] PROGMEM = "O4 T100 V15 L4 MS g12>c12>e12>G6>E12 ML>G2"; // "charge" melody

void initiateCountdown()
{
  ledRed(1);
  ledYellow(0);
  ledGreen(0);

  lcd.gotoXY(0, 0);
  lcd.print(F("Press A"));
  lcd.gotoXY(0, 1);
  lcd.print(F("to start!"));

  buttonA.waitForButton();

  ledRed(0);
  ledYellow(1);

  for (int i = 0; i < 4; i++)
  {
    buzzer.playNote(NOTE_G(3), 200, 15);
    delay(1000);
  }

  ledYellow(0);
  ledGreen(1);

  buzzer.playNote(NOTE_G(4), 500, 15);
  delay(1000);

  ledGreen(0);

  lcd.clear();
}

void setup()
{
  hackathonMotors.init();
  hackathonIRSensors.init();

  randomSeed((unsigned int)millis());
  hackathonIRSensors.readLineSensors();
  right_threshold = hackathonIRSensors.lineSensorRightCount - 400;
  left_threshold = hackathonIRSensors.lineSensorLeftCount - 400;
  //initiateCountdown();
}

void StopMotor(){
  hackathonMotors.setSpeeds(0,0);
  delay(500);
}

void MoveFwdLeft_180(int milli){
  hackathonMotors.setSpeeds(100,180);
  delay((int)(3.35 * milli));
  StopMotor();
}

void MoveFwdRight_180(int milli){
  hackathonMotors.setSpeeds(180,100);
  delay((int)(3.35 * milli));
  StopMotor();
}

void MoveFwd(int milli){
  hackathonMotors.setSpeeds(120,100);
  delay((int)(3.35 * milli));
  StopMotor();
}

void MoveBckRight_180(int milli){
  hackathonMotors.setSpeeds(-100,-180);
  delay((int)(3.35 * milli));
  StopMotor();
}

void MoveBckLeft_180(int milli){
  hackathonMotors.setSpeeds(-160,-100);
  delay((int)(3.35 * milli));
  StopMotor();
}


void MoveBck(int milli){
  hackathonMotors.setSpeeds(-100,-110);
  delay((int)(3.35 * milli));
  StopMotor();
}


void ARotateLeft(int degree){
  float perDegree = 12.5;
  hackathonMotors.setSpeeds(-90,90);
  delay((int)(perDegree*degree));
  StopMotor();
}


void ARotateRight(int degree){
  float perDegree = 12.5;
  hackathonMotors.setSpeeds(90,-90);
  delay((int)(perDegree*degree));
  StopMotor();
}


void Move10Times(){
  int count = 0;
  while(count < 10)
  {
    hackathonMotors.setSpeeds(50, 50);
    delay(5000);
    hackathonMotors.setSpeeds(0, 0);
    count = count + 1;
  }
  hackathonMotors.setSpeeds(0, 0); 
}

void loop()
{
   initiateCountdown();
   //hackathonMotors.moveForwardWDelay(MOTOR_FORWARD_SPEED,90);
   //1
    ARotateLeft(5);
    ARotateLeft(4);
    ARotateLeft(3);
    //2
    MoveBckLeft_180(65);
    //3
    MoveFwdLeft_180(55);
   //4
    ARotateLeft(5);
    ARotateLeft(4);
    ARotateLeft(3);
    //5
     MoveBck(125);
    // //6
    ARotateRight(5);
    ARotateRight(4);
    ARotateRight(3);
    ARotateRight(3);
    //ARotateRight(2);
    //ARotateRight(1);
    // //7
     MoveFwdRight_180(75);
    
   //hackathonMotors.moveBackwardsWDelay(MOTOR_FORWARD_SPEED,90);

}


