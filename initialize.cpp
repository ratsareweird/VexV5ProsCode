#include "main.h"
#include <iterator>
#include <string>
#include <utility>




// runs initialization code. This occurs as soon as the program is started.
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "cut the board");

	set_all_brake_modes(drive_left_group, pros::E_MOTOR_BRAKE_BRAKE);
	set_all_brake_modes(drive_right_group, pros::E_MOTOR_BRAKE_BRAKE);	

	lady_brown.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	potentiometer.calibrate();

	set_color(RED);

	pros::Task sort(color_sort);

}


// runs while the robot is in the disabled state
void disabled() {

}

// runs after initialize() and before autonomous
void competition_initialize() {
	imu.reset(true);
	imu.set_data_rate(5);


	left_rotation.reset();
	right_rotation.reset();
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


	Code code_number = REDPLUS;
                                                                                                              
	// alliance
	if (code_number == REDPLUS) {


		// get stake
		move_inches(-33, 90);
		move_inches(-28, 50, 50);
		move_clamp(true);

		// score preload
		move_intake(true);

		// get ring 1
		turn(-70);
		//controller.set_text(1, 1, std::to_string(imu.get_heading()));
		move_inches(20, 90, 90);
		move_inches(20, 30, 30);

		// get ring 2
		move_inches(-0.2, 40, 40);
		turn(25);
		move_inches(44, 80, 65);
		//fix_angle();
		//turn(15);
		move_inches(17, 50, 50);
		turn_to(315, false);
		move_seconds(0.5, 90, 90);
		move_inches(-15, 40, 40);
		move_intake(false);
		move_inches(-30, 90);
		move_lady_brown(true, true);
		turn(180);
		move_lady_brown(false, true);
		



		/*move_inches(-5, 40);
		turn_to(340, 70, 20);
		move_inches(12, 70);
		move_intake(true, false);
		move_seconds(0.3, 20, 70);
		move_intake(true, true);
		move_seconds(0.9, 20, 70);
		pros::delay(1000);

		// touch ladder
		move_inches(-20, 70);
		move_lady_brown(true, true);
		turn_to(130, true);
		move_lady_brown(false, true);
		move_seconds(2, 60, 60);*/

	}

	else if (code_number == REDNEG) {


		// get goal
		move_inches(-15, 70, 73);
		move_inches(-13, 50);
		move_clamp(true);

		// get ring 1
		move_intake(true);
		turn_to(70, true);
		move_inches(10, 70);
		move_inches(5, 30);

		// get ring 2
		turn_to(165, 40, -40);
		move_inches(8, 50);
		pros::delay(300);
		move_inches(-6, 50);
		turn_to(150, -40, 40);
		move_inches(14, 50, 30);
		pros::delay(600);
		move_inches(-10, 50);
		turn_to(30, false);
		move_inches(25, 70);

		turn_to(35, true);
		move_seconds(1, 127 * 0.75, 127 * 0.75);
		pros::delay(800);

		move_inches(-20, 40);
		turn_to(260, false);
		move_intake(false);
		//move_seconds(2, 60, 60);

		// // get goal
		// move_inches(-15, 70);
		// move_inches(-13, 30);
		// move_clamp(true);

		// // get ring 1
		// turn_to(70, true);
		// move_intake(true);

		// move_inches(10, 70);
		// move_inches(10, 30);

		// // get ring 2
		// turn_to(170, 40, -40);
		// move_inches(9, 50);
		// pros::delay(300);
		// move_inches(-5, 50);
		// turn_to(165, -40, 40);
		// move_inches(10, 50, 30);
		// pros::delay(600);
		// move_inches(-10, 50);
		// turn_to(20, false);
		// move_inches(28, 70);

		// turn_to(65, true);
		// move_seconds(1.5, 127, 127);
		// pros::delay(800);





		// move_inches(-20, 70);
		// turn_to(260, false);
		// //move_seconds(2, 60, 60);



	}
	else if (code_number == BLUEPLUS) {

	imu.set_heading(15);

	motor_seconds(lady_brown, 1, 127);
	motor_seconds(lady_brown, 0.75, -127);

	turn_to(70, 0, -60);
	move_inches(-19, 70);
	move_inches(-10, 30);

	move_clamp(true);

	turn_to(180, true);

	move_intake(true);

	move_inches(15, 70);
	turn_to(90, 0, 50);

	move_inches(20, 60);

	turn_to(130, true);

	// 	// score 
	// 	move_inches(-15, 70);
	// 	move_inches(-13, 30);
	// 	move_clamp(true);

	// 	// score preload
	// 	move_intake(true);

	// 	// get ring 1
	// 	turn_to(70, true);
	// 	move_inches(10, 70);
	// 	move_inches(10, 30);

	// 	// get ring 2
	// 	move_inches(-5, 40);
	// 	turn_to(30, 20, 70);
	// 	move_inches(14, 70);
	// 	move_intake(true, false);
	// 	move_seconds(0.8, 70, 20);
	// 	move_intake(true);
	// 	move_seconds(0.8, 70, 20);



	// 	pros::delay(1000);

	// 	// touch ladder
	// 	move_inches(-20, 70);
	// 	move_inches(-10, 30);
	// 	move_lady_brown(true, true);
	// 	turn_to(245, false);
	// 	move_lady_brown(false, true);

	// 	move_seconds(1.5, 50, 50);
	 }

	else if (code_number == BLUENEG) {

		// get goal
		move_inches(-15, 70, 73);
		move_inches(-13, 30);
		move_clamp(true);

		// get ring 1
		move_intake(true);
		turn_to(290, false);
		move_inches(14, 70);
		move_inches(5, 30);

		// get ring 2
		turn_to(195, -40, 40);
		move_inches(8, 50);
		pros::delay(300);
		move_inches(-6, 50);
		turn_to(205, 40, -40);
		move_inches(10, 20, 50);
		pros::delay(600);
		move_inches(-10, 50);
		turn_to(330, true);
		move_inches(25, 70);

		turn_to(325, false);
		move_seconds(1, 127 * 0.75, 127 * 0.75);
		pros::delay(800);

		move_inches(-20, 40);
		turn_to(100, true);
		move_intake(false);
		//move_seconds(2, 60, 60);


	}

	else if (code_number == SKILLS) {
		set_color(RED);

		// score preload on alliance stake
		motor_seconds(conveyor, 0.4, 127);
		motor_seconds(conveyor, 0.1, -127);

		// get 1st ring
		move_intake(true, false);
		turn_to(30, 90, 50);
		move_inches(19, 80);
		move_intake(true);
		move_inches(10, 50);

		// get stake
		turn_to(11, false);
		move_intake(false);
		move_inches(-7, 80);
		move_inches(-14, 40);
		move_clamp(true);
	
		// get ring 2
		move_inches(3, 40);
		move_intake(true);
		turn_to(38, true);
		move_inches(13, 80);
		move_inches(7, 50);
	
		// get ring 3
		turn_to(30, 35, 85);
		move_inches(8, 50);
		move_inches(4, 30);
		move_seconds(0.2, 30, 30);

		pros::delay(300);

		// get ring 4, 5
		move_inches(-12, 80);
		turn_to(145, 20, -60);
		move_inches(19, 75);
		move_inches(12, 50);
		move_seconds(0.8, 50, 50);

		// get ring 6
		move_inches(-22, 75);
		turn_to(110, 0, 90);
		move_inches(4, 70);
		move_seconds(0.3, 50, 50);

		// score stake
		turn_to(10, -60, 60);
		pros::delay(250);
		move_inches(-8, 80);		
		move_seconds(0.6, -50, -50);
		motor_seconds(conveyor, 0.2, -127);
		move_clamp(false);
		move_intake(false);

		// go to other side
		move_inches(7, 60, 40);
		turn_to(75, true);
		move_seconds(0.5, 50, 50);
		move_inches(-70, 70, 72);
		move_inches(-15, 30);

		// get 2nd stake
		move_clamp(true);

		// get 1st ring pt.2
		turn_to(26, false);
		move_intake(true);
		move_inches(17, 60);

		// get 2nd ring pt.2
		turn_to(288, false);
		move_inches(18, 60);

		// get 3rd ring pt.2
		turn_to(315, 70, -20);
		move_inches(13, 50);
		move_seconds(0.6, 30, 30);

		// get 4th and 5th ring pt.2
		move_seconds(0.2, -70, -70);
		turn_to(205, -55, 30);
		move_inches(25, 70);
		move_inches(23, 50);
		move_seconds(0.5, 40, 40);

		// get 6th ring pt.2
		move_inches(-13, 75);
		move_inches(-8, 40);
		turn_to(240, 90, 0);
		move_inches(4, 70);
		move_seconds(0.6, 50, 50);

		// score stake 2
		move_inches(-3, 50);
		turn_to(15, true);
		move_inches(-5, 60);
		move_seconds(1, -50, -50);

		move_intake(false);
		move_clamp(false);

		// go to other side pt.2
		move_inches(1, 40);
		turn_to(30);
		move_inches(19, 60);
		turn_to(4, -20, 20);
		move_inches(44, 70, 72);

		// get ring 1 pt.3
		move_intake(true);
		pros::Task task(intake_until_red);
		move_inches(11, 50);

		// get ring 2 pt.3
		turn_to(73, true);
		move_inches(25, 50);

		// get stake 3
		turn_to(210, true);
		move_inches(-18, 70);
		move_inches(-11, 40);
		move_clamp(true);

		// get ring 3 pt.3
		move_intake(true);
		turn_to(160, false);
		move_inches(24, 70);
		move_inches(4, 40);

		// get ring 4 pt.3
		turn_to(105, false);
		move_inches(19, 50);

		// get ring 5 pt.3
		turn_to(15, false);
		move_inches(12, 50);

		// get ring 6 pt.3 and poop all over the flor
		move_inches(-6, 50);
		turn_to(30, 50, 0);
		move_inches(10, 50, 60);
		move_inches(13, 60, 60);
		move_seconds(0.25, 60, 60);
		move_inches(-10, 60, 60);


		// score stake 3
		turn_to(270, -70, 70);
		move_seconds(0.5, -60, -80);
		move_seconds(0.5, -40, -40);
		move_clamp(false);
		move_conveyor(true, false);
		pros::delay(300);
		move_conveyor(false, false);

		// get 4th stake
		move_inches(15, 50);
		turn_to(114, false);
		move_intake(true, false);
		move_inches(-54, 70, 73);
		move_intake(false);
		turn_to(120, true);
		move_inches(-11, 40);
		move_seconds(0.4, -30, -30);
		move_clamp(true);

		// descore blue
		turn_to(265, true);
		move_blue_ring_descorer(true);
		motor_seconds(conveyor, 2, -127);
		move_blue_ring_descorer(false);

		// get last ring
		move_intake(true);
		move_inches(10, 50);
		move_inches(5, 30);

		pros::delay(500);

		move_inches(6, 40);

		move_conveyor(false, true);

		move_doinker(true);
		turn_to(145, -40, 60);
		move_doinker(false);
		move_seconds(1, -50, -50);
		move_clamp(false);
		move_intake(false);

		// go to hang
		move_inches(15, 70);
		move_lady_brown(true, true);

		turn_to(305, true);
		move_lady_brown(false, true);

		move_inches(-5, 90);

		move_intake(false, false);
		move_seconds(0.4, -90, -90);

		set_all_brake_modes(drive_left_group, pros::E_MOTOR_BRAKE_COAST);
		set_all_brake_modes(drive_right_group, pros::E_MOTOR_BRAKE_COAST);	

		move_intake(true, false);

		move_seconds(0.33, -40, -40);
		move_intake(false);


		move_seconds(0.3, -90, -90);
		move_seconds(0.2, 90, 90);

		set_all_brake_modes(drive_left_group, pros::E_MOTOR_BRAKE_BRAKE);
		set_all_brake_modes(drive_right_group, pros::E_MOTOR_BRAKE_BRAKE);	

		

	}

	else if (code_number == TEST) {
		move_inches(160, 80);
		reset_angle();


	}

}

// runs the user driver code
void opcontrol() {

	set_all_brake_modes(drive_left_group, pros::E_MOTOR_BRAKE_BRAKE);
	set_all_brake_modes(drive_right_group, pros::E_MOTOR_BRAKE_BRAKE);




	while (true) {
		drive_wheels();
		drive_clamp();
		drive_conveyor();
		drive_lady_brown();
		drive_intake();
		drive_doinker();
		drive_descorer();


		pros::lcd::set_text(0, "left " + std::to_string(left_rotation.get_velocity()));
		pros::lcd::set_text(1, "right "+ std::to_string(right_rotation.get_velocity()));
		pros::lcd::set_text(2, "sensor value: "+ std::to_string(color_switch.get_value()));

		pros::delay(20);
	}
}
