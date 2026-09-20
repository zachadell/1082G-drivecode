# 1082G VEX Robotics Drive Code

![VEX Robotics](https://img.shields.io/badge/VEX%20Robotics-1082G-blue)
![PROS](https://img.shields.io/badge/PROS-V5-red)
![LemLib](https://img.shields.io/badge/LemLib-Odometry%20%26%20PID-orange)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue)

The official robot control code for **VEX Robotics Team 1082G**.

This repository contains the robot's driver-control system, autonomous routines, LemLib drivetrain and odometry configuration, PID controllers, and LVGL Brain display.

---

## Features

* PROS V5 control system
* LemLib drivetrain and autonomous control
* 6-motor drivetrain
* Lift mechanism
* Wrist mechanism
* Claw mechanism
* Horizontal and vertical tracking-wheel odometry
* Inertial sensor heading
* Lateral and angular PID controllers
* LVGL Brain display
* Arcade drive
* Autonomous scoring routine
* Motor brake control

---

## Hardware

### Drivetrain

| Component   | Ports         | Gearset |
| ----------- | ------------- | ------- |
| Left Drive  | -11, -12, -13 | Blue    |
| Right Drive | 1, 2, 3       | Blue    |

The drivetrain uses **2.75-inch Omni wheels** with a track width of approximately **12.8125 inches**.

```cpp
pros::MotorGroup left ({-11, -12, -13}, pros::MotorGearset::blue);
pros::MotorGroup right ({1, 2, 3}, pros::MotorGearset::blue);
```

### Mechanisms

| Mechanism | Port | Gearset |
| --------- | ---: | ------- |
| Lift      |    4 | Blue    |
| Wrist     |   14 | Green   |
| Claw      |    5 | Green   |

### Sensors

| Sensor                     | Port | Purpose                    |
| -------------------------- | ---: | -------------------------- |
| Horizontal Rotation Sensor |    9 | Horizontal odometry        |
| Vertical Rotation Sensor   |   10 | Vertical odometry          |
| IMU                        |    8 | Heading / angular odometry |

---

## Software

### PROS

This project uses **PROS** as the VEX V5 development framework.

PROS provides the motor, controller, sensor, task, and competition APIs used by the robot.

### LemLib

**LemLib** is used for:

* Drivetrain control
* Odometry
* Point-to-point movement
* Turning
* PID control
* Autonomous movement

---

# PID Configuration

The robot uses separate PID controllers for **lateral movement** and **angular movement**.

## Lateral PID

```cpp
lemlib::ControllerSettings lateral_controller(
    10,   // kP
    0,    // kI
    3,    // kD
    3,    // anti-windup
    1,    // small error range
    100,  // small error timeout
    3,    // large error range
    500,  // large error timeout
    20    // slew
);
```

## Angular PID

```cpp
lemlib::ControllerSettings angular_controller(
    2,   // kP
    0,   // kI
    10,  // kD
    3,
    1,
    100,
    3,
    500,
    0
);
```

### PID Values

| Controller | kP | kI | kD |
| ---------- | -: | -: | -: |
| Lateral    | 10 |  0 |  3 |
| Angular    |  2 |  0 | 10 |

### What the Terms Do

* **P — Proportional:** Responds to the current distance from the target.
* **I — Integral:** Responds to accumulated error over time.
* **D — Derivative:** Responds to how quickly the error is changing and helps reduce overshoot and oscillation.

The robot currently uses `kI = 0` for both controllers. P and D were used to achieve the desired autonomous response.

---

# Odometry

The robot uses two tracking wheels and an IMU for odometry.

```cpp
lemlib::TrackingWheel hori_track(
    &hori,
    lemlib::Omniwheel::NEW_2,
    -1
);

lemlib::TrackingWheel vert_track(
    &vert,
    lemlib::Omniwheel::NEW_2,
    1.375
);
```

The sensors are configured using:

```cpp
lemlib::OdomSensors sensors(
    &vert_track,
    nullptr,
    &hori_track,
    nullptr,
    &imu
);
```

The autonomous starts with the following robot pose:

```cpp
chassis.setPose(63.5, 7.75, 180);
```

The pose consists of:

* **X position**
* **Y position**
* **Heading**

---

# Driver Control

The robot uses **arcade drive**.

### Driving

| Controller Input | Function          |
| ---------------- | ----------------- |
| Left Stick Y     | Forward / Reverse |
| Right Stick X    | Turning           |

```cpp
int dir = master.get_analog(ANALOG_LEFT_Y);
int turn = -master.get_analog(ANALOG_RIGHT_X);

left.move(dir - turn);
right.move(dir + turn);
```

### Lift

| Button | Action    |
| ------ | --------- |
| R1     | Lift Up   |
| R2     | Lift Down |

### Wrist

| Button | Action     |
| ------ | ---------- |
| L1     | Wrist Up   |
| L2     | Wrist Down |

### Claw

The claw uses the **A button** as a toggle.

Each press changes between the two claw states.

```cpp
if (master.get_digital_new_press(DIGITAL_A)) {
    claw.brake();

    if (clawUp) {
        claw.move(-127);
        clawUp = false;
    } else {
        claw.move(127);
        clawUp = true;
    }
}
```

---

# Autonomous

The autonomous routine uses LemLib to control the robot's position and heading.

The autonomous sequence includes:

1. Moving the lift and wrist.
2. Driving to the first position.
3. Operating the claw.
4. Moving toward the scoring area.
5. Turning toward field positions.
6. Collecting/scoring game elements.
7. Moving to additional field positions.
8. Completing the final scoring sequence.

Example point-to-point movement:

```cpp
chassis.moveToPoint(
    52,
    15,
    2000,
    {.forwards = false}
);
```

Example turning command:

```cpp
chassis.turnToPoint(
    74,
    6,
    1000
);
```

The autonomous can also limit its maximum speed for more controlled movements:

```cpp
chassis.moveToPoint(
    54,
    44,
    1000,
    {.forwards = false, .maxSpeed = 50}
);
```

---

# LVGL Display

The V5 Brain uses **LVGL** to display robot information.

The display includes:

* Robot graphics
* X position
* Y position
* Heading
* Button interaction

The robot's position is updated every 20 milliseconds:

```cpp
lv_label_set_text(
    text,
    std::format(
        "X: {:.2f}\nY: {:.2f}\nTheta: {:.2f}",
        chassis.getPose().x,
        chassis.getPose().y,
        chassis.getPose().theta
    ).c_str()
);
```

---

# Project Structure

```text
1082G/
├── include/
│   └── main.h
├── src/
│   └── main.cpp
├── assets/
│   └── ...
├── Makefile
├── project.pros
└── README.md
```

The main robot code is located in:

```text
src/main.cpp
```

---

# Tuning

PID should be tuned through repeated testing on the actual robot.

### Lateral Movement

If the robot:

* **Undershoots:** Increase kP.
* **Overshoots:** Reduce kP or increase kD.
* **Oscillates:** Increase kD and/or reduce kP.
* **Moves too slowly:** Increase kP.

### Turning

If the robot:

* **Does not turn aggressively enough:** Increase kP.
* **Overshoots the target angle:** Increase kD and/or reduce kP.
* **Oscillates:** Increase kD.

Before tuning PID, verify that the drivetrain and odometry measurements are accurate.

---

# Important Measurements

The current drivetrain configuration is:

```text
Track Width: 12.8125 in
Wheel Diameter: 2.75 in
Drive RPM: 450
```

Tracking-wheel configuration:

```text
Horizontal Tracking Wheel:
Diameter: 2 in
Offset: -1 in

Vertical Tracking Wheel:
Diameter: 2 in
Offset: 1.375 in
```

If the robot's physical configuration changes, these values may need to be updated and PID/odometry may need to be retuned.

---

# Team

## VEX Robotics Team 1082G

Robot control software developed for VEX V5 competition.

---

## License

This code is maintained by **VEX Robotics Team 1082G**.

For team use and educational purposes.
