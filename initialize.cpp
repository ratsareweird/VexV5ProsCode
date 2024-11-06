#include "main.h"
#include <string>


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "hawt");

	leftFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	leftMiddle.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	leftBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	rightFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	rightMiddle.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	rightBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	primary.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	flywheel.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	gyro.tare_rotation();
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

	// skills

	gyro.tare_rotation();
	pros::delay(2000);
	moveSeconds(0.1, 100);
	moveRollerer(0.3);
	moveSeconds(0.35, -100);
	// pick up
	turnAccurate(65);
	primary = -127;
	moveSeconds(0.8, 80);
	primary = 0;
	moveSeconds(0.4, 40);
	// second roller
	moveRollerer(0.35);
	// to corner
	moveSeconds(1, -40);
	turnAccurate(200);
	moveSeconds(0.1, 127);
	expand();
	moveSeconds(0.3, -90);


	// Alliance

	/*gyro.tare_rotation();
	pros::delay(2000);
	moveSeconds(0.1, 100);
	moveRollerer(0.15);
	moveSeconds(0.3, -100);
	turnAccurate(-100);

	moveSeconds(1.55, 80);
	moveSpeed(0, 0);
	pros::delay(250);

	turnAccurate(-205);
	flywheel = 127 * 0.82;
	pros::delay(2900);
	primary = 127;
	pros::delay(725);
	primary = 0;
	pros::delay(900);
	primary = 725;
	pros::delay(725);
	flywheel = 0;*/






	/*leftFront.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	leftMiddle.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	leftBack.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	rightFront.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	rightMiddle.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	rightBack.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	primary.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	flywheel.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);*/
}

void opcontrol() {


	while (true) {

		wheels();
		drivePrimary();
		driveFlywheel();
		driveExpand();
		pros::lcd::set_text(1, std::to_string(encoder.get_value()));


		pros::delay(10);
	}
}