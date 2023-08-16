#include "main.h"

Joystick_ throttle(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK, BUTTONS_COUNT,HAT_COUNT,
                    X_AXIS,Y_AXIS,Z_AXIS,RX_AXIS,RY_AXIS,RZ_AXIS,RUDDER_AXIS,THROTTLE_AXIS, 
                    ACCELERATOR_AXIS,BRAKE_AXIS,STEERING_AXIS);

float filteredThrottle = 0;
float filteredCursorX = 0;
float filteredCursorY = 0;
float filteredAntenna = 0;
float filteredRange = 0;

uint8_t row2buttons = 0;
uint8_t row3buttons = 0;
uint8_t row4buttons = 0;

void setup() {
    throttle.setZAxisRange(0,1023);
    throttle.setRyAxisRange(0,1023);
    throttle.setRxAxisRange(0,1023);
    throttle.setXAxisRange(0,1023);
    throttle.setYAxisRange(0,1023);
    throttle.begin(false);
    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(6,INPUT);
    pinMode(7,INPUT);
    pinMode(8,INPUT);
    pinMode(9,INPUT);
}

void loop() {
    filteredThrottle = getAxisValue(filteredThrottle, A0, throttle_min, throttle_max, throttle_deadzone_min, throttle_deadzone_max, throttle_deadzone_value, 0.1);
    throttle.setZAxis(filteredThrottle);
    filteredCursorX = getAxisValue(filteredCursorX, A1, cursorX_min, cursorX_max, cursorX_deadzone_min, cursorX_deadzone_max, cursorX_deadzone_value, 0.5);
    throttle.setXAxis(filteredCursorX);
    filteredCursorY = getAxisValue(filteredCursorY, A2, cursorY_min, cursorY_max, cursorY_deadzone_min, cursorY_deadzone_max, cursorY_deadzone_value, 0.5);
    throttle.setYAxis(filteredCursorY);
    filteredAntenna = getAxisValue(filteredAntenna, A10, 0, 1024, antenna_deadzone_min, antenna_deadzone_max, antenna_deadzone_value, 0.1);
    throttle.setRyAxis(filteredAntenna);
    filteredRange = getAxisValue(filteredRange, A3, 0, 1024, range_deadzone_min, range_deadzone_max, range_deadzone_value, 0.1);
    throttle.setRxAxis(filteredRange);
    applyRowValues(&row2buttons, 2);
    applyRowValues(&row3buttons, 3);
    applyRowValues(&row4buttons, 4);
    throttle.sendState();
}

void applyRowValues(uint8_t* values, uint8_t pin){
    uint8_t button1;
    uint8_t button2;
    uint8_t button3;
    uint8_t button4;
    uint8_t local = *values;
    if (pin == 2) {
        digitalWrite(2, LOW);
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        button1 = 1;
        button2 = 2;
        button3 = 3;
        button4 = 4;
    } else if (pin == 3){
        digitalWrite(2, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(4, HIGH);
        button1 = 6;
        button2 = 7;
        button3 = 8;
        button4 = 9;
    } else if (pin == 4){
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        digitalWrite(4, LOW);
        button1 = 5;
        button2 = 0;
        button3 = 254;
    }
    bool c6 = !digitalRead(6);
    bool c7 = !digitalRead(7);
    bool c8;
    bool c9;
    if (button3 != 254) {
        c8 = !digitalRead(8);
        c9 = !digitalRead(9);
    }
    bool pressed = bitRead(local, 0);
    if (c6 && !pressed) 
        throttle.pressButton(button1);
    else if (!c6 && pressed)
        throttle.releaseButton(button1);
    bitWrite(local, 0, c6);

    pressed = bitRead(local, 1);
    if (c7 && !pressed) 
        throttle.pressButton(button2);
    else if (!c7 && pressed)
        throttle.releaseButton(button2);
    bitWrite(local, 1, c7);

    if (button3 == 254) {
        *values = local;
        return;
    }
        
    pressed = bitRead(local,2);
    if (c8 && !pressed) 
        throttle.pressButton(button3);
    else if (!c8 && pressed)
        throttle.releaseButton(button3);
    bitWrite(local, 2, c8);

    pressed = bitRead(local, 3);
    if (c9 && !pressed) 
        throttle.pressButton(button4);
    else if (!c9 && pressed)
        throttle.releaseButton(button4);
    bitWrite(local, 3, c9);
    *values = local;
}

float getAxisValue(float filteredValue, uint8_t pin, int minAxisValue, int maxAxisValue, int deadZoneMin, int deadZoneMax, int deadZoneValue, float expCoeff) {
    int newVal = map(analogRead(pin),minAxisValue,maxAxisValue,0,1025);
    if (newVal<0) newVal=0;
    else if (newVal>1024) newVal = 1024;
    if (newVal>deadZoneMin && newVal<deadZoneMax)
        newVal = deadZoneValue;
    else if (newVal < deadZoneMin)
        newVal = map(newVal,0,deadZoneMin,0,deadZoneValue);
    else if (newVal > deadZoneMax)
        newVal = map(newVal,deadZoneMax,1024,deadZoneValue,1024);
    if (abs(newVal - filteredValue) > 1.5) expCoeff = 0.9;
    filteredValue += (newVal - filteredValue) * expCoeff;
    if (filteredValue<0) filteredValue = 0;
    if (filteredValue>1024) filteredValue=1024;
    return filteredValue;
}

