#include "main.h"
#include "lemlib/api.hpp"
#include "liblvgl/display/lv_display.h"
#include "liblvgl/widgets/image/lv_image.h"
#include "liblvgl/widgets/label/lv_label.h"

pros::MotorGroup left ({-11, -12, -13}, pros::MotorGearset::blue);
pros::MotorGroup right ({1, 2, 3}, pros::MotorGearset::blue);

pros::Motor lift (4, pros::MotorGearset::blue);
pros::Motor wrist (14, pros::MotorGearset::green);
pros::Motor claw (5, pros::MotorGearset::green);

lemlib::Drivetrain drivetrain(&left, &right, 12.8125, lemlib::Omniwheel::NEW_275, 450, 2);

pros::Rotation hori (9);
pros::Rotation vert (10);
pros::Imu imu (8);

lemlib::TrackingWheel hori_track(&hori, lemlib::Omniwheel::NEW_2, -1);
lemlib::TrackingWheel vert_track(&vert, lemlib::Omniwheel::NEW_2, 1.375);

lemlib::OdomSensors sensors(&vert_track, nullptr, &hori_track, nullptr, &imu);

lemlib::ControllerSettings lateral_controller(10, 0, 3, 3, 1, 100, 3, 500, 20);
lemlib::ControllerSettings angular_controller(2, 0, 10, 3, 1, 100, 3, 500, 0);

lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors);

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;

	LV_IMAGE_DECLARE(shayaan);
	lv_obj_t* img = lv_image_create(lv_screen_active());
	lv_image_set_src(img, &shayaan);
	lv_image_set_scale(img, 270);
	lv_obj_align(img, LV_ALIGN_CENTER, 0, -20);
	
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

	LV_IMAGE_DECLARE(sillysmol);
	lv_obj_t* logo = lv_image_create(lv_screen_active());
	lv_image_set_src(logo, &sillysmol);
	lv_image_set_scale(logo, 40);
	lv_obj_align(logo, LV_ALIGN_CENTER, 139, -20);

	lift.set_brake_mode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
	wrist.set_brake_mode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);

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
	wrist.move(127);
	lift.move(127);
	pros::delay(300);
	wrist.brake();
	lift.brake();
	chassis.moveToPoint(52, 15, 2000, {.forwards = false});
	chassis.waitUntilDone();
	claw.move(-127);
	pros::delay(300);
	chassis.moveToPoint(74, 24, 3000);
	chassis.turnToPoint(74, 6, 1000);
	pros::delay(100);
	chassis.moveToPoint(74, 6, 1000);
	chassis.moveToPoint(74, 10, 1000, {.forwards = false});
	chassis.moveToPoint(74, 6, 1000);
	chassis.waitUntilDone();
	lift.move(127);
	chassis.moveToPoint(74, 35, 2000, {.forwards = false, .maxSpeed = 75});
	chassis.waitUntilDone();
	pros::delay(100);
	claw.brake();
	chassis.moveToPoint(74, 41, 3000, {.forwards = false, .maxSpeed = 50});
	lift.brake();
	pros::delay(100);
	claw.move(127);
	lift.move(-127);
	pros::delay(750);
	claw.brake();
	lift.brake();
	chassis.moveToPoint(54, 28, 2000, {.forwards = false});
	chassis.waitUntilDone();
	lift.move(-127);
	pros::delay(100);
	claw.move(-127);
	lift.brake();
	pros::delay(300);
	claw.brake();
	lift.move(127);
	chassis.moveToPoint(60, 36, 2000);
	chassis.waitUntilDone();
	chassis.moveToPose(54, 42, 45, 1000, {.forwards = false});
	chassis.waitUntilDone();
	lift.brake();

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

	

	while (true) {	

		// Arcade control scheme
		int dir = master.get_analog(ANALOG_LEFT_Y);
		int turn = -master.get_analog(ANALOG_RIGHT_X);
		left.move(dir - turn);
		right.move(dir + turn);
		pros::delay(20);

		// Lift
		if (master.get_digital(DIGITAL_R1) && !master.get_digital(DIGITAL_R2)) {
			lift.move(127);
		} else if (master.get_digital(DIGITAL_R2) && !master.get_digital(DIGITAL_R1)) {
			lift.move(-127);
		} else {
			lift.brake();
		}

		// wrist
		if (master.get_digital(DIGITAL_L1) && !master.get_digital(DIGITAL_L2)) {
			wrist.move(127);
		} else if (master.get_digital(DIGITAL_L2) && !master.get_digital(DIGITAL_L1)) {
			wrist.move(-127);
		} else {
			wrist.brake();
		}
	}
}