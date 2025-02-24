#include "main.h"
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
#include <math.h>


// wheels
pros::Motor left_front(-18, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::degrees);
pros::Motor left_middle(5, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::degrees);
pros::Motor left_back(-7, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
pros::Motor right_front(9, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
pros::Motor right_middle(-10, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
pros::Motor right_back(8, pros::v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
std::vector<pros::Motor> drive_left_group = {left_front,left_middle,left_back};
std::vector<pros::Motor> drive_right_group = {right_front,right_middle,right_back};

// motors
pros::Motor intake(-20,pros::v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
pros::Motor lady_brown(1, pros::v5::MotorGears::red, pros::v5::MotorEncoderUnits::deg);

// pistons
pros::adi::DigitalOut clamp('F');
pros::adi::DigitalOut ejector('A');
pros::adi::DigitalOut doinker('E');

// inertia sensor
pros::Imu imu(12);

// optical shaft encoder
pros::adi::Encoder left_drive_encoder('H', 'G', false);
pros::adi::Encoder right_drive_encoder('B', 'C', false);

// optical sensor
pros::Optical optical_sensor(14);

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);



// global variables
const double PI = 3.1415926;

const double DRIVE_TRAIN_GEAR_RATIO = 3.0 / 4.0;
const double WHEEL_RADIUS = 1.625;
const double CHASSIS_WIDTH = 12;

Color ejectColor = BLUE;
Color side = RED;

pros::vision_signature_s_t RED_SIG = pros::Vision::signature_from_utility(1, 8543, 11685, 10114, -729, 1, -364, 3.000, 0);
pros::vision_signature_s_t BLUE_SIG = pros::Vision::signature_from_utility(2, -4287, -3641, -3964, 8191, 10083, 9137, 3.000, 0);



// helper functions

/**
Sets the speed of all motors in a specified motor group
*/
void group_speed(std::vector<pros::Motor> group, int speed) {
  for (pros::Motor motor : group)
    motor.move(speed);
}

/**
Sets the speed of each side of the chassis
*/
void wheels_speed(int left_speed, int right_speed) {
  group_speed(drive_left_group, left_speed);
  group_speed(drive_right_group, right_speed);
}

/**
Turns a specified piston on or off
*/
void activate_piston(pros::adi::DigitalOut piston, bool on) {
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
Gets the inches that an encoder has registered that the chassis has moved
*/
double degrees_to_drive_inches(pros::adi::Encoder encoder) {
  return degrees_to_inches(encoder.get_value(), WHEEL_RADIUS);
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
    pros::Task::delay(10);
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
    motor.move(-speed);
  else if (pros::c::motor_get_position(motor.get_port()) < rotation - margin)
    motor.move(speed);
  else
    motor.move(0);
}


/**
Gets motor average motor position of a specified motor group
*/
int average_motor_position(std::vector<pros::Motor> group) {
  int position = 0;
  for (pros::Motor motor : group) {
    position += motor.get_position();
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

pros::Task* back_off_task = nullptr;

pros::controller_digital_e_t intake_button = DIGITAL_R2;
pros::controller_digital_e_t extake_button = DIGITAL_R1;
void drive_intake() {
  static int timer = 0;
  if (controller.get_digital(intake_button)) {
    intake.move(127);

    if (intake.get_actual_velocity() < 20) {
      timer += 1;
    }

    if (timer >= 200) {
      back_off_task = new pros::Task(back_off);
      timer = 0;
    }
  }

  else if (controller.get_digital(extake_button)) {
    intake.move(-127);
    timer = 0;
  }
  else {
    intake.move(0);
    timer = 0;
  }
}

void back_off() {
  move_intake(true);
  pros::Task::delay(200);
  move_intake(false);

  back_off_task->remove();
  delete back_off_task;
  back_off_task = nullptr;
}

pros::controller_digital_e_t brown_up = DIGITAL_L1;
pros::controller_digital_e_t brown_down = DIGITAL_DOWN;
void drive_lady_brown() {
  if (controller.get_digital(brown_up) && lady_brown.get_position() < 165) {
    if (lady_brown.get_position() < 50) {
      lady_brown.move(35);
    }
    else {
      lady_brown.move(70);
    }
  }
  else if (controller.get_digital(brown_down)) {
    if (lady_brown.get_position() < 50) {
      lady_brown.move(-35);
    }
    else {
      lady_brown.move(-70);
    }
  }
  else if (lady_brown.get_position() > 150) {
    lady_brown.move(-15);
  }
  else {
    lady_brown.move(10);
  }
 /* if (controller.get_digital(brown_up) && lady_brown.get_position() < 165) {
    lady_brown.move(60);
  }
  else if (controller.get_digital(brown_down)) {
    lady_brown.move(-60);
  }
  else if (lady_brown.get_position() < 130 && lady_brown.get_position() > 55) {
    lady_brown.move(-20);
  }
  else if (lady_brown.get_position() > 150) {
    lady_brown.move(-15);
  }
  else {
    lady_brown.move(15);
  }*/

 /* static int position = 0;
  if (controller.get_digital_new_press(brown_up) && position < 2) {
    position += 1;
  }
  else if (controller.get_digital(brown_down) && position > 0) {
    position -= 1;
  }

  static int target = 0;
  if (position == 2) {
    target = 90;
  }
  else if (position == 1) {
    target = 15;
  }
  else {
    target = 0;
  }

  if (lady_brown.get_position() < target + 5) {  
    lady_brown.move(60);
  }
  else if (lady_brown.get_position() > target - 5) {
    lady_brown.move(-60);
  }
  else {
    lady_brown.move(0);
  }*/


 
}

pros::controller_digital_e_t clamp_button = DIGITAL_B;
pros::controller_digital_e_t alt_clamp_button = DIGITAL_L2;
void drive_clamp() {
  static bool on = false;
  if (controller.get_digital_new_press(clamp_button) || controller.get_digital_new_press(alt_clamp_button)) {
    on = !on;
    activate_piston(clamp, on);
  }
}

pros::controller_digital_e_t doinker_button = DIGITAL_UP;
void drive_doinker() {
  static bool on = false;
  if (controller.get_digital_new_press(doinker_button)) {
    on = !on;
    activate_piston(doinker, on);
  }
}



// auton functions
 
/**
Moves chassis a specified amount of inches at a specified speed
 */
void move_inches(double inches, int speed) {
  left_drive_encoder.reset();
  right_drive_encoder.reset();


  wheels_speed(speed * sign(inches), speed * sign(inches));

  while (fabs(degrees_to_drive_inches(left_drive_encoder)) < fabs(inches)) {  
    pros::Task::delay(10);
  }

  wheels_speed(0, 0);
}

void move_inches(double inches, int left_speed, int right_speed){
  left_drive_encoder.reset();


  wheels_speed(left_speed, right_speed);

  while (fabs(degrees_to_drive_inches(left_drive_encoder)) < fabs(inches)) {  
    pros::Task::delay(10);
  }
  wheels_speed(0, 0);
}



void move_inches_c(double left_inches, double right_inches, int max_speed) {
  left_drive_encoder.reset();
  right_drive_encoder.reset();

  double speed_modifier;

  if (right_inches >= left_inches) {
    speed_modifier = (double) left_inches / right_inches;
    wheels_speed(speed_modifier * max_speed, max_speed);
  }
  else if (right_inches < left_inches) {
    speed_modifier = (double) right_inches / left_inches;
    wheels_speed(max_speed, speed_modifier * max_speed);
  }

  while (fabs(degrees_to_drive_inches(right_drive_encoder)) < fabs(right_inches)) {
    pros::Task::delay(20);
  }
  wheels_speed(0, 0);

}


/**
Moves the left and right of chassis at a specified speed for a specified amount seconds.
*/
void move_seconds(float seconds, int left, int right) {
  wheels_speed(left, right);

  pros::Task::delay(seconds * 1000);
  wheels_speed(0, 0);
}

/**
Turns the chassis towards the nearest object then stores the change in the imu value
*/
int compensate() {
  static int margin = 15;
  int yaw = imu.get_heading();
  bool inMiddle = false;
  while (!inMiddle) {
    if (vision_sensor.get_by_sig(0, 1).x_middle_coord < VISION_FOV_WIDTH / 2 - margin) {
      wheels_speed(-25, 25);
    }
    else if (vision_sensor.get_by_sig(0, 1).x_middle_coord > VISION_FOV_WIDTH / 2 + margin) {
      wheels_speed(25, -25);
    }
    else {
      wheels_speed(0, 0);
      yaw += imu.get_heading();
      inMiddle = true;
    }

    pros::Task::delay(10);
  }

  return yaw;
}

/**
Turns the chassis in a direction at a specified speed for a specified amount of seconds.
*/
void turn_seconds(float seconds, int speed, bool right) {
  if (right)
    wheels_speed(-speed, speed);
  else 
    wheels_speed(speed, -speed);

  pros::Task::delay(seconds * 1000);
  wheels_speed(0, 0);
}

/**
Tries to hold the drivetrain in place.
*/
void hold_drivetrain(bool left_side, bool right_side, float seconds) {
  double timer = 0;
  int speed = 30;
  double margin = 0;

  left_drive_encoder.reset();
  right_drive_encoder.reset();
  
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
    pros::Task::delay(10);
    timer += 10;
  }
}

/**
Turns the chassis until it reaches a specified degree using the imu.
This will probably turn in the direction that will result in the least amount of turning
*/
void reset_angle() {
  static int margin = 3;
  static int speed = 60;

  double start_heading = imu.get_heading();

  if (imu.get_heading() > 180) {
    wheels_speed(speed, -speed);
  }
  else {
    wheels_speed(-speed, speed);
  }

  while (imu.get_heading() > margin && imu.get_heading() < 360 - margin) {
    pros::Task::delay(10);
  }

  if (imu.get_heading() > 180) {
    wheels_speed(-speed, speed);
  }
  else {
    wheels_speed(speed, -speed);
  }

  while (imu.get_heading() > 1 && imu.get_heading() < 360 - 1) {
    pros::Task::delay(10);
  }

  wheels_speed(0, 0);
}

/**
Turns the chassis to an absoulte degree with a specified direction
 */
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
    pros::Task::delay(10);
  }
  if (!right) {
    wheels_speed(speed / 2, -speed / 2);
  }
  else {
    wheels_speed(-speed / 2, speed / 2);
  }

  while (imu.get_heading() > degrees + 1 || imu.get_heading() < degrees - 1) {
    pros::Task::delay(10);
  }

  wheels_speed(0, 0);
}

/**
Turns the chassis to an absoulte degree with a specified direction at a specified speed
 */
void turn_to(int degrees, int speed, bool right) {
  static int margin = 3;
  if (right) {
    wheels_speed(speed, -speed);
  }
  else {
    wheels_speed(-speed, speed);
  }
  while (imu.get_heading() > degrees + margin || imu.get_heading() < degrees - margin) {
    pros::Task::delay(10);
  }
  wheels_speed(0, 0);
}

/**
Turns the chassis to an absoulte degree with a specified direction at individual left and right speed
 */
void turn_to(int degrees, int left_speed, int right_speed) {
  static int margin = 3;
  wheels_speed(left_speed, right_speed);
  
  while (imu.get_heading() > degrees + margin || imu.get_heading() < degrees - margin) {
    pros::Task::delay(10);
  }
  wheels_speed(0, 0);
}

void fix_angle(int degrees) {
  int margin = 5;
  if (imu.get_heading() > degrees + margin) {
    wheels_speed(-40, 40);
  }
  else if (imu.get_heading() < degrees - margin) {
    wheels_speed(40, 40);
  }

  while (imu.get_heading() > degrees + margin || imu.get_heading() < degrees - margin) {
    pros::Task::delay(20);
  }
  wheels_speed(0, 0);
}

/**
Moves the left and right of the chassis at a specified speed until it reaches a certain absolute degree is reached.
*/
void move_until_degrees(int left_speed, int right_speed, int degrees) {
  if (degrees < imu.get_heading()) {
    wheels_speed(left_speed, right_speed);
    while (imu.get_heading() > degrees) 
      pros::Task::delay(10);
  }
  else {
    wheels_speed(left_speed, right_speed);
    while (imu.get_heading() < degrees) 
      pros::Task::delay(10);
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
      intake.move(127);
    }
    else {
      intake.move(-127);
    }
  }
  else {
    intake.move(0);
  }
}

/**
Turns intake on or off
*/
void move_intake(bool on) {
  if (on) {
    intake.move(127);
  }
  else {
    intake.move(0);
  }
}

/**
This is a combination of move intake and move conveyor
*/
void move_intake_and_conveyor(bool on) {
  move_intake(on, true);
}

/**
Opens or closes the clamp
*/
void move_clamp(bool on) {
  activate_piston(clamp, on);
}

/**
Moves any specified motor for a specifed amount of seconds
 */
void motor_seconds(pros::Motor motor, float seconds, int speed) {
  motor.move(speed);
  pros::Task::delay(seconds * 1000);
  motor.move(0);
}


/**
Turns the ejector task on or off
*/
bool ejectorEnabled = false;
void enableEjector(bool on) {
  ejectorEnabled = on;
}


/**
Sets the color that the ejector likes, sets the refresh rate of the optical sensor, and lights the led
 */

void setColor(Color color) {
  optical_sensor.set_integration_time(10);
  optical_sensor.set_led_pwm(50);

  side = color;
  if (color == RED) {

    ejectColor = BLUE;
  }
  else {
    ejectColor = RED;
  }
}

/**
Returns the color that the optical sensor sees and is close enough to the sensor
 */
Color get_seen_color() {
  if (optical_sensor.get_proximity() > 200) {
    if (optical_sensor.get_rgb().red > 150) {
      // controller.set_text(0, 0, "RED")
      return RED;
    }
    else if (optical_sensor.get_rgb().blue > 150){
      return BLUE;
    }
  }

  return NONE;
}

/**
This is what is passed into the task
 */
void ejectorTask(){
	while (true) {
    if (get_seen_color() == ejectColor) {
      activate_piston(ejector, true);
      pros::Task::delay(300);
    }
    else {
      activate_piston(ejector, false);
    }
    pros::Task::delay(10);
  }
}


void taskTest() {
  while (true){
    pros::Task::delay(30);
  }
}
