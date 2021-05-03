/** 
 *  @file geekbot
 *  @brief Recieves commands via serial port and uses them to control the geekbot
 */
#include <Servo.h>

//pin constants
const int LEFT_SERVO_PIN = 3;
const int RIGHT_SERVO_PIN = 5;
const int WRIST_SERVO_PIN = 10;
const int GRIPPER_SERVO_PIN = 9;
const int CAMERA_SERVO_PIN = 11;

//ad0 is left gripper pad
//ad1 is right gripper pad

const int SERVO_LEFT_FORWARD = 1580;
const int SERVO_RIGHT_FORWARD = 1410;
const int SERVO_LEFT_REVERSE = 1410;
const int SERVO_RIGHT_REVERSE = 1580;
const int SERVO_DELAY = 10;

//speed control
int CW_MIN_SPEED = 1400;      //servo pulse in microseconds for slowest clockwise speed
int CW_MAX_SPEED = 1000;      //servo pulse in microseconds for fastest clockwise speed
int CCW_MIN_SPEED = 1600;      //servo pulse in microseconds for slowest counter-clockwise speed
int CCW_MAX_SPEED = 2000;      //servo pulse in microseconds for fastest counter-clockwise speed
const int SERVO_STOP = 1500;         //servo pulse in microseconds for stopped servo


const int WRIST_MAX = 2080;
const int WRIST_MIN = 1620;
const int CAMERA_FORWARD = 1500;

//gripper control
int wristPos = WRIST_MIN;
int gripperPos = 1500;
int cameraPos = CAMERA_FORWARD;


char incomingByte = 0; // for incoming serial data

Servo servoLeft, servoRight, servoWrist, servoGripper, servoCamera;

float servoSpeedLeft = SERVO_STOP;   //left servo speed. Ranges from FULL_SPEED_REVERESE to FULL_SPEED_FORWARD with halfway in between (0) being stopped
float servoSpeedRight = SERVO_STOP;  //right servo speed. Ranges from FULL_SPEED_REVERESE to FULL_SPEED_FORWARD with halfway in between (0) being stopped

void test_camera(void) 
{
  Serial.println("Testing Camera Servo");
  
  cameraPos = 800;
  servoCamera.writeMicroseconds(cameraPos);
  delay(SERVO_DELAY);
        
  cameraPos = 2400;
  servoCamera.writeMicroseconds(cameraPos);
  delay(SERVO_DELAY);
  
  cameraPos = CAMERA_FORWARD;
  servoCamera.writeMicroseconds(cameraPos);
  delay(SERVO_DELAY);  
}

void test_wrist(void)
{
  Serial.println("Testing Wrist Servo");
  
  wristPos = WRIST_MIN;
  servoWrist.writeMicroseconds(wristPos);
  delay(SERVO_DELAY);
  
  wristPos = WRIST_MAX;
  servoWrist.writeMicroseconds(wristPos);
  delay(SERVO_DELAY);
}

void test_gripper(void)
{
  Serial.println("Testing Gripper Servo");
  
  gripperPos = 800;
  servoGripper.writeMicroseconds(gripperPos);
  delay(SERVO_DELAY);
  
  gripperPos = 2400;
  servoGripper.writeMicroseconds(gripperPos);
  delay(SERVO_DELAY);
  
  gripperPos = 1500;
  servoGripper.writeMicroseconds(gripperPos);
  delay(SERVO_DELAY);
}

bool isStopped()
{
  if (servoSpeedLeft == SERVO_STOP && servoSpeedRight == SERVO_STOP)
  {
    return true;
  }
  return false;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  servoLeft.attach(LEFT_SERVO_PIN);
  servoRight.attach(RIGHT_SERVO_PIN);
  servoWrist.attach(WRIST_SERVO_PIN);
  servoGripper.attach(GRIPPER_SERVO_PIN);
  servoCamera.attach(CAMERA_SERVO_PIN);

  servoLeft.writeMicroseconds(servoSpeedLeft);
  servoRight.writeMicroseconds(servoSpeedRight);
  servoWrist.writeMicroseconds(wristPos);
  servoGripper.writeMicroseconds(gripperPos);
  servoCamera.writeMicroseconds(cameraPos);

  while(!Serial);
  Serial.println("Starting Geekbot Test");
  //test_camera();
  //test_wrist();
  //test_gripper();
  Serial.println("Controls");
  Serial.println("w Forward");
  Serial.println("x Reverse");
  Serial.println("a Left");
  Serial.println("d Right");
  Serial.println("c,q Nudge Left");
  Serial.println("e,z Nudge Right");
  Serial.println("s Stop");
  Serial.println("j gripper wide");
  Serial.println("k gripper medium");
  Serial.println("l gripper narrow");
  Serial.println("b camera up");
  Serial.println("g camera center");
  Serial.println("t camera down");
  Serial.println("n gripper up");
  Serial.println("h gripper center");
  Serial.println("y gripper down");
}

