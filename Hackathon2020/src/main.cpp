#include <Zumo32U4.h>
#include "HackathonMotors.h"

/************************************************************************************* 
   Defining Constant
 ************************************************************************************/
#define LINE_SENSORS_THRESHOLD   1300  // microseconds
#define PROX_FRONT_SENSORS_SCAN_THRESHOLD 2
#define PROX_SIDE_SENSORS_SCAN_THRESHOLD  2
#define RA_SIZE 3  // number of readings to include in running average of accelerometer readings
#define XY_ACCELERATION_THRESHOLD 2400  // for detection of contact (~16000 = magnitude of acceleration due to gravity)
#define MOTOR_FORWARD_SPEED     350
#define MOTOR_FORWARD_MAX_SPEED 400
#define MOTOR_REVERSE_SPEED     200
#define MOTOR_REVERSE_MAX_SPEED 400
#define MOTOR_TURN_SPEED        200
#define MOTOR_TURN_MAX_SPEED    400
#define MOTOR_ACQ_TURN_SPEED    300
#define TARGET_ACQUIRED_SENSIBILITY 1
#define MOTOR_REVERSE_DURATION  300 // ms
#define MOTOR_TURN_DURATION     100 // ms

/************************************************************************************* 
   Defining Variables
 ************************************************************************************/
Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Buzzer buzzer;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proximitySensors;
HackathonMotors hackathonMotors;

const char title[] PROGMEM = "MECHATRONICS - David, Domah, Lubrun, Poorun";
const char sound_effect[] PROGMEM = "O4 T100 V15 L4 MS g12>c12>e12>G6>E12 ML>G2"; // "charge" melody
unsigned int lineSensorValues[3];
uint8_t scrollingTitleDisplayPosition = 0;
uint16_t scrollingTitleLastShiftTime = millis() - 2000;
unsigned long loop_start_time;
unsigned long last_turn_time;
unsigned long contact_made_time;
#define MIN_DELAY_AFTER_TURN          400  // ms = min delay before detecting contact event
#define MIN_DELAY_BETWEEN_CONTACTS   1000  // ms = min delay between detecting new contact event

/************************************************************************************* 
   METHODS
 ************************************************************************************/
void scrollTitleOnLCD() {
  if ((uint16_t)(millis() - scrollingTitleLastShiftTime) > 250) {
      scrollingTitleLastShiftTime = millis();

      lcd.gotoXY(0, 0);
      
      for (uint8_t i = 0; i < 8; i++) {
        char c = pgm_read_byte(title + scrollingTitleDisplayPosition + i);
        
        lcd.print(c);
      }

      scrollingTitleDisplayPosition++;

      if(scrollingTitleDisplayPosition + 8 >= (uint8_t)(strlen(title))) {
        scrollingTitleDisplayPosition = 0;
      }
    }
}

void initiateCountdown() {
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

  for (int i = 0; i < 4; i++) {
    //buzzer.playNote(NOTE_G(3), 200, 15);
    delay(1000);
  }

  ledYellow(0);
  ledGreen(1);
  
  //buzzer.playNote(NOTE_G(4), 500, 15);
  delay(1000);

  ledGreen(0);

  lcd.clear();
}


bool check_for_border(){
  lineSensors.read(lineSensorValues);
	return lineSensorValues[0] < LINE_SENSORS_THRESHOLD || lineSensorValues[2] < LINE_SENSORS_THRESHOLD;
}

void on_border_detect(){
	  lcd.gotoXY(0, 0);
    lcd.print(lineSensorValues[0]);
    lcd.gotoXY(0, 1);
    lcd.print("R:");
    lcd.print(lineSensorValues[2]);

    hackathonMotors.moveBackwards(MOTOR_REVERSE_MAX_SPEED);
    delay(MOTOR_REVERSE_DURATION);
    //Border closer to left end
    if(lineSensorValues[0] <= lineSensorValues[2]) {
      //Turn clockwise at normal speed
      //motors.setSpeeds(MOTOR_TURN_SPEED, -MOTOR_TURN_SPEED);
      lcd.gotoXY(0, 0);
      lcd.print(" RIGHT ");
        hackathonMotors.turnRightWAngle(MOTOR_TURN_MAX_SPEED,113);
    //Border closer to right end
    }else{
      //Turn anti-clockwise at normal speed
            lcd.gotoXY(0, 0);
            lcd.print(" LEFT ");
      hackathonMotors.turnLeftWAngle(MOTOR_TURN_MAX_SPEED,113);
    } 
}

