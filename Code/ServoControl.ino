
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Ramp.h> // include library

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
ramp myRamp;
// our servo # counter
uint8_t servonum = 0;

// Servos and their pulse limits.
// {Servo PIN, SERVO MIN LIMIT, SERVO MAX LIMIT, Pulse offset value}
int servo_1[] = {5, 200, 400, 0}; // 1. servo (15kg MG995 - Rotating base)
int servo_2[] = {0, 73, 322, -2}; // 2. servo (20kg DS3218MG)
int servo_3[] = {1, 97, 347, -5}; // 3. servo (25kg RDS3225)
int servo_4[] = {2, 78, 320, -4}; // 4. servo (15kg RDS3115MG)
int servo_5[] = {3, 98, 464, 5}; // 5. servo (15kg MG995 - Wrist)
int servo_6[] = {4, 87, 459, 0}; // 6. servo (15kg MG995 - Gripper)

 
void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  setAngle(servo_2, 90);
  delay(1000);
  setAngle(servo_3, 90);
  delay(1000);
  setAngle(servo_4, 90);
  delay(1000);
  Serial.print("Value start at: ");
  // Setting start value.
  myRamp.go(90);
  Serial.println(myRamp.getValue());        // accessing start value
  Serial.println("Starting interpolation"); // 
  myRamp.go(120, 1000, LINEAR, BACKANDFORTH);   
  delay(10);
}

void goToPos(int servo, int starting_pos, int end_position, int movespeed) {
  if (starting_pos < end_position) {
    for(int i = starting_pos; i < end_position; i++) {
      pwm.setPWM(servo, 0, i);
      delay(movespeed);
    }
  } else {
    for (int i = starting_pos; i > end_position; i--) {
      pwm.setPWM(servo, 0, i);
      delay(movespeed);
    }
  }
}

void setAngle(int servo[], int target_angle) {
  int servo_num = servo[0];
  int MIN_LIMIT = servo[1];
  int MAX_LIMIT = servo[2];
  if (servo_num == 1 || servo_num == 2){
    target_angle = 180 - target_angle;
  }
  long pulselen = myMap(target_angle, 0, 180, MIN_LIMIT, MAX_LIMIT);
  // Adding offset.
  pulselen += servo[3];
  pwm.setPWM(servo_num, 0, pulselen);
}


long myMap(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
}

void loop() {
  //setAngle(servo_2, myRamp.update());
  //setAngle(servo_3, myRamp.update());
  //setAngle(servo_4, myRamp.update()); 
  delay(100);
}
