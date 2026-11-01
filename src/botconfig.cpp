#include "botconfig.h"
#include "main.h"
#include "lemlib/api.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::MotorGroup left ({-11, -1}, pros::MotorGearset::blue);
pros::MotorGroup right ({20, 10}, pros::MotorGearset::blue);

pros::MotorGroup lift ({-9, 5}, pros::MotorGearset::green);
pros::Motor lift1(-9, pros::MotorGearset::red);
pros::Motor lift2(5, pros::MotorGearset::red);

pros::Motor toggle(15, pros::MotorGearset::green);

pros::ADIDigitalOut claw('A');
pros::Optical clawOptical(2);

lemlib::Drivetrain drivetrain(&left, &right, 12.8125, lemlib::Omniwheel::NEW_275, 450, 2);

pros::Rotation hori (19);
pros::Rotation vert (-18);
pros::Imu imu (17);

lemlib::TrackingWheel hori_track(&hori, lemlib::Omniwheel::NEW_2, -0.25);
lemlib::TrackingWheel vert_track(&vert, lemlib::Omniwheel::NEW_2, -1.75);

lemlib::OdomSensors sensors(&vert_track, nullptr, &hori_track, nullptr, &imu);

//                                            kP,Ki,KD
lemlib::ControllerSettings lateral_controller(10, 0, 3, 3, 1, 100, 3, 500, 20);

lemlib::ControllerSettings angular_controller(4, .2, 20, 3, 1, 100, 3, 500, 0);

lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors);

lemlib::PID liftPID(10, 0, 3, 2, false);