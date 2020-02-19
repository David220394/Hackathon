#include "HackathonIRSensors.h"

#include <inttypes.h>
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proximitySensors;

HackathonIRSensors::HackathonIRSensors(uint16_t lineSensorsThreshold, uint8_t frontProxSensorsScanThreshold, uint8_t sideProxSensorsScanThreshold, uint8_t targetAcquisitionSensibility) {
    this->lineSensorsThreshold = lineSensorsThreshold;
    this->frontProxSensorsScanThreshold = frontProxSensorsScanThreshold;
    this->sideProxSensorsScanThreshold = sideProxSensorsScanThreshold;
    this->targetAcquisitionSensibility = targetAcquisitionSensibility;
}

void HackathonIRSensors::init() {
    lineSensors.initThreeSensors();
    proximitySensors.initThreeSensors();
}

void HackathonIRSensors::readLineSensors(){
    lineSensors.read(lineSensorValues);

    lineSensorLeftCount = lineSensorValues[0];
    lineSensorCentreCount = lineSensorValues[1];
    lineSensorRightCount = lineSensorValues[2];
}

bool HackathonIRSensors::borderDetectedByLeftLineSensor(){
    return lineSensorLeftCount < lineSensorsThreshold;
}

bool HackathonIRSensors::borderDetectedByCentreLineSensor(){
    return lineSensorCentreCount < lineSensorsThreshold;
}

bool HackathonIRSensors::borderDetectedByRightLineSensor(){
    return lineSensorRightCount < lineSensorsThreshold;
}

void HackathonIRSensors::readProximitySensors(){
    // proxSensorsFrontLeftCount = 0;
    // proxSensorsFrontRightCount = 0;
    // proxSensorsSideLeftCount = 0;
    // proxSensorsSideRightCount = 0;

    //for(unsigned short int i=0; i<10; i++) {
        proximitySensors.read();

        proxSensorsFrontLeftCount = proximitySensors.countsFrontWithLeftLeds();
        proxSensorsFrontRightCount = proximitySensors.countsFrontWithRightLeds();
        proxSensorsSideLeftCount = proximitySensors.countsLeftWithLeftLeds();
        proxSensorsSideRightCount = proximitySensors.countsRightWithRightLeds();
    //}
}

bool HackathonIRSensors::targetLocatedWithFrontLeftProxSensor() {
    return proxSensorsFrontLeftCount > frontProxSensorsScanThreshold;
}

bool HackathonIRSensors::targetLocatedWithFrontRightProxSensor() {
    return proxSensorsFrontRightCount > frontProxSensorsScanThreshold;
}

bool HackathonIRSensors::targetLocatedWithSideLeftProxSensor() {
    return proxSensorsSideLeftCount > sideProxSensorsScanThreshold;
}

bool HackathonIRSensors::targetLocatedWithSideRightProxSensor() {
    return proxSensorsSideRightCount > sideProxSensorsScanThreshold;
}

bool HackathonIRSensors::targetAcquired() {
    return targetLocatedWithFrontRightProxSensor() && targetLocatedWithFrontLeftProxSensor() && (proxSensorsFrontLeftCount - proxSensorsFrontRightCount == 0);
}