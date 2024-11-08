#include "main.h"
#include <cstdint>
#include <stdexcept>
#include <vector>


// motors

// wheels
pros::Motor left_front(16, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_middle(2, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_back(11, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_front(4, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_middle(15, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_back(14, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
std::vector<pros::Motor> drive_left_group = {left_front,left_middle,left_back};
std::vector<pros::Motor> drive_right_group = {right_front,right_middle,right_back};

// intake
pros::Motor left_intake(20, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_intake(12, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);

// conveyor
pros::Motor conveyor(18, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);

// clamp
pros::ADIDigitalOut clamp('E');

// lift
pros::ADIDigitalOut right_lift('F');


// inertia sensor
pros::Imu imu(17);

// clamp sensor
pros::ADIDigitalIn clamp_sensor('H');

// optical shaft encoder
pros::c::adi_encoder_t left_drive_encoder = pros::c::adi_encoder_init('C', 'D', false);
pros::c::adi_encoder_t right_drive_encoder = pros::c::adi_encoder_init('A', 'B', true);

// vision sensor
pros::Vision vision_sensor(19, pros::E_VISION_ZERO_CENTER);


// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// global variables
const double PI = 3.1415926;

const double DRIVE_TRAIN_GEAR_RATIO = 3.0 / 5.0;
const double WHEEL_RADIUS = 1.625;


// helper functions

/**
Sets the speed of all motors in a specified motor group
*/
void group_speed(std::vector<pros::Motor> group, int speed) {
  for (pros::Motor motor : group)
    motor = speed;
}

/**
Sets the speed of each side of the drivetrain
*/
void wheels_speed(int left_speed, int right_speed) {
  group_speed(drive_left_group, left_speed);
  group_speed(drive_right_group, right_speed);
}

/**
Turns a specified piston on or off
*/
void activate_piston(pros::ADIDigitalOut piston, bool on) {
  if (on)
    piston.set_value(1);
  else 
    piston.set_value(0);
}

/**
Returns sign of a number
*/
int sign(double value) {
  if (value > 0)
    return 1;
  else if (value < 0)
    return -1;
  else
    return 0;
}

/**
Returns inches in terms of degrees turned
*/
double degrees_to_inches(int degrees, double radius) {
  return 2 * PI * radius * (degrees / 360.0);
}

/**
Gets the inches traveled an encoder connected to the drivetrain
*/
double degrees_to_drive_inches(pros::c::adi_encoder_t encoder) {
  return degrees_to_inches(pros::c::adi_encoder_get(encoder), WHEEL_RADIUS);
}

/**
Sets all the brake modes in motor group
*/
void set_all_brake_modes(std::vector<pros::Motor> group, pros::motor_brake_mode_e mode) {
  for (pros::Motor motor : group) 
    pros::c::motor_set_brake_mode(motor.get_port(), mode);
}

/**
Waits until a motor's velocity is zero
*/
void wait_until_motor_move_complete(pros::Motor motor) {
  while (motor.get_actual_velocity() > 0) {
    pros::delay(10);
  }
}

/**
Holds a specified motor group in place. This function should be used in a loop.
*/
void better_hold(int rotation, std::vector<pros::Motor> group) {
  static int margin = 5;
  static int speed = 20;
  
  if (average_motor_position(group) > rotation + margin) 
    group_speed(group, -speed);
  else if (average_motor_position(group) < rotation - margin)
    group_speed(group, speed);
  else
    group_speed(group, 0);
  
}

/**
Holds a specified motor in place. This function should be used in a loop.
*/
void better_hold(int rotation, pros::Motor motor) {
  static int margin = 5;
  static int speed = 20;
  
  if (pros::c::motor_get_position(motor.get_port()) > rotation + margin) 
    motor = -speed;
  else if (pros::c::motor_get_position(motor.get_port()) < rotation - margin)
    motor = speed;
  else
    motor = -0;
}


/**
Gets motor average motor position of a specified motor group
*/
int average_motor_position(std::vector<pros::Motor> group) {
  int position = 0;
  for (pros::Motor motor : group) {
    position += pros::c::motor_get_position(motor.get_port());
  } 
  return position / group.size();
}


// drive functions

pros::controller_analog_e_t drive_left_stick = ANALOG_LEFT_Y;
pros::controller_analog_e_t drive_right_stick = ANALOG_RIGHT_Y;
pros::controller_digital_e_t drive_other_way = DIGITAL_X;
void drive_wheels() {
  static int left_speed;
  static int right_speed;
  
  static bool forward = true;

  if (controller.get_digital_new_press(drive_other_way)) {
    forward = !forward;
  }

  if (forward) {
    left_speed = controller.get_analog(drive_left_stick);
    right_speed = controller.get_analog(drive_right_stick);
  }
  else {
    left_speed = -controller.get_analog(drive_right_stick);
    right_speed = -controller.get_analog(drive_left_stick);
  }

  wheels_speed(left_speed, right_speed);
}  



pros::controller_digital_e_t intake_button = DIGITAL_R2;
pros::controller_digital_e_t extake_button = DIGITAL_R1;
void drive_intake() {
  if (controller.get_digital(intake_button)) {
    left_intake = 127;
    right_intake = 127;
    conveyor = 127;
  }
  else if (controller.get_digital(extake_button)) {
    right_intake = -127;
    left_intake = -127;
    conveyor = -127;
  }
  else {
    left_intake = 0;
    right_intake = 0;
    conveyor = 0;
  }
}


pros::controller_digital_e_t clamp_button = DIGITAL_A;
void drive_clamp() {
  static bool on = false;

  if (controller.get_digital_new_press(clamp_button)) {
    on = !on;
    activate_piston(clamp, on);
  }

  if (clamp_sensor.get_new_press()) {
    on = true;
    activate_piston(clamp, on);
    pros::lcd::set_text(0, "yay");
    
  }  
  pros::lcd::set_text(3, std::to_string(pros::c::adi_digital_get_new_press(2)));

}

pros::controller_digital_e_t lift_button = DIGITAL_B;
void drive_lift(){
  static bool on = false;
  if (controller.get_digital_new_press(lift_button)) {
    on = !on;
    activate_piston(right_lift, on);
  }
}  


pros::controller_digital_e_t sp1 = DIGITAL_L1;
pros::controller_digital_e_t sp2 = DIGITAL_L2;
void score_preload() {
  if (controller.get_digital(sp1) && controller.get_digital_new_press(sp2)) {
    motor_seconds(conveyor, 0.1, -127);
		turn_to(302, -70, 0);
		motor_seconds(conveyor, 0.7, 127);
		motor_seconds(conveyor, 0.1, -127);
  }
}



// auton functions

void move_inches(double inches, int speed) {
  pros::c::adi_encoder_reset(left_drive_encoder);
  pros::c::adi_encoder_reset(right_drive_encoder);

  wheels_speed(speed * sign(inches), speed * sign(inches));

  while (fabs(degrees_to_drive_inches(left_drive_encoder)) < fabs(inches)) {  
    pros::delay(10);
  }

  wheels_speed(0, 0);
}


/**
Moves the robots left and right side a specified amount in inches at an accurate speed.
*/
void move_inches(double left_inches, double right_inches) {
  pros::c::adi_encoder_reset(left_drive_encoder);
  pros::c::adi_encoder_reset(right_drive_encoder);

  wheels_speed(50 * sign(left_inches), 50 * sign(right_inches));

  bool left_goal = false;
  bool right_goal = false;
  while (!left_goal || !right_goal) {
    if (fabs(degrees_to_drive_inches(left_drive_encoder)) > fabs(left_inches)) {
      left_goal = true;
      group_speed(drive_left_group, 0);
    }
    if (fabs(degrees_to_drive_inches(right_drive_encoder)) > fabs(right_inches)) {
      group_speed(drive_right_group, 0);
      right_goal = true;
    }
    
    pros::delay(10);
  }
  wheels_speed(0, 0);
}

/**
Moves the drivetrain's left and right side a specified amount in inches at a specified speed.
*/
void move_inches(double left_inches, double right_inches, int speed) {
  pros::c::adi_encoder_reset(left_drive_encoder);
  pros::c::adi_encoder_reset(right_drive_encoder);

  wheels_speed(speed * sign(left_inches), speed * sign(right_inches));

  bool left_goal = false;
  bool right_goal = false;
  while (!left_goal || !right_goal) {
    if (fabs(degrees_to_drive_inches(left_drive_encoder)) > fabs(left_inches)) {
      left_goal = true;
      group_speed(drive_left_group, 0);
    }
    if (fabs(degrees_to_drive_inches(right_drive_encoder)) > fabs(right_inches)) {
      group_speed(drive_right_group, 0);
      right_goal = true;
    }

    pros::delay(10);
  }
  wheels_speed(0, 0);
}

void move_inches(double left_inches, double left_speed, double right_inches, double right_speed) {
  pros::c::adi_encoder_reset(left_drive_encoder);
  pros::c::adi_encoder_reset(right_drive_encoder);

  wheels_speed(left_speed, right_speed);

  bool left_goal = false;
  bool right_goal = false;
  while (!left_goal || !right_goal) {
    if (fabs(degrees_to_drive_inches(left_drive_encoder)) > fabs(left_inches)) {
      left_goal = true;
      group_speed(drive_left_group, 0);
    }
    if (fabs(degrees_to_drive_inches(right_drive_encoder)) > fabs(right_inches)) {
      right_goal = true;
      group_speed(drive_right_group, 0);
    }
    controller.set_text(1, 1, std::to_string(fabs(degrees_to_drive_inches(left_drive_encoder))));

    pros::delay(10);
  }
  wheels_speed(0, 0);
}

/**
Moves the drivetrain at a specified speeds for a specified amount seconds.
*/
void move_seconds(float seconds, int left, int right) {
  wheels_speed(left, right);

  pros::delay(seconds * 1000);
  wheels_speed(0, 0);
}

/**
Turns the drivetrain towards the nearest object then stores the change in the imu value
*/
int compensate() {
  static int margin = 15;
  int yaw = imu.get_heading();
  bool inMiddle = false;
  while (!inMiddle) {
    if (pros::c::vision_get_by_sig(17, 0, 1).x_middle_coord < VISION_FOV_WIDTH / 2 - margin) {
      wheels_speed(-25, 25);
    }
    else if (pros::c::vision_get_by_sig(17, 0, 1).x_middle_coord > VISION_FOV_WIDTH / 2 + margin) {
      wheels_speed(25, -25);
    }
    else {
      wheels_speed(0, 0);
      yaw += imu.get_heading();
      inMiddle = true;
    }

    pros::delay(10);
  }

  return yaw;
}

/**
Turns the drivetrain in a direction at a specified speed for a specified amount of seconds.
*/
void turn_seconds(float seconds, int speed, bool right) {
  if (right)
    wheels_speed(-speed, speed);
  else 
    wheels_speed(speed, -speed);

  pros::delay(seconds * 1000);
  wheels_speed(0, 0);
}

/**
Tries to hold the drivetrain in place.
*/
void hold_drivetrain(bool left_side, bool right_side, float seconds) {
  double timer = 0;
  int speed = 30;
  double margin = 0;

  pros::c::adi_encoder_reset(left_drive_encoder);
  pros::c::adi_encoder_reset(right_drive_encoder);
  
  while (timer / 1000 < seconds) {
    if (left_side) {
      if (degrees_to_drive_inches(left_drive_encoder) < margin) {
        group_speed(drive_left_group, speed);

      }
      else if (degrees_to_drive_inches(left_drive_encoder) > margin) {
        group_speed(drive_left_group, -speed);
      }
      else {
        group_speed(drive_left_group, 0);
      }
    }
    if (right_side) {
      if (degrees_to_drive_inches(right_drive_encoder) < margin) {
        group_speed(drive_right_group, speed);

      }
      else if (degrees_to_drive_inches(right_drive_encoder) > margin) {
        group_speed(drive_right_group, -speed);
      }
      else {
        group_speed(drive_right_group, 0);
      }
    }
    pros::delay(10);
    timer += 10;
  }
}

/**
Turns the drivetrain until it reaches a specified degree using the imu. The imu is never tared.
*/
void turn_to(int degrees) {
  double startHeading = imu.get_heading();
  
  if (degrees + startHeading < 180) {
    wheels_speed(50, -50);
    pros::c::delay(10);
    imu.set_heading(0);
    while (imu.get_heading() < degrees + startHeading) {
      pros::c::delay(10);
      controller.set_text(1, 1, std::to_string(imu.get_heading()));
    }
  }
  else {
    wheels_speed(-50, 50);
    pros::c::delay(10);
    imu.set_heading(359.9);
    while (imu.get_heading() > degrees + startHeading) {
      pros::c::delay(10);
      controller.set_text(1, 1, std::to_string(imu.get_heading()));
    }
  }
  wheels_speed(0, 0);
  imu.set_heading(imu.get_heading() + startHeading);
  controller.set_text(1, 1, std::to_string(imu.get_heading()));

}
void turn_to(int degrees, bool right) {
  static int margin = 3;
  static int speed = 40;
  if (right) {
    wheels_speed(speed, -speed);
  }
  else {
    wheels_speed(-speed, speed);
  }
  while (imu.get_heading() > degrees + margin || imu.get_heading() < degrees - margin) {
    pros::delay(10);
  }
  wheels_speed(0, 0);
}

void turn_to(int degrees, int speed, bool right) {
  static int margin = 3;
  if (right) {
    wheels_speed(speed, -speed);
  }
  else {
    wheels_speed(-speed, speed);
  }
  while (imu.get_heading() > degrees + margin || imu.get_heading() < degrees - margin) {
    pros::delay(10);
  }
  wheels_speed(0, 0);
}

void turn_to(int degrees, int left_speed, int right_speed) {
  static int margin = 3;
  wheels_speed(left_speed, right_speed);
  
  while (imu.get_heading() > degrees + margin || imu.get_heading() < degrees - margin) {
    pros::delay(10);
  }
  wheels_speed(0, 0);
}

/**
Moves the drivetrain's left and right until it reaches a certain degree.
*/
void move_until_degrees(int left_speed, int right_speed, int degrees) {
  if (degrees < imu.get_heading()) {
    wheels_speed(left_speed, right_speed);
    while (imu.get_heading() > degrees) 
      pros::delay(10);
  }
  else {
    wheels_speed(left_speed, right_speed);
    while (imu.get_heading() < degrees) 
      pros::delay(10);
  }

  wheels_speed(0, 0);
  wheels_speed(0, 0);
}

/**
Turns the intake on or off in a specifed direction.
*/
void move_intake(bool on, bool forward) {
  if (on) {
    if (forward) {
      left_intake = 127;
      right_intake = 127;
    }
    else {
      left_intake = -127;
      right_intake = -127;
    }

  }
  else {
    right_intake = 0;
    left_intake = 0;
  }
}

void move_conveyor(bool on, bool forward) {
  if (on) {
    if (forward) {
      conveyor = 127;    }
    else {
      conveyor = -127;
    }
  }
  else {
    conveyor = 0;
  }
}

void move_intake_and_conveyor(bool on) {
  move_intake(on, true);
  move_conveyor(on, true);
}

void move_clamp(bool on) {
  activate_piston(clamp, on);
}

void motor_seconds(pros::Motor motor, float seconds, int speed) {
  motor = speed;
  pros::delay(seconds * 1000);
  motor = 0;
}