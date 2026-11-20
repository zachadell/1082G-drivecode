#pragma once

#include "main.h"
#include "lemlib/api.hpp"
#include "auton.h"
#include "lift.h"


#ifndef BOTCONFIG
#define BOTCONFIG

extern pros::Controller master;

extern pros::MotorGroup left;
extern pros::MotorGroup right;

extern pros::Motor lift1;
extern pros::Motor lift2;
extern pros::Motor lift3;
extern pros::MotorGroup lift;

extern pros::Motor toggle;

extern pros::Motor claw;
extern pros::Optical clawOptical;

extern lemlib::Drivetrain drivetrain;

extern pros::Rotation hori;
extern pros::Rotation vert;
extern pros::Imu imu;

extern lemlib::TrackingWheel hori_track;
extern lemlib::TrackingWheel vert_track;

extern lemlib::OdomSensors sensors;

extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;

extern lemlib::Chassis chassis;

extern lemlib::PID liftPID;

#endif