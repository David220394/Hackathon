#ifndef HACKATHONIRSENSORS_H
#define HACKATHONIRSENSORS_H

#include <inttypes.h>
 
class HackathonIRSensors {
    private:
        uint16_t lineSensorsThreshold;
        uint8_t frontProxSensorsScanThreshold;
        uint8_t sideProxSensorsScanThreshold;
        uint8_t targetAcquisitionSensibility;

        unsigned int lineSensorValues[3];
    
    public:
        HackathonIRSensors(uint16_t lineSensorsThreshold, uint8_t frontProxSensorsScanThreshold, uint8_t sideProxSensorsScanThreshold, uint8_t targetAcquisitionSensibility);

        void init();
        
        void readLineSensors();

        bool borderDetectedByLeftLineSensor();
        bool borderDetectedByCentreLineSensor();
        bool borderDetectedByRightLineSensor();

        void readProximitySensors();
        
        bool targetLocatedWithFrontLeftProxSensor();
        bool targetLocatedWithFrontRightProxSensor();
        
        bool targetLocatedWithSideLeftProxSensor();
        bool targetLocatedWithSideRightProxSensor();

        bool targetAcquired();

        unsigned int lineSensorLeftCount;
        unsigned int lineSensorCentreCount;
        unsigned int lineSensorRightCount;

        uint8_t proxSensorsFrontLeftCount;
        uint8_t proxSensorsFrontRightCount;
        uint8_t proxSensorsSideLeftCount;
        uint8_t proxSensorsSideRightCount;
};
 
#endif