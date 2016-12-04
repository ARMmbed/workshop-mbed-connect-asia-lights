#include "mbed.h"                 // Load the mbed libraries
#include "ChainableLED.h"         // Driver for the LED
#include "accelerometer.h"        // Driver for the accelerometer

Accelerometer accelerometer;      // Declare the accelerometer
ChainableLED rgbLed(D2, D3, 1);   // Declare the LED (it's chainable!)

Timeout pirTimeout;               // Timer we're using to disable the light after X seconds

// YOUR CODE HERE

int main() {

  printf("Hello world\r\n");

  // set the color to RED at startup, so we can verify that wiring is correct
  rgbLed.setColorRGB(0, 0xff, 0x0, 0x0);

  accelerometer.start();
  accelerometer.change(&pir_rise);

  while (1) {
    wait_ms(500);
  }

}
