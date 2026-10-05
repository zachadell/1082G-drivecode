#include "botconfig.h"
#include "main.h"
#include "lemlib/api.hpp"

pros::MotorGroup left ({-20, -10}, pros::MotorGearset::blue);
pros::MotorGroup right ({11, 1}, pros::MotorGearset::blue);

pros::MotorGroup lift ({-9, 4}, pros::MotorGearset::green);

pros::ADIAnalogOut claw('A');

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