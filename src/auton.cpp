#include "liblvgl/display/lv_display.h"
#include "liblvgl/widgets/image/lv_image.h"
#include "liblvgl/widgets/label/lv_label.h"
#include "auton.h"
#include "botconfig.h"
#include <vector>

ASSET(Skills1f_TXT);

struct Segment {
	std::string name;
	float lookahead;
	int timeout;
	bool forwards;
};



void runRight() {
    toggle.move_relative(300 * 5, 1000);
	pros::delay(500);
	chassis.moveToPoint(0, -30, 500, {.maxSpeed = 70});
}

void runLeft() {
	toggle.move_relative(300 * 5, 1000);
	pros::delay(500);
	chassis.moveToPoint(24, -48, 2000, {.maxSpeed = 70});
	lift.move(127);
	pros::delay(225);
	lift.brake();
	chassis.waitUntilDone();
	/*
	lift.move(-127);
	pros::delay(400);
	lift.brake();
	*/
	claw.set_value(true);
	chassis.moveToPoint(0, -48, 3000, {.forwards = false, .maxSpeed = 50}, false);

	pros::delay(200);


	/*
	chassis.moveToPoint(0, -40, 2000, {.forwards = false}, false);
	lift.move(-127);
	pros::delay(250);
	lift.brake();
	chassis.turnToPoint(24, -24, 1000, {}, false);
	pros::delay(100);
	chassis.moveToPoint(27, -21, 3000, {.maxSpeed = 30, .minSpeed = 15}, false);
	claw.set_value(false);
	pros::delay(100);
	lift.move(127);
	pros::delay(400);
	lift.brake();
	chassis.moveToPoint(24, -48, 1500, {}, false);
	lift.move(-100);
	pros::delay(300);
	lift.brake();
	claw.set_value(true);
	*/
}

void runSkills() {
	pros::delay(100);
	// ------ init -------


	
	toggle.move_relative(360 * 5, 1000);
	pros::delay(500);
	lift.move(127);
	pros::delay(200);
	lift.brake();
	chassis.moveToPoint(24, -48, 1000, {}, false);
	lift.move(-127);
	pros::delay(200);
	lift.brake();
	claw.set_value(true);

	pros::delay(200);

	chassis.moveToPoint(0, -40, 2000, {.forwards = false}, false);
	lift.move(-127);
	pros::delay(250);
	lift.brake();
	chassis.turnToPoint(24, -24, 1000, {}, false);
	pros::delay(100);
	chassis.moveToPoint(27, -21, 3000, {.maxSpeed = 30, .minSpeed = 15}, false);
	claw.set_value(false);
	pros::delay(100);
	lift.move(127);
	pros::delay(400);
	lift.brake();
	chassis.moveToPoint(24, -48, 1500, {}, false);
	lift.move(-100);
	pros::delay(300);
	lift.brake();
	claw.set_value(true);
	
	pros::delay(200);
	
	chassis.moveToPoint(24, -24, 2000, {.forwards = false}, false);
	lift.move(-127);
	pros::delay(300);
	lift.brake();
	chassis.turnToPoint(48, -48, 1000, {}, false);
	chassis.moveToPoint(51, -45, 3000, {.maxSpeed = 30, .minSpeed = 15}, false);
	claw.set_value(false);
	lift.move(127);
	pros::delay(500);
	lift.brake();
	chassis.moveToPoint(24, -48, 2000, {}, false);
	lift.move(-127);
	pros::delay(100);
	lift.brake();
	claw.set_value(true);
	/*
	
	pros::delay(200);
	
	chassis.moveToPoint(60, -45, 2000, {.forwards = false}, false);
	chassis.turnToHeading(180, 1000, {}, false);
	claw.set_value(false);
	chassis.moveToPoint(60, -66, 2000, {}, false);
	lift.move(75);
	pros::delay(100);
	lift.brake();
	chassis.moveToPoint(60, -45, 2000, {.forwards = false}, false);
	chassis.turnToPoint(24, -48, 2000, {}, false);
	lift.move(127);
	pros::delay(600);
	lift.brake();
	chassis.moveToPoint(24, -48, 2000, {}, false);
	pros::delay(200);
	lift.move(-127);
	pros::delay(250);
	lift.brake();
	claw.set_value(true);
	*/

	
}