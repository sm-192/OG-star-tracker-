#ifndef _TRACKING_RATES_H_
#define _TRACKING_RATES_H_ 1

#include "config.h"

#if MOTOR_TRACKING_RATE == TRACKING_RATE_BOARD_V2
#if STEPPER_TYPE == STEPPER_0_9
// gear ratio 101.25, 0.9deg motor, 64 msteps, f_cpu@240MHz, f_isr@40MHz
#define TRACKER_MOTOR_MICROSTEPPING 64
#define STEPS_PER_SECOND_256MICROSTEP 120

enum trackingRateS
{
    TRACKING_SIDEREAL = 664846, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 666667,    // SOLAR (24h)
    TRACKING_LUNAR = 680967,    // LUNAR (24h, 31 min)
};
#else // stepper 1.8 deg
// gear ratio 101.25, 1.8deg motor, 64 msteps, f_cpu@240MHz, f_isr@40MHz
#define TRACKER_MOTOR_MICROSTEPPING 64
#define STEPS_PER_SECOND_256MICROSTEP 60

enum trackingRateS
{
    TRACKING_SIDEREAL = 1329691, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 1333333,    // SOLAR (24h)
    TRACKING_LUNAR = 1361934,    // LUNAR (24h, 31 min)
};
#endif
#else
#error Unknown tracking rate setting
#endif

#endif /* _TRACKING_RATES_H_ */
