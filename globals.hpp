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

extern pros::Motor intake;
extern pros::Motor conveyor;

extern pros::Motor lady_brown;

// pistons
extern pros::adi::DigitalOut clamp;
extern pros::adi::DigitalOut ejector;
extern pros::adi::DigitalOut doinker;
extern pros::adi::DigitalOut descorer;

// inertia sensor
extern pros::IMU imu;

// rotation sensors
extern pros::Rotation left_rotation;
extern pros::Rotation right_rotation;

extern pros::adi::DigitalIn color_switch;

// optical shaft encoders
extern pros::adi::Encoder left_drive_encoder;
extern pros::adi::Encoder right_drive_encoder;

// lady brown degree detector
extern pros::adi::AnalogIn potentiometer;
 
extern pros::Vision vision_sensor;

extern pros::Optical optical_sensor;

// main controller
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

double degrees_to_inches(double degrees, double radius);

double degrees_to_drive_inches(pros::adi::Encoder encoder);

double rotation_sensor_inches(pros::Rotation sensor, double time_step);


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
void drive_conveyor();
void drive_lady_brown();
void drive_doinker();
void drive_descorer();

void score_preload(); 

// auton functions
void move_inches(double inches, int speed);
void move_inches(double inches, int left_speed, int right_speed);
void move_inches_long(double inches, int speed);
void move_seconds(float seconds, int left, int right);
void hold_drivetrain(bool left_side, bool right_side, float seconds);
int compensate();
void turn_seconds(float seconds, int speed, bool right);
void reset_angle();
void reset_angle(int speed);

void turn_to(int degrees, bool right);
void turn_to(int degrees);
void turn_to(int degrees, int speed, bool right);
void turn_to(int degrees, int left_speed, int right_speed);
void fix_angle();
void move_until_degrees(int left_speed, int right_speed, int degrees);
void move_intake(bool on, bool forward);
void move_intake(bool on);
void move_only_intake(bool on);
void move_conveyor(bool on, bool forward);
void move_lady_brown(bool on, bool forward);
void move_clamp(bool on);
void move_doinker(bool on);
void move_blue_ring_descorer(bool on);
void motor_seconds(pros::Motor motor, float seconds, int speed);

void enableEjector(bool on);

void lift_setup_task();

void intake_until_red();

void back_off();

void toggle_lift_task(bool on);

void color_sort();
int get_robot_angle();

void start_completable_task(pros::Task task);

void taskTest();

void set_color(Color color);
Color get_seen_color();


void turn(int angle);

void start_timer();
double delta_time();

void set_robot_angle(int angle);


//poop
