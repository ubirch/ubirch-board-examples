/*! Blink - simplistic app. Blinks LED, reverses blink pattern on button press.
 * Written by Matthias Krauss
 * Copyright 2016 Press Every Key UG
 */


#include <stdint.h>
#include <stdbool.h>
#include <board.h>

/*! stupid busy wait - don't use in production! */
void delay(uint32_t delay) {
    for (volatile uint32_t i=0; i<delay; i++) {}
}

int main(void) {
    BOARD_Init();

    bool inverse = false;
    bool buttonWasOn = false;
    while (1) {
        bool buttonIsOn = Button_Read();
        if (buttonIsOn && !buttonWasOn) {   //button going down
            inverse = !inverse;
        }
        buttonWasOn = buttonIsOn;
        LED_Write(!inverse);
        delay(1000000);
        LED_Write(inverse);
        delay(5000000);
    }
}
