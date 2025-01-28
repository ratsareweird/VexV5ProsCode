#include "api.h"

// motors

// drive train
extern pros::Motor left_front;
extern pros::Motor left_middle;
extern pros::Motor left_back;
extern pros::Motor right_front;
extern pros::Motor right_middle;
extern pros::Motor right_back;
extern std::vector<pros::Motor> drive_left_group;
extern std::vector<pros::Motor> drive_right_group;
 
// intake
extern pros::Motor left_intake;
extern pros::Motor right_intake;

// conveyor
extern pros::Motor conveyor;


extern pros::adi::DigitalOut clamp;

extern pros::adi::DigitalOut lift;

extern pros::adi::DigitalOut ejector;



// inertia sensor
extern pros::IMU imu;


// optical shaft encoders
extern pros::adi::Encoder left_drive_encoder;
extern pros::adi::Encoder right_drive_encoder;

extern pros::adi::Encoder lift_encoder;

// clamp sensor
extern pros::adi::DigitalIn clamp_sensor;
 
// vision sensor
extern pros::Vision vision_sensor;

// optical sensor
extern pros::Optical optical_sensor;

// controller
extern pros::Controller controller;

enum Color {
    RED,
    BLUE,
    NONE
};

// helper functions
void group_speed(std::vector<pros::Motor> group, int speed);
void wheels_speed(int left, int right);

void activate_piston(pros::adi::DigitalOut piston, bool on);

double degrees_to_inches(int degrees, double radius);

double degrees_to_drive_inches(pros::adi::Encoder encoder);

int sign(double value);

void better_hold(int rotation, std::vector<pros::Motor> group);
void better_hold(int rotation, pros::Motor motor);

void wait_until_motor_move_complete(pros::Motor motor);
void set_all_brake_modes(std::vector<pros::Motor> group, pros::motor_brake_mode_e mode);

int average_motor_position(std::vector<pros::Motor> group);
bool isGroupEmpty(std::vector<pros::Motor> group);

// drive functions
void drive_wheels();
void drive_clamp();
void drive_intake();
void drive_lift();

void score_preload(); 

// auton functions
void move_inches(double inches, int speed);
void move_inches(double inches, int left_speed, int right_speed);
void move_inches_c(double left_inches, double right_inches, int max_speed);
void move_seconds(float seconds, int left, int right);
void hold_drivetrain(bool left_side, bool right_side, float seconds);
int compensate();
void turn_seconds(float seconds, int speed, bool right);
void turn_to(int degrees);
void turn_to(int degrees, bool right);
void turn_to(int degrees, int speed, bool right);
void turn_to(int degrees, int left_speed, int right_speed);
void fix_angle(int degrees);
void move_until_degrees(int left_speed, int right_speed, int degrees);
void move_intake_and_conveyor(bool on);
void move_intake(bool on, bool forward);
void move_conveyor(bool on, bool forward);
void move_clamp(bool on);
void motor_seconds(pros::Motor motor, float seconds, int speed);

void enableEjector(bool on);

void ejectorTask();
void lift_setup_task();

void toggle_lift_task(bool on);

void ejectorFun();

void taskTest();

void setColor(Color color);
Color get_seen_color();


//poop
