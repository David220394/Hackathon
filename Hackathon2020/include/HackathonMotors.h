#ifndef HACKATHONMOTORS_H
#define HACKATHONMOTORS_H

#include <inttypes.h>
 
class HackathonMotors {
    private:
        void turnWDelay(int16_t leftSpeed, int16_t rightSpeed, uint16_t delayInMilli);
        void turnWAngle(int16_t leftSpeed, int16_t rightSpeed, int32_t angleInDegrees);
    
    public:
        void init();
        
        void stop();

        void moveForward(uint16_t speed);
        void moveBackwards(uint16_t speed);
        
        void moveForwardWDelay(uint16_t speed, uint16_t delayInMilli);
        void moveBackwardsWDelay(uint16_t speed, uint16_t delayInMilli);

        void turnRightWDelay(uint16_t speed, uint16_t delayInMilli);
        void turnLeftWDelay(uint16_t speed, uint16_t delayInMilli);
        void turnRightWAngle(uint16_t speed, int32_t angleInDegrees);
        void turnLeftWAngle(uint16_t speed, int32_t angleInDegrees);
        void driftLeft(uint16_t speed);
        void driftRight(uint16_t speed);
};
 
#endif