#include "main.h"
#include <string>
#include <utility>




// runs initialization code. This occurs as soon as the program is started.
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "cut the board");

	set_all_brake_modes(drive_left_group, pros::E_MOTOR_BRAKE_BRAKE);
	set_all_brake_modes(drive_right_group, pros::E_MOTOR_BRAKE_BRAKE);	

	setColor(RED);


}


// runs while the robot is in the disabled state
void disabled() {

}

// runs after initialize() and before autonomous
void competition_initialize() {
	imu.reset(true);
	left_drive_encoder.reset();
 	right_drive_encoder.reset();
}


enum Code {
	REDPLUS,
	REDNEG,
	BLUEPLUS,
	BLUENEG,
	SKILLS,
	TEST	
};



// runs the user autonomous code
void autonomous() {

	pros::Task task(ejectorTask, "Fun time");


	Code code_number = SKILLS;

	// alliance
	if (code_number == REDPLUS) {
		// score on wall stake
		motor_seconds(conveyor, 0.1, -127);
		turn_to(297, -60, 0);
		move_seconds(0.15, 0, 50);

		motor_seconds(conveyor, 0.7, 127);
		motor_seconds(conveyor, 0.1, -127);

		// pick up second ring
		turn_to(345, 90, -45);
		turn_to(290, -20, 90);
		turn_to(210, -40, 60);
		move_intake_and_conveyor(true);
		move_inches(4, 60);
		move_inches(11, 30);
		pros::Task::delay(600);
		move_intake_and_conveyor(false);
		move_inches(-3, 70);


		// go to mobile goal
		turn_to(145, false);
		move_inches(-20, 100);
		move_inches(-10, 30);
		move_clamp(true);
		pros::Task::delay(100);
		move_conveyor(true, true);



		// go to third ring
		turn_to(14, 50, false);
		move_intake(true, true);

		move_inches(12, 90);
		pros::Task::delay(100);

		// go to fourth ring
		move_inches(-7, 80);
		turn_to(10, true);
		move_inches(20, 90);
		turn_to(62, 70);
		move_inches(10, 90);

		// pick up fourth ring wiggle
		move_seconds(0.4, 30, 90);
		move_seconds(0.2, 40, 40);
		move_seconds(0.2, 50, -50);
		move_seconds(0.2, -50, 50);
		move_seconds(0.2, 50, -50);
		//pros::Task::delay(400);

		// touch ladder
		//move_seconds(0.2, 0, -100);
		move_inches(-5, 90);
		turn_to(170, 90, true);
		turn_to(200, 30);
		move_inches(40, 110);
		turn_to(45, true);
		move_intake_and_conveyor(false);
		move_seconds(0.8, -40, -40);

		// alec is a hero and deserves uppies

	}

	// alliance 2
	else if (code_number == BLUEPLUS) {
		// score on wall stake
		motor_seconds(conveyor, 0.1, -127);
		turn_to(63, 0, -60);
		move_seconds(0.15, 50, 0);

		motor_seconds(conveyor, 0.7, 127);
		motor_seconds(conveyor, 0.1, -127);

		// pick up second ring
		turn_to(15, -45, 90);
		turn_to(90, 90, -20);
		turn_to(150, 60, -40);
		move_intake_and_conveyor(true);
		move_inches(6, 60);
		move_inches(9, 30);
		pros::Task::delay(400);
		move_intake_and_conveyor(false);

		// go to mobile goal
		//move_inches(-2, 70);
		turn_to(210, true);
		move_inches(-17, 100);
		move_inches(-10, 30);
		move_clamp(true);
		pros::Task::delay(100);
		move_conveyor(true, true);


		// go to third ring
		turn_to(335, 50, true);
		move_intake(true, true);
		move_inches(14, 90);
		move_inches(8, 30);

		pros::Task::delay(300);

		// go to fourth ring
		move_inches(-4, 80);
		turn_to(340, false);
		move_inches(17, 90);
		turn_to(305, -70, 70);
		move_inches(6, 90);

		// pick up fourth ring wiggle
		move_seconds(0.4, 90, 30);
		move_seconds(0.2, 40, 40);
		move_seconds(0.2, -50, 50);
		move_seconds(0.2, 50, -80);
		pros::Task::delay(400);

		// touch ladder
		move_seconds(0.4, -100, -100);
		//fix_angle(315);
		set_all_brake_modes(drive_left_group, pros::E_MOTOR_BRAKE_COAST);
		set_all_brake_modes(drive_right_group, pros::E_MOTOR_BRAKE_COAST);

		move_inches(-38, 85);
		move_intake_and_conveyor(false);
		//move_seconds(0.8, -40, -40);
	}

	else if (code_number == REDNEG) {
		// get goal
		move_inches(-19, 100);
		move_inches(-14, 50);
		move_clamp(true);

		// get ring 2 and 3
		turn_to(122, true);
		move_intake_and_conveyor(true);
		move_inches(10, 60);
		move_inches(6, 30);
		turn_to(125, false);
		move_inches(-3, 40);
		move_inches(5, 30);
		move_inches(-4, 40);
		move_inches(8, 30);


		pros::Task::delay(300);


		// get ring 4
		move_inches(-3, 70);
		turn_to(45, false);
		move_inches(12, 50);
		//move_inches(20, 70);
		move_seconds(0.8, 60, 80);

		// THE WIGGLE
		move_seconds(0.5, 90, 50);
		move_seconds(0.3, -50, 50);
		move_seconds(0.3, 50, -50);
		move_inches(-5, 127);
		turn_to(185, 80,  true);
		turn_to(205, 25, true);

		//move_clamp(false);
		//move_seconds(0.1, -50, 50);


		//turn_to(40, true);

		// touch ladder
		move_inches(25, 100);

		turn_to(34, true);

		set_all_brake_modes(drive_left_group, pros::E_MOTOR_BRAKE_COAST);
		set_all_brake_modes(drive_right_group, pros::E_MOTOR_BRAKE_COAST);

		move_intake_and_conveyor(false);

		task.suspend();
		activate_piston(ejector, true);

		move_seconds(0.5, -127,-127);

	}

	else if (code_number == BLUENEG) {
		// get goal
		move_inches(-19, 100);
		move_inches(-14, 50);
		move_clamp(true);

		// get ring 2 and 3
		turn_to(240, false);
		move_intake_and_conveyor(true);
		move_inches(8, 60);
		move_inches(4, 30);
		turn_to(235, true);
		move_inches(-3, 40);
		move_inches(6, 30);
		move_inches(-4, 40);
		move_inches(8, 30);


		pros::Task::delay(300);


		// get ring 4
		move_inches(-3, 70);
		turn_to(313, true);
		move_inches(12, 50);
		//move_inches(20, 70);
		move_seconds(0.8, 80, 60);

		// THE WIGGLE
		move_seconds(0.5, 50, 90);
		move_seconds(0.3, 50, -50);
		move_seconds(0.3, -50, 50);
		move_inches(-5, 127);
		turn_to(175, 80,  false);
		turn_to(145, 25, false);

		//move_clamp(false);
		//move_seconds(0.1, -50, 50);


		//turn_to(40, true);

		// touch ladder
		move_inches(25, 100);

		turn_to(308, false);

		set_all_brake_modes(drive_left_group, pros::E_MOTOR_BRAKE_COAST);
		set_all_brake_modes(drive_right_group, pros::E_MOTOR_BRAKE_COAST);

		move_intake_and_conveyor(false);

		task.suspend();
		activate_piston(ejector, true);

		move_seconds(0.3, -127,-127);

	}

	else if (code_number == SKILLS) {
		/*// score on wall stake
		motor_seconds(conveyor, 0.4, 127);
		motor_seconds(conveyor, 0.1, -127);

		// get 1st ring
		turn_to(30, 90, 30);
		move_intake_and_conveyor(true);
		move_inches(22, 80);
		move_inches(10, 50);

		// get stake
		turn_to(7, false);
		move_inches(-11, 80);
		move_intake_and_conveyor(false);
		move_inches(-8, 50);
		move_clamp(true);
	
		// get ring 2
		move_intake_and_conveyor(true);
		turn_to(29, true);
		move_inches(18, 80);
		move_inches(7, 50);
	
		// get ring 3
		turn_to(42, -50, 60);
		move_inches(14, 70);
		move_inches(10, 30);
		pros::Task::delay(200);

		// get ring 4, 5
		move_inches(-9, 80);
		turn_to(165, true);
		move_inches(15, 75);
		pros::Task::delay(300);
		move_inches(10, 50);
		move_seconds(0.2, 50, 50);

		// get ring 6
		move_inches(-16, 75);
		turn_to(110, 0, 90);
		move_inches(4, 70);
		move_seconds(0.3, 50, 50);

		// score stake
		turn_to(2, -60, 60);
		move_inches(-18, 80);
		pros::Task::delay(400);
		move_clamp(false);
		move_intake_and_conveyor(false);

		// go to other side
		move_inches(10, 80);
		turn_to(280, false);
		move_inches(50, 83, 80);

		// get ring 7
		turn_to(315);
		move_intake_and_conveyor(true);
		move_inches(25, 50);
		move_intake_and_conveyor(false);

		// get stake 2
		turn_to(3, true);
		move_inches(-12, 50);*/
		move_clamp(true);

		// get ring 8
		//turn_to(318, false);
		imu.set_heading(315);
		move_intake_and_conveyor(true);
		move_inches(25, 70);

		// get ring 9
		//turn_to(321, 40, -40);
		move_inches(22, 60, 50);

		// get ring 10 and 11
		turn_to(190, -70, 40);
		move_inches(20, 70);
		move_inches(10, 50);
		move_seconds(0.3, 30, 30);

		// get ring 12
		move_inches(-16, 70);
		turn_to(265, 90, 0);
		move_inches(5, 50);


		move_intake_and_conveyor(false);
	}

	else if (code_number == TEST) {
		pros::Task::delay(1000000);
	}

	task.remove();

}

// runs the user driver code
void opcontrol() {

	set_all_brake_modes(drive_left_group, pros::E_MOTOR_BRAKE_BRAKE);
	set_all_brake_modes(drive_right_group, pros::E_MOTOR_BRAKE_BRAKE);

	pros::Task tasks(ejectorTask, "Fun drive");


	while (true) {
		drive_wheels();
		drive_clamp();
		drive_intake();
		drive_lift();

		controller.set_text(1, 1, std::to_string(conveyor.get_position()));

		pros::Task::delay(20);
	}
}