void loop() {
  // put your main code here, to run repeatedly
  
  // send data only when you receive data:
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
    incomingByte = Serial.read();

    switch (incomingByte) {
      // wheel speed
      case 'q' : //slight left
      case 'c' :
        servoSpeedLeft -= 5;
        servoSpeedRight -= 5;
        break;
      case 'e' : //slight right
      case 'z' : 
        servoSpeedLeft += 5;
        servoSpeedRight += 5;
        break;
      case 'w' : //forward
        if (isStopped())
        {
          servoSpeedLeft = SERVO_LEFT_FORWARD;
          servoSpeedRight = SERVO_RIGHT_FORWARD;
        }
        else
        {
          servoSpeedLeft += 10;
          servoSpeedRight -= 10;
        }
        break;
      case 'x' : //reverse
        if(isStopped())
        {
          servoSpeedLeft = SERVO_LEFT_REVERSE;
          servoSpeedRight = SERVO_RIGHT_REVERSE;
        }
        else
        {
          servoSpeedLeft -= 10;
          servoSpeedRight += 10;
        }
        break;
      case 'a' : //left
        if(isStopped())
        {
          servoSpeedLeft = SERVO_LEFT_REVERSE;
          servoSpeedRight = SERVO_RIGHT_FORWARD;
        }
        else
        {
          servoSpeedLeft -= 20;
          servoSpeedRight -= 20;
        }
        break;
      case 'd' : //right
        if(isStopped())
        {
          servoSpeedLeft = SERVO_LEFT_FORWARD;
          servoSpeedRight = SERVO_RIGHT_REVERSE;
        }
        else
        {
          servoSpeedLeft += 20;
          servoSpeedRight += 20;
        }
        break;
      case 's' : //stop
        servoSpeedLeft = SERVO_STOP;
        servoSpeedRight = SERVO_STOP;
        break;

      // opening and closing of gripper
      case 'j' :
        gripperPos = 2400;
        servoGripper.writeMicroseconds(gripperPos);
        delay(SERVO_DELAY);
        break;
      case 'k' :
        gripperPos = 1500;
        servoGripper.writeMicroseconds(gripperPos);
        delay(SERVO_DELAY);
        break;
      case 'l' : 
        gripperPos = 800;
        servoGripper.writeMicroseconds(gripperPos);
        delay(SERVO_DELAY);
        break;

      // camera up and down
      case 'b' :
        cameraPos += 10;
        servoCamera.writeMicroseconds(cameraPos);
        delay(SERVO_DELAY);
        break;
      case 'g' :
        cameraPos = 1500;
        servoCamera.writeMicroseconds(cameraPos);
        delay(SERVO_DELAY);
        break;
      case 't' :
        cameraPos -= 10;
        servoCamera.writeMicroseconds(cameraPos);
        delay(SERVO_DELAY);
        break;

      // gripper up and down
      case 'n' :
        if (wristPos < WRIST_MAX)
        {
          wristPos += 20;
          servoWrist.writeMicroseconds(wristPos);
          delay(SERVO_DELAY);
        }
        break;
      case 'h' : 
        wristPos = 1500;
        servoWrist.writeMicroseconds(wristPos);
        delay(SERVO_DELAY);
        break;
      case 'y' :
        if (wristPos > WRIST_MIN) 
        {
          wristPos -= 20;
          servoWrist.writeMicroseconds(wristPos);
          delay(SERVO_DELAY);
        }
        break;
    }

    servoLeft.writeMicroseconds(servoSpeedLeft);
    servoRight.writeMicroseconds(servoSpeedRight);

    Serial.print("left ");
    Serial.println(servoSpeedLeft);
    Serial.print("right ");
    Serial.println(servoSpeedRight);
    
    Serial.print("gripper ");
    Serial.println(gripperPos);
    Serial.print("Camera ");
    Serial.println(cameraPos);
    Serial.print("Wrist ");
    Serial.println(wristPos);

  }

}
