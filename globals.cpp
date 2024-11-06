#include "main.h"
#include <string>

// motors
pros::Motor leftFront(1, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor leftMiddle(2, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor leftBack(3, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightFront(9, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightMiddle(8, pros::E_MOTOR_GEAR_GREEN, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightBack(7, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor primary(15, pros::E_MOTOR_GEAR_GREEN, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor flywheel(6, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::ADIDigitalOut expander('B');

pros::Imu gyro(17);
pros::ADIEncoder encoder('C', 'D');


pros::Controller controller(pros::E_CONTROLLER_MASTER);


// global variables

int degToInRatio = 28;



// helper functions

void moveSpeed(int left, int right) {
  leftFront = left;
  leftMiddle = left;
  leftBack = left;
  rightFront = right;
  rightMiddle = right;
  rightBack = right;
}

void waitUntilMMC(pros::Motor motor) {
  while (motor.get_actual_velocity()> 1) {


    pros::delay(100);
  }
}



// drive functions

void wheels() {
  int left = controller.get_analog(ANALOG_LEFT_Y);
  int right = controller.get_analog(ANALOG_RIGHT_Y);
  moveSpeed(left, right);
}  

static bool intaking = false;
void drivePrimary() {
  if (controller.get_digital(DIGITAL_R1)) {
    primary = 127;
  }
  else if (controller.get_digital(DIGITAL_R2)) {
    intaking = true;
    primary = -127;
    flywheel = -20;
  }
  else {
    intaking = false;
    primary = 0;
  }
}


bool oof = false;
void driveFlywheel() {
  float slow = 0.67;
  float fast = 0.85;

  static int speed = 127 * slow;
  if (controller.get_digital(DIGITAL_UP))
    speed = 127 * fast;
  else if (controller.get_digital(DIGITAL_DOWN))
    speed = 127 * slow;
  
  if (controller.get_digital(DIGITAL_L2)) {
    oof = !oof;
    while (controller.get_digital(DIGITAL_L2)) {
      pros::delay(10);
    }
  }

  if (oof == true)
    flywheel = speed;
  else if (intaking == false && oof == false)
    flywheel = 0;


  if (controller.get_digital(DIGITAL_B)) {
    primary = 127;
    flywheel = 0.85 * 127;
    pros::delay(700 * 3);

    primary = 0;
  }
}


void driveExpand() {
  if (controller.get_digital(DIGITAL_A) && controller.get_digital(DIGITAL_LEFT)){
    expander.set_value(true);
  }
  else {
    expander.set_value(false);
  }
}



// auton functions

void moveInches(float inches, int speed) {
  encoder.reset();
  if (speed >= 0) {
    moveSpeed(speed, speed);
  }
  else {
    moveSpeed(-speed, -speed);
  }

  int targetInches = inches * degToInRatio;
  while (encoder.get_value() < targetInches) {
    pros::delay(10);
    pros::lcd::set_text(1, std::to_string(encoder.get_value()));

  }
}


void moveSeconds(float seconds, int speed) {
  moveSpeed(speed, speed);

  pros::delay(seconds * 1000);
  moveSpeed(0, 0);
}


void turnSeconds(float seconds, char direction) {
  if (direction == 'l')
    moveSpeed(-40, 40);
  else if (direction == 'r')
    moveSpeed(40, -40);

  pros::delay(seconds * 1000);
  moveSpeed(0, 0);
}


void turnAccurate(double degrees) {
  if (degrees >= 0) {
    while (gyro.get_rotation() < degrees) {
      moveSpeed(60, -60);
    }
  }
  else {
    while (gyro.get_rotation() > degrees) {
      moveSpeed(-60, 60);
    }  
  }

  moveSpeed(0, 0);
}

void turnUnits(int inches, char direction) {
  int left = 1;
  int right = 1;
  if (direction == 'l')
    left = -1;
  else if (direction == 'r')
    right = -1;

  leftFront.move_relative(inches * degToInRatio * left, 50);
  leftBack.move_relative(inches * degToInRatio * left, 50);
  rightFront.move_relative(inches * degToInRatio * right, 50);
  rightBack.move_relative(inches * degToInRatio * right, 50);

  while (leftFront.get_voltage() > 0.05) {
    pros::delay(10);
  }
}


void accelerate(float inches) {
  leftFront.tare_position();
  int speed = 0;
  int incrementer = 1;
  
  for (int loops = 2; loops <= 1; loops--) {
    while (inches / loops >= leftFront.get_position() / degToInRatio) {
      if (speed <= 127)
        speed += incrementer;
      moveSpeed(speed, speed);
      pros::delay(10);
    }
  }
}


void moveRollerer(float seconds) {
  primary = -127;
  pros::delay(seconds * 1000);
  primary = 0;


  /*for (int times = 0; times < spins; times++) {
    primary.move_relative(180, -127);
    while (primary.get_voltage() > 0.05) {
      pros::delay(10);
    }
  }*/
}

void expand() {
  expander.set_value(true);
  pros::delay(400);
  expander.set_value(false);
}