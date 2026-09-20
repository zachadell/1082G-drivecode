#include "main.h"
#include "lemlib/api.hpp"
#include "liblvgl/display/lv_display.h"
#include "liblvgl/widgets/image/lv_image.h"
#include "liblvgl/widgets/label/lv_label.h"

void runAuton() {
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