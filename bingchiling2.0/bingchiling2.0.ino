#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver myServo = Adafruit_PWMServoDriver();

//#define SERVORIGHTFAST 250
//#define SERVOLEFTSLOW 400
//#define SERVORIGHTSLOW 300
//#define SERVOLEFTFAST 500
#define SERVOMIN 250
#define SERVOMAX 500
#define PIN_ANALOG_X 0
#define PIN_ANALOG_Y 1
#define STICK_X_MIDDLE = 491
#define STICK_Y_MIDDLE = 516
float stick_x, stick_y, magnitude, stick_dir;
int RIGHT = 0;
int LEFT = 0;
int position;

void setup() {
  Serial.begin(9600);
  delay(10);
}

void loop() {
  //Input Polling
  //Get vector of analog stick
  int ANALOG_X = analogRead(PIN_ANALOG_X);
  int ANALOG_Y = analogRead(PIN_ANALOG_Y);
  //Serial.print("x: ");Serial.print(ANALOG_X);
  //Serial.print(" y: ");Serial.print(ANALOG_Y);
  //Map analog to ranges -1.0 to 1.0
  //Accomodate for drifting
  stick_x = (ANALOG_X - 491) / 491.0;
  if (stick_x > 1.0) stick_x = 1.0;
  else if (stick_x < -1.0) stick_x = -1.0;
  stick_y = ((ANALOG_Y - 25) - 491) / 491.0;
  if (stick_y > 1.0) stick_y = 1.0;
  else if (stick_y < -1.0) stick_y = -1.0;  
  Serial.print(" sx: ");Serial.print(stick_x);
  Serial.print(" sy: ");Serial.print(stick_y);
  //Get magnitude of the vector
  magnitude = sqrt(pow(stick_x, 2) + pow(stick_y, 2));
  //Serial.print(" m: ");Serial.print(magnitude);
  
  if (magnitude > 0.75) {
    Serial.println(" - FAST");
    Serial.print("right =");
    Serial.println(RIGHT);
    RIGHT = 250;
    LEFT = 500;
   
  }
  else if (magnitude > 0.0) { 
    Serial.print(" - SLOW");
    RIGHT = 300;
    LEFT = 400;
  }
  else {
    Serial.print(" - STOP");
    stop();
  }

  stick_dir = getStickDirection(stick_x, stick_y);

  if (stick_dir == 1 || stick_dir == 4 || stick_dir == 7)
    {
      Serial.print(" - LEFT");
//      position = constrain(LEFT, SERVOMIN,SERVOMAX);
      myServo.setPWM(0,0,LEFT);
    }
  else if (stick_dir == 3 || stick_dir == 6 || stick_dir == 9)
    {
      Serial.print(" - RIGHT");
//      position = constrain(RIGHT, SERVOMIN,SERVOMAX);
      myServo.setPWM(0,0,RIGHT);
    }
  else if (stick_dir == 7 || stick_dir == 8 || stick_dir == 9)
    Serial.print(" - UP");
  else if (stick_dir == 1 || stick_dir == 2 || stick_dir == 3)
    Serial.print(" - DOWN");    
  
  
  Serial.println(" ");

  
  //Servo Operation

  

  
  //Finish
  delay(100);
}

void stop(){
  myServo.setPWM(0,0,0);
}
//void rotateTo(int position){
//  position = constrain(position, SERVOMIN,SERVOMAX);
//  myServo.setPWM(0,0,position);
//}
int dir8[]  = { 8, 9, 6, 3, 2, 1, 4, 7, 0 };

float getStickAngle(float x, float y){
  float rad = atan2(x, y);
  if (rad < 0.0f) rad += PI * 2.0f;
  float deg = 180.0f * rad / PI;
  return deg;
}

int getStickDirection(float x, float y){
  if (magnitude == 0) return 0;
  float sectorSize = 360.0f / 8.0f;
  float angle = getStickAngle(x, y) + sectorSize / 2.0f;
  int dir = (int)floor(angle / sectorSize);
  return dir8[dir];
}
