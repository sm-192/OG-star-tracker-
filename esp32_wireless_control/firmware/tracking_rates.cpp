#include "tracking_rates.h"
#include "config.h"
#include "consts.h"

// Calculate tracking rate from period in milliseconds
// Formula: Timer_reload_value = TIMER_APB_CLK_FREQ / steps_per_second
// Where steps_per_second = (steps_for_full_revolution_at_64_microstepping) / day_period_in_seconds
uint64_t TrackingRates::calculateTrackingRate(uint64_t period_ms)
{
    // Convert STEPS_PER_TRACKER_FULL_REV_INT from 256 microstepping to 64 microstepping
    uint64_t steps_per_revolution_microstep =
        STEPS_PER_TRACKER_FULL_REV_INT / (MAX_MICROSTEPS / TRACKER_MOTOR_MICROSTEPPING);

    // Calculate steps per second for the given period
    // steps_per_second = steps_per_revolution / period_in_seconds
    // period_in_seconds = period_ms / 1000
    double steps_per_second =
        (double) steps_per_revolution_microstep / ((double) period_ms / 1000.0);

    // Timer reload value = timer_frequency / steps_per_second
    uint64_t timer_reload_value = (uint64_t) (TIMER_APB_CLK_FREQ / steps_per_second);

    return timer_reload_value;
}

// Constructor - calculates all tracking rates
TrackingRates::TrackingRates()
{
    sidereal_rate = calculateTrackingRate(SIDERAL_DAY_MS);
    solar_rate = calculateTrackingRate(SOLAR_DAY_MS);
    lunar_rate = calculateTrackingRate(LUNAR_DAY_MS);
    setRate(TRACKING_RATE); // Set initial rate based on TRACKING_RATE
}

void TrackingRates::setRate(TrackingRateType type)
{
    switch (type)
    {
        case TRACKING_SIDEREAL:
            current_rate = sidereal_rate;
            break;
        case TRACKING_SOLAR:
            current_rate = solar_rate;
            break;
        case TRACKING_LUNAR:
            current_rate = lunar_rate;
            break;
    }
};

// Global instance of tracking rates
TrackingRates trackingRates;