bool search(){
	proximitySensors.read();

  bool targetInFront = false;

  if(proximitySensors.countsFrontWithLeftLeds() > PROX_FRONT_SENSORS_SCAN_THRESHOLD || proximitySensors.countsFrontWithRightLeds() > PROX_FRONT_SENSORS_SCAN_THRESHOLD) {
    //Target detected with front proximity sensors

    lcd.gotoXY(0, 1);
    lcd.print("   ACQ  ");

    targetInFront = abs(proximitySensors.countsFrontWithLeftLeds() - proximitySensors.countsFrontWithRightLeds()) < TARGET_ACQUIRED_SENSIBILITY && proximitySensors.countsFrontWithRightLeds() != 0 &&  proximitySensors.countsFrontWithLeftLeds() != 0;

    while(!targetInFront){
      //Target more to the left
      if(proximitySensors.countsFrontWithLeftLeds() > proximitySensors.countsFrontWithRightLeds()){
        //Turn anti-clockwise at target acquisition speed
        hackathonMotors.turnLeftWAngle(MOTOR_ACQ_TURN_SPEED,10);
      //Target more to the right
      }else{
        //Turn clockwise at target acquisition speed
        hackathonMotors.turnRightWAngle(MOTOR_ACQ_TURN_SPEED,10);
      }

      proximitySensors.read();

      targetInFront = abs(proximitySensors.countsFrontWithLeftLeds() - proximitySensors.countsFrontWithRightLeds()) < TARGET_ACQUIRED_SENSIBILITY && proximitySensors.countsFrontWithRightLeds() != 0 &&  proximitySensors.countsFrontWithLeftLeds() != 0;
    }
    hackathonMotors.stop();

  //Target detected with side proximity sensors  
  } if(proximitySensors.countsLeftWithLeftLeds() > PROX_SIDE_SENSORS_SCAN_THRESHOLD || proximitySensors.countsRightWithRightLeds() > PROX_SIDE_SENSORS_SCAN_THRESHOLD){
    lcd.gotoXY(0,0);
    ledGreen(1);
    lcd.print("TEST");
    //Target to the left
    if(proximitySensors.countsLeftWithLeftLeds() > proximitySensors.countsRightWithRightLeds()) {
      //Turn anti-clockwise at target acquisition speed
      hackathonMotors.turnLeftWAngle(MOTOR_ACQ_TURN_SPEED,75);
    //Target to the right
    }else{
      //Turn clockwise at target acquisition speed
      hackathonMotors.turnRightWAngle(MOTOR_ACQ_TURN_SPEED,75);
    }
    ledGreen(0);
	}
  return targetInFront;
}

void attack(){
  ledRed(1);
      lcd.gotoXY(0,0);
    lcd.print("ATTACK");
  proximitySensors.read();
  bool check_front_sensors = proximitySensors.countsFrontWithLeftLeds() > PROX_FRONT_SENSORS_SCAN_THRESHOLD || proximitySensors.countsFrontWithRightLeds() > PROX_FRONT_SENSORS_SCAN_THRESHOLD;
	hackathonMotors.moveForward(MOTOR_FORWARD_MAX_SPEED);
  while(!check_for_border() && check_front_sensors){
    if(proximitySensors.countsFrontWithLeftLeds() > proximitySensors.countsFrontWithRightLeds()){
        hackathonMotors.driftLeft(MOTOR_FORWARD_MAX_SPEED);
    }else if(proximitySensors.countsFrontWithLeftLeds() < proximitySensors.countsFrontWithRightLeds()){
        hackathonMotors.driftRight(MOTOR_FORWARD_MAX_SPEED);
    }else{
      hackathonMotors.moveForward(MOTOR_FORWARD_MAX_SPEED);
    }
    proximitySensors.read();
    check_front_sensors = proximitySensors.countsFrontWithLeftLeds() > PROX_FRONT_SENSORS_SCAN_THRESHOLD || proximitySensors.countsFrontWithRightLeds() > PROX_FRONT_SENSORS_SCAN_THRESHOLD;
  }
  ledRed(0);
  if(check_for_border()){
    on_border_detect();
  }
}

//setup
void setup() {
  hackathonMotors.init();
  lineSensors.initThreeSensors();
  proximitySensors.initThreeSensors();
  initiateCountdown();
  lcd.clear();
}

//loop
void loop(){
    lineSensors.read(lineSensorValues);
    lcd.gotoXY(0, 0);
    lcd.print("L:");
    lcd.print(lineSensorValues[0]);
    lcd.gotoXY(0, 1);
    lcd.print("C:");
    lcd.print(lineSensorValues[1]);

     while(lineSensorValues[1] > LINE_SENSORS_THRESHOLD){
     lineSensors.read(lineSensorValues);
     hackathonMotors.moveForward(MOTOR_FORWARD_SPEED);
   }
   if(lineSensorValues[0] > LINE_SENSORS_THRESHOLD){
     while (!(lineSensorValues[1] > LINE_SENSORS_THRESHOLD))
     {
        lineSensors.read(lineSensorValues);
       hackathonMotors.turnLeftWAngle(MOTOR_TURN_SPEED,BORDER_DETECT_MIN_TURN_ANGLE);
     }
    }else if(lineSensorValues[2] > LINE_SENSORS_THRESHOLD){
      while (!(lineSensorValues[1] > LINE_SENSORS_THRESHOLD))
     {
        lineSensors.read(lineSensorValues);
       hackathonMotors.turnRightWAngle(MOTOR_TURN_SPEED,BORDER_DETECT_MIN_TURN_ANGLE);
     }
    }

}
  
  

