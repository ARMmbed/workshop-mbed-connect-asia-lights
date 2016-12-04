#ifndef ACCEL_H
#define ACCEL_H

#include "fxos8700cq.h"           // Driver for the accelerometer
#include "mbed_events.h"          // Event queue library

FXOS8700CQ accel(PTE25, PTE24, FXOS8700CQ_SLAVE_ADDR1); // FRDM-K64F accelerometer
InterruptIn accel_interrupt_pin(PTC13);  // Wait for an interrupt to happen...

EventQueue queue(32 * EVENTS_EVENT_SIZE);
rtos::Thread eventThread;

class Accelerometer {
public:
    Accelerometer() : isFirstEvent(true) {}

    void start() {
        eventThread.start(this, &Accelerometer::start_thread);

        accel_interrupt_pin.fall(this, &Accelerometer::interrupt);
        accel_interrupt_pin.mode(PullUp);

        accel.config_int();      // enabled interrupts from accelerometer
        accel.config_feature();  // turn on motion detection
        accel.enable();          // enable accelerometer
    }

    void start_thread() {
        while (1) {
            queue.dispatch();
        }
    }

    void interrupt() {
        accel.clear_int();

        if (isFirstEvent) {
            isFirstEvent = false;
        }
        else {
            queue.call(callback);
        }
    }

    void change(void (*cb)(void)) {
        callback = cb;
    }

private:
    void (*callback)(void);
    bool isFirstEvent;
};

#endif