#include "main.h"
#include <string>
#include <utility>


// runs initialization code. This occurs as soon as the program is started.
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "i dont wanna die");

	set_all_brake_modes(drive_left_group, pros::E_MOTOR_BRAKE_BRAKE);
	set_all_brake_modes(drive_right_group, pros::E_MOTOR_BRAKE_BRAKE);	
}


// runs while the robot is in the disabled state
void disabled() {


}

// runs after initialize() and before autonomous
void competition_initialize() {
	imu.reset(true);
	pros::c::adi_encoder_reset(left_drive_encoder);
	pros::c::adi_encoder_reset(right_drive_encoder);

}


// runs the user autonomous code
void autonomous() {

	enum Code {
		AUTON1,
		AUTON2,
		SKILLS,
		TEST	
	};

	Code code_number = TEST;

	// alliance
	if (code_number == AUTON1) {
		// score on wall stake
		motor_seconds(conveyor, 0.1, -127);
		turn_to(302, -70, 0);
		motor_seconds(conveyor, 0.7, 127);
		motor_seconds(conveyor, 0.1, -127);

		// pick up second ring
		turn_to(345, 90, -45);
		turn_to(180, -20, 80);
		move_conveyor(true, true);
		move_intake(true, true);
		move_inches(8, 30);
		pros::delay(700);
		turn_to(145, false);
		move_intake(false, false);
		move_conveyor(false, true);

		// back up into moving goal 
		move_inches(-22, 100);
		move_inches(-11, 50);
		move_clamp(true);
		pros::delay(100);

		// score ring on stake
		move_conveyor(true, true);
		pros::delay(750);

		// go to last ring
		turn_to(5, false);
		move_intake(true, true);
		move_inches(10, 40);

		// score last ring
		pros::delay(1200);
		move_intake(false, true);

		// touch ladder
		turn_to(20, true);
		move_conveyor(false, true);
		wheels_speed(-70, -70);	
		pros::delay(1200);
		wheels_speed(0, 0);






// alec is a hero and deserves uppies

	}

	// alliance 2
	else if (code_number == AUTON2) {
		// score on wall stake
		motor_seconds(conveyor, 0.1, -127);
		turn_to(58, 0, -70);
		motor_seconds(conveyor, 0.7, 127);
		motor_seconds(conveyor, 0.1, -127);

		// pick up second ring
		turn_to(15, -45, 90);
		turn_to(180, 80, -20);
		move_conveyor(true, true);
		move_intake(true, true);
		move_inches(10, 50);
		pros::delay(300);
		move_inches(8, 30);
		pros::delay(400);

		turn_to(215, true);
		move_intake(false, false);
		move_conveyor(false, true);

		// back up into moving goal 
		move_inches(-22, 100);
		move_inches(-11, 50);
		move_clamp(true);
		pros::delay(100);

		// score ring on stake
		move_conveyor(true, true);
		pros::delay(750);

		// go to last ring
		move_inches(5, 50);
		turn_to(355, true);
		move_intake(true, true);
		move_inches(10, 40);

		// score last ring
		pros::delay(1200);
		move_intake(false, true);

		// touch ladder
		turn_to(340, false);
		move_conveyor(false, true);
		move_seconds(1.2, -70, -70);

	}

	else if (code_number == SKILLS) {
		// score on wall stake
		motor_seconds(conveyor, 0.05, -127);
		turn_to(302, -70, 0);
		motor_seconds(conveyor, 0.7, 127);
		motor_seconds(conveyor, 0.1, -127);

		// get 1st ring
		move_inches(10, 60, 10, 90);
		move_intake_and_conveyor(true);
		move_inches(23, 90, 23, 60);

		// get stake
		move_intake_and_conveyor(false);
		turn_to(290, 80, false);
		move_inches(15, -80);
		move_clamp(true);

		//get 2nd ring
		turn_to(310, true);
		move_intake_and_conveyor(true);
		move_inches(24, 90);
		pros::delay(300);


		// get 3rd ring
		turn_to(307, false);
		move_inches(15, 110);
		move_inches(10, 60);

		pros::delay(300);

		// get 4th/5th ring
		move_inches(-4, 100);
		turn_to(68, 35, -70);
		move_inches(20, 100);
		move_inches(20, 50);

		// get 6th ring
		move_inches(-26, 100);
		move_inches(10, 30, 30, 80);
		move_inches(-2, 100);


		// drop stake
		turn_to(240, false);
		move_seconds(0.5, -127, -127);
		move_clamp(false);
		move_seconds(0.5, -127, -127);
		move_clamp(false);
		move_seconds(0.3, 127, 127);


		move_intake_and_conveyor(false);

	}

	else if (code_number == TEST) {
	
		pros::delay(3000);
		motor_seconds(conveyor, 0.1, -127);
		turn_to(302, -70, 0);
		motor_seconds(conveyor, 0.7, 127);
		motor_seconds(conveyor, 0.1, -127);

		// pick up second ring
		turn_to(345, 90, -45);
		turn_to(180, -20, 80);
		move_conveyor(true, true);
		move_intake(true, true);
		move_inches(10, 30);
		pros::delay(500);
		turn_to(145, false);
		move_intake(false, false);
		move_conveyor(false, true);

		// back up into moving goal 
		move_inches(-24, 100);
		move_inches(-11, 50);
		move_clamp(true);
		pros::delay(100);

		// score ring on stake
		move_conveyor(true, true);
		pros::delay(750);

		// go to last ring
		turn_to(5, false);
		move_intake(true, true);
		move_inches(10, 40);

		// score last ring
		pros::delay(1200);
		move_intake(false, true);

		// touch ladder
		turn_to(20, true);
		move_conveyor(false, true);
		wheels_speed(-70, -70);	
		pros::delay(1200);
		wheels_speed(0, 0);

	}
}

// runs the user driver code
void opcontrol() {
	while (true) {
		drive_wheels();
		drive_clamp();
		drive_intake();
		drive_lift();

		score_preload();

		pros::delay(10);
	}
}