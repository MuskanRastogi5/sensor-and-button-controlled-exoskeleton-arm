#include <stdio.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/keyboard.h>

#define TIME_STEP 64
#define MAX_VELOCITY 6.28
#define TOTAL_MOTORS 5

double to_radian(int deg) {
  return deg / 180 * 3.14;
}

void display_commands() {
  printf("\n*** Commands ***\n");
  printf("'1': elbow motor, '2' ankle motor, '3' finger1, '4' finger2, '5' middle finger\n");
  printf("'W' : Pull up\n");
  printf("'S' : Pull down\n");
}

void move_motor(WbDeviceTag motor, int key) {
  if(key == 87) {
    wb_motor_set_velocity(motor, MAX_VELOCITY/2);
    wb_motor_set_position(motor, wb_motor_get_max_position(motor));  // full pull
  }
  else if(key == 83) {
    wb_motor_set_velocity(motor, MAX_VELOCITY/2);
    wb_motor_set_position(motor, wb_motor_get_min_position(motor));  // straight down
  }
}

int main(int argc, char **argv) {
  /* necessary to initialize webots stuff */
  wb_robot_init();

   // Initilise motor
   WbDeviceTag elbow = wb_robot_get_device("motor1");
   WbDeviceTag ankle_motor = wb_robot_get_device("ankle_motor");
   WbDeviceTag finger1 = wb_robot_get_device("palm_finger_1_joint");
   WbDeviceTag finger2 = wb_robot_get_device("palm_finger_2_joint");
   WbDeviceTag thumb = wb_robot_get_device("finger_middle_joint_1");

   WbDeviceTag all_motors[TOTAL_MOTORS] = {elbow, ankle_motor, finger1, finger2, thumb};

   for(int i = 0; i < TOTAL_MOTORS; i++) {
     // initialising position of motor
     wb_motor_set_position(all_motors[i], wb_motor_get_target_position(all_motors[i]));
     // initialising velocity of motor
     wb_motor_set_velocity(all_motors[i], 0);
   }

   wb_keyboard_enable(100);
  display_commands();
  WbDeviceTag current_motor = elbow;

  while (wb_robot_step(TIME_STEP) != -1) {

    /* Process sensor data here */
    int key = wb_keyboard_get_key();
    if(key == -1) {
      for(int motor_i = 0; motor_i < TOTAL_MOTORS; motor_i++)
      wb_motor_set_velocity(all_motors[motor_i], 0);
    }
    else {
      if(key >= '1' && key <= '5') {
        current_motor = all_motors[key-'1'];
      }
      move_motor(current_motor, key);
    }
    // printf("%d\n", key);

    // FINGERS
  };

  /* Enter your cleanup code here */

  /* This is necessary to cleanup webots resources */
  wb_keyboard_disable();
  wb_robot_cleanup();

  return 0;
}
