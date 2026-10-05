#pragma once

#include "main.h"
#include "lemlib/api.hpp"

#ifndef BOTCONFIG
#define BOTCONFIG

extern pros::MotorGroup left;
extern pros::MotorGroup right;

extern pros::MotorGroup lift;
extern pros::ADIAnalogOut claw;

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

#endif