#include <Arduino.h>
#include <Joystick.h>

const unsigned int BUTTONS_COUNT = 10u;
const unsigned int HAT_COUNT = 0u;
const bool X_AXIS = true;
const bool Y_AXIS = true;
const bool Z_AXIS = true;
const bool RX_AXIS = true;
const bool RY_AXIS = true;
const bool RZ_AXIS = false;
const bool RUDDER_AXIS = false;
const bool THROTTLE_AXIS = false;
const bool ACCELERATOR_AXIS = false;
const bool BRAKE_AXIS = false;
const bool STEERING_AXIS = false;

const int throttle_min= 125;
const int throttle_max= 875;

const float throttle_deadzone_min = 230;
const float throttle_deadzone_max = 350;
const float throttle_deadzone_value = 258;

const float antenna_deadzone_min = 430;
const float antenna_deadzone_max = 520;
const float antenna_deadzone_value = 514;

const float range_deadzone_min = 430;
const float range_deadzone_max = 550;
const float range_deadzone_value = 514;

const int cursorX_min= 382;
const int cursorX_max= 860;
const int cursorX_deadzone_min= 400;
const int cursorX_deadzone_max= 500;
const int cursorX_deadzone_value= 514;

const int cursorY_min= 330;
const int cursorY_max= 785;
const int cursorY_deadzone_min= 420;
const int cursorY_deadzone_max= 480;
const int cursorY_deadzone_value= 514;

void applyRowValues(uint8_t* values, uint8_t pin);
float getAxisValue(float filteredValue, uint8_t pin, int minAxisValue, int maxAxisValue, int deadZoneMin, int deadZoneMax, int deadZoneValue, float expCoeff);