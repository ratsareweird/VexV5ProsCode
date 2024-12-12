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


extern pros::ADIDigitalOut clamp;

extern pros::ADIDigitalOut lift;

extern pros::ADIDigitalOut ejector;



// inertia sensor
extern pros::IMU imu;


// optical shaft encoders
extern pros::c::adi_encoder_t left_drive_encoder;
extern pros::c::adi_encoder_t right_drive_encoder;

// clamp sensor
extern pros::ADIDigitalIn clamp_sensor;
 
// vision sensor
extern pros::Vision vision_sensor;

// controller
extern pros::Controller controller;

// helper functions
void group_speed(std::vector<pros::Motor> group, int speed);
void wheels_speed(int left, int right);

void activate_piston(pros::ADIDigitalOut piston, bool on);

double degrees_to_inches(int degrees, double radius);

double degrees_to_drive_inches(pros::c::adi_encoder_t encoder);

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
void move_inches(double left_inches, double right_inches);
void move_inches(double left_inches, double right_inches, int speed);
void move_inches(double left_inches, double left_speed, double right_inches, double right_speed);
void move_seconds(float seconds, int left, int right);
void hold_drivetrain(bool left_side, bool right_side, float seconds);
int compensate();
void turn_seconds(float seconds, int speed, bool right);
void turn_to(int degrees);
void turn_to(int degrees, bool right);
void turn_to(int degrees, int speed, bool right);
void turn_to(int degrees, int left_speed, int right_speed);
void move_until_degrees(int left_speed, int right_speed, int degrees);
void move_intake_and_conveyor(bool on);
void move_intake(bool on, bool forward);
void move_conveyor(bool on, bool forward);
void move_clamp(bool on);
void motor_seconds(pros::Motor motor, float seconds, int speed);

void enableEjector(bool on);

void ejectorTask();

void ejectorFun();

void taskTest();

void setColor(bool red);
bool getColor();


//poop
