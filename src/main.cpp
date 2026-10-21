#include "main.h"
#include "lemlib/api.hpp"
#include "liblvgl/display/lv_display.h"
#include "liblvgl/widgets/image/lv_image.h"
#include "liblvgl/widgets/label/lv_label.h"
#include "botconfig.h"
#include "auton.h"
#include "lift.h"


void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::clear();
	pros::lcd::register_btn1_cb(on_center_button);

	chassis.calibrate(); 
    chassis.setPose(63.5, 7.75, 180);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);

	lift.set_brake_mode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
	lift1.set_brake_mode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
	lift2.set_brake_mode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);

	lv_obj_t* text = lv_label_create(lv_screen_active());
	lv_obj_align(text, LV_ALIGN_TOP_LEFT, 30, 30);

	pros::Task screen_task([=]() {
        while (true) {
            lv_label_set_text(text, std::format("X: {:.2f}\nY: {:.2f}\nTheta: {:.2f}", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta).c_str());
            pros::delay(20);
        }
    });
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous() {
	liftReset();
	waitForLift();
	liftToHeight(10);
	waitForLift();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	bool clawOpen = false;
	bool buttonPressing = false;
	master.clear();
	master.print(0,0, "Claw: %s", clawOpen ? "Open" : "Closed");

	
	while (true) {	
		// Arcade control scheme
		int dir = -master.get_analog(ANALOG_LEFT_Y);
		int turn = -master.get_analog(ANALOG_RIGHT_X);
		left.move(dir - turn);
		right.move(dir + turn);
		pros::delay(20);

		// Lift
		if (master.get_digital(DIGITAL_L2) && !master.get_digital(DIGITAL_L1)) {
			lift.move(70);
		} else if (master.get_digital(DIGITAL_L1) && !master.get_digital(DIGITAL_L2)) {
			lift.move(-100);
		} else {
			lift.brake();
		}
		
		//claw
		if (master.get_digital(DIGITAL_R2) && !buttonPressing) {
			buttonPressing = true;
			claw.set_value(!clawOpen);
			clawOpen = !clawOpen;
		} else if (!master.get_digital(DIGITAL_R2) && buttonPressing) {
			buttonPressing = false;
		}
		/*
		if (master.get_digital(DIGITAL_R1) && !master.get_digital(DIGITAL_R2)) {
			claw.set_value(true);
		} else if (master.get_digital(DIGITAL_R2) && !master.get_digital(DIGITAL_R1)) {
			claw.set_value(false);
		}
		*/
	}
}