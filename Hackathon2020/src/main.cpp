#include <Wire.h>
#include <Zumo32U4.h>
#include "HackathonMotors.h"
#include "HackathonIRSensors.h"

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

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Buzzer buzzer;
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


void loop()
{
   initiateCountdown();
   //hackathonMotors.moveForwardWDelay(MOTOR_FORWARD_SPEED,90);
   hackathonMotors.turnLeftWDelay(MOTOR_TURN_SPEED,450);
   //hackathonMotors.moveBackwardsWDelay(MOTOR_FORWARD_SPEED,90);

}

  