#include "botconfig.h"
#include "main.h"
#include "lemlib/api.hpp"
#include "lift.h"

float Kp = 10;
float liftDeadband = 1;
float pi = 3.14159265358979;
float radtodeg = 180 / pi;

pros::Task* liftTask = nullptr;

void waitForLift() {
    if (liftTask == nullptr) return; 

    while (liftTask->get_state() != pros::E_TASK_STATE_DELETED) {
        pros::delay(20);
    }
}

void liftToHeight(float height) {
    liftTask = new pros::Task{[=] {
        while (true) {
            float target = asin((height - 21.7) / 25.5) * radtodeg;
            float error = (target - lift.get_position());
            lift.move(Kp * error);
            if (abs(error) < liftDeadband) {
                break;
            }
            pros::delay(50);
        }
        lift.brake();
    }};
}

void liftReset() {
    liftTask = new pros::Task{[] {
        lift.move(-127);
        pros::delay(100);
        while (lift.get_torque() < 1.0)
        {
            lift.move(-127);
            pros::delay(50);
        }
        lift.brake();
        lift.set_zero_position(-33.836);
    }};
}