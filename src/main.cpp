#include "main.h"
#include "lemlib/api.hpp"
#include "liblvgl/display/lv_display.h"
#include "liblvgl/widgets/image/lv_image.h"
#include "liblvgl/widgets/label/lv_label.h"

pros::MotorGroup left ({-11, -12, -13}, pros::MotorGearset::blue);
pros::MotorGroup right ({1, 2, 3}, pros::MotorGearset::blue);

pros::Motor lift (4, pros::MotorGearset::red);
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
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	bool isRightAuto = false;

	if (isRightAuto) {

		wrist.move(127); 														// prepare wrist/lift for scoring preload
		lift.move(127);
		pros::delay(300);
		wrist.brake();
		lift.brake();
		chassis.moveToPoint(63.5, 12, 2000, {.forwards = false, .maxSpeed = 50}); 				// move away from wall
		chassis.moveToPoint(50, 24, 2000, {.forwards = false, .maxSpeed = 50}); 				// move to low goal
		chassis.waitUntilDone();
		claw.move(-127); 														// score preload
		pros::delay(300);
		chassis.moveToPoint(74, 24, 6000, {.maxSpeed = 50});										// move to center
		chassis.turnToPoint(74, 6, 2000, {.maxSpeed = 50});										// look at toggle
		chassis.moveToPoint(74, 6, 2000, {.maxSpeed = 50});										// toggle once (opponents color)
		chassis.moveToPoint(74, 14, 2000, {.forwards = false});					// move back
		chassis.moveToPoint(74, 6, 2000, {.maxSpeed = 50});										// toggle twice (alliance color, +10 pts)
		chassis.waitUntilDone();
		claw.brake();
		chassis.moveToPoint(59, 33, 6000, {.forwards = false, .maxSpeed = 50});					// move close to pin + cup
		chassis.waitUntilDone();
		lift.move(127);															// raise the lift
		claw.move(127);															// prepare claw for grabbing pin
		pros::delay(1000);
		lift.brake();
		chassis.moveToPoint(54, 42, 2000, {.forwards = false, .maxSpeed = 50});					// move claw over pin + cup
		chassis.waitUntilDone();
		lift.move(-127);														// grab pin + cup
		pros::delay(1000);
		lift.brake();
		chassis.turnToPoint(48, 24, 2000, {.forwards = false, .maxSpeed = 50});					// turn to goal
		chassis.waitUntilDone();
		lift.move(127);															// raise lift to required height
		pros::delay(2000);
		lift.brake();
		chassis.moveToPoint(48, 24, 4000, {.forwards = false, .maxSpeed = 50});					// move pin + cup over goal
		chassis.waitUntilDone();
		pros::delay(500);
		claw.brake();
		claw.move(-127);														// score pin + cup (+10 pts)
	} else {
		wrist.move(127); 														// prepare wrist/lift for scoring preload
		pros::delay(300);
		wrist.brake();
		chassis.moveToPoint(96, 24, 3000, {.forwards = false, .maxSpeed = 50}); 				// move to low goal
		chassis.waitUntilDone();
		pros::delay(300);
		left.move(-50);
		right.move(-50);
		pros::delay(100);
		left.brake();
		right.brake();
		claw.move(-50); 														// score preload
		pros::delay(300);
		chassis.moveToPoint(77, 24, 6000, {.maxSpeed = 50});					// move to center
		chassis.turnToHeading(180, 1000);										// look at toggle
		chassis.moveToPoint(70, 0, 1000);										// toggle once (opponents color)
		chassis.moveToPoint(70, 14, 2000, {.forwards = false, .maxSpeed = 50});					// move back
		chassis.moveToPoint(70, 0, 1000);										// toggle twice (alliance color, +10 pts)
		chassis.waitUntilDone();
		claw.brake();
		chassis.moveToPoint(85, 33, 6000, {.forwards = false, .maxSpeed = 50});					// move close to pin + cup
		chassis.waitUntilDone();
		lift.move(127);															// raise the lift
		claw.move(127);															// prepare claw for grabbing pin
		pros::delay(1300);
		lift.brake();
		chassis.moveToPoint(92, 40, 3000, {.forwards = false, .maxSpeed = 50});					// move claw over pin + cup
		chassis.waitUntilDone();
		lift.move(-127);														// grab pin + cup
		pros::delay(1000);
		lift.brake();
		chassis.turnToPoint(96, 24, 2000, {.forwards = false, .maxSpeed = 50});					// turn to goal
		chassis.waitUntilDone();
		lift.move(127);															// raise lift to required height
		pros::delay(2000);
		lift.brake();
		chassis.moveToPoint(96, 24, 2000, {.forwards = false, .maxSpeed = 50});					// move pin + cup over goal
		chassis.waitUntilDone();
		pros::delay(500);
		claw.brake();
		claw.move(-127);														// score pin + cup (+10 pts)
		pros::delay(300);
		chassis.moveToPoint(84, 48, 2000, {.maxSpeed = 50});										// move away from goal
		chassis.waitUntilDone();
		claw.brake();
		claw.move(127);															// prepare claw for grabbing pin + cup
		chassis.turnToPoint(120, 24, 3000, {.forwards = false, .maxSpeed = 50});				// turn towards goal
		chassis.moveToPoint(117, 26, 3000, {.forwards = false, .maxSpeed = 50});				// move claw over pin + cup
		chassis.waitUntilDone();
		pros::delay(200);
		chassis.waitUntilDone();
		lift.move(-127);														// grab pin + cup
		pros::delay(1750);
		lift.brake();
		chassis.turnToPoint(96, 24, 2000, {.forwards = false, .maxSpeed = 50});	// turn to goal
		chassis.waitUntilDone();
		lift.move(127);															// raise lift
		pros::delay(2500);
		chassis.moveToPoint(96, 24, 2500, {.forwards = false, .maxSpeed = 50});	// move claw over goal
		chassis.waitUntilDone();
		lift.brake();
		claw.move(-127);														// score pin + cup (+10 pts)
		chassis.moveToPoint(117, 26, 2000, {.maxSpeed = 50});					// move away from goal
		chassis.turnToPoint(72, 72, 3000, {.maxSpeed = 50});					// turn to mid
		chassis.moveToPoint(72, 72, 2000, {.maxSpeed = 50});					// move to mid
	}
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
	bool clawUp = false;
	claw.move(-127);
	master.clear();
	master.print(0, 0, "Claw: OUT");
	
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
		
		//claw
		if (master.get_digital_new_press(DIGITAL_A)) {
			master.clear();
			claw.brake();
			if (clawUp) {
				claw.move(-127);
				clawUp = false;
				master.print(0, 0, "Claw: OUT");
			} else {
				claw.move(127);
				clawUp = true;
				master.print(0, 0, "Claw: IN");
			}
		}
	}
}