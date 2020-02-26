#include "HackathonMotors.h"

#include <inttypes.h>
#include <Wire.h>
#include <Zumo32U4.h>

#include "TurnSensor.h"

Zumo32U4Motors motors;
L3G gyro;

void HackathonMotors::init() {
    delay(500); // Delay to give the user time to remove their finger for gyro calibration 
    
    turnSensorSetup();
    turnSensorReset();
}

void HackathonMotors::stop(){
    motors.setSpeeds(0, 0);
}

void HackathonMotors::moveForward(uint16_t speed){
    motors.setSpeeds(speed, speed);
}
void HackathonMotors::setSpeeds(uint16_t speed1,uint16_t speed2){
    motors.setSpeeds(speed1, speed2);
}

void HackathonMotors::moveBackwards(uint16_t speed){
    motors.setSpeeds(-speed, -speed);
}

void HackathonMotors::driftLeft(uint16_t speed){
    motors.setSpeeds((uint16_t)(speed-30),speed);
}
void HackathonMotors::driftRight(uint16_t speed){
    motors.setSpeeds(speed,(uint16_t)(speed-30));
}

void HackathonMotors::moveForwardWDelay(uint16_t speed, uint16_t delayInMilli){
    moveForward(speed);
    
    delay(delayInMilli);
    
    stop();
}

void HackathonMotors::moveBackwardsWDelay(uint16_t speed, uint16_t delayInMilli){
    moveBackwards(speed);
    
    delay(delayInMilli);
    
    stop();
}

void HackathonMotors::turnLeftWDelay(uint16_t speed, uint16_t delayInMilli){
    turnWDelay(-speed, speed, delayInMilli);
}

void HackathonMotors::turnRightWDelay(uint16_t speed, uint16_t delayInMilli){
    turnWDelay(speed, -speed, delayInMilli);
}

void HackathonMotors::turnLeftWAngle(uint16_t speed, int32_t angleInDegrees) {
    turnWAngle(-speed, speed, angleInDegrees);
}

void HackathonMotors::turnRightWAngle(uint16_t speed, int32_t angleInDegrees) {
    turnWAngle(speed, -speed, angleInDegrees);
}

void HackathonMotors::turnWDelay(int16_t leftSpeed, int16_t rightSpeed, uint16_t delayInMilli){
    motors.setSpeeds(leftSpeed, rightSpeed);
    
    delay(delayInMilli);
    
    stop();
}

void HackathonMotors::turnWAngle(int16_t leftSpeed, int16_t rightSpeed, int32_t angleInDegrees) {
    

    turnSensorReset();

    motors.setSpeeds(leftSpeed, rightSpeed);
    
    do {
        delay(1);
    } while (abs(turnSensorUpdate()) < angleInDegrees);
    
    //stop();
}
