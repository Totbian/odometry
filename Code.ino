#include <pitches.h>                                          // Calls pitches library for sound
#include <Wire.h>                                             // Calls for I2C bus library for MD25
#include <Servo.h>                                            // Calls servo library for dispenser
#define MD25ADDRESS         0x58                              // Address of the MD25
#define SPEED1              0x00                              // Byte to send speed to both motors for forward and backwards motion if operated in MODE 2 or 3 and Motor 1 Speed if in MODE 0 or 1
#define SPEED2              0x01                              // Byte to send speed for turn speed if operated in MODE 2 or 3 and Motor 2 Speed if in MODE 0 or 1
#define ENCODERONE          0x02                              // Byte to read motor encoder 1
#define ENCODERTWO          0x06                              // Byte to read motor encoder 2
#define ACCELERATION        0xE                               // Byte to define motor acceleration
#define CMD                 0x10                              // Byte to reset encoder values
#define MODE_SELECTOR       0xF                               // Byte to change between control MODES

int Mode = 0;                                                 // MODE in which the MD25 will operate selector value 
float radius = 13;                                            // Distance from one wheel to the center
float pi = 3.14159265359;                                     // Value of pi
float Wheel_Distance_CM = 0;                                  // both wheel distance
float Wheel_1_Distance = 0;                                   // Wheel 1 travel distance variable
float Wheel_2_Distance_CM = 0;                                // Wheel 2 travel distance variable
int servo_pos = 146;                                          // variable to store the servo position
int stop_delay = 10;                                          // delay after each manouver on the track
Servo myservo;                                                // create servo object to control a servo


void setup(){
  Wire.begin();                                               // Begin I2C bus
  Serial.begin(9600);                                         // Begin serial
  delay(100);                                                 // Wait for everything to power up
  myservo.attach(9);                                          // attaches the servo on pin 9 to the servo object
  myservo.write(servo_pos);                                   // sets the servo to the initial marker pickup location
  Wire.beginTransmission(MD25ADDRESS);                        // Set MD25 operation MODE
  Wire.write(MODE_SELECTOR);
  Wire.write(Mode);                                           
  Wire.endTransmission();
  encodeReset();                                              // Calls a function that resets the encoder values to 0 
}

void loop(){

  // forward(distance,speed);  - speed 128+ for forward 128- reverse
  // Turn(angle,speed); - angle - degrees +ve left -ve right
  
  forward(34,150);                // 13-1 forward
  Turn(-90,150);                  // 1 turn right
  forward(26,150);                // 1-2 forward
  drop();                         // 2 drop marker
  Turn(52.5946,150);              // 2 turn left
  forward(36.21693,150);          // 2-11 forward
  Turn(37.4054,150);              // 11 turn left
  Backwardandturn(18, 0.76, 40);  // 11-10 reverse allong a circular path(radius(cm), fraction of circle, speed of outer wheel)
  drop();                         // 10 drop marker
  Turn(-95,150);                  // 10 turn right
  forward(18,150);                // 10-9 forward
  Turn(36,150);                   // 9 turn left
  forward(62.22895,106);          // 9-8 forward
  Turn(48,150);                   // 8 turn left
  drop();                         // 8 drop marker
  forward(39,150);                // 8-5 forward
  Turn(85,150);                   // 5 turn left
  forward(37.5,150);              // 5-6 forward
  Turn(89,145);                   // 6 turn left
  drop();                         // 6 drop marker
  forward(39,150);                // 6-7 forward
  Turn(89,145);                   // 7 turn left
  forward(39,150);                // 7-8 forward
  Turn(-87,145);                  // 8 turn right
  forward(23,150);                // 8-4 forward
  Turn(90,150);                   // 4 turn left
  drop();                         // 4 drop marker
  Forwardandturn(26,0.25,150);    // 4-3 forward allong a circular path(radius(cm), fraction of circle, speed of outer wheel)       
  Turn(87,150);                   // 3 turn left
  forward(50,106);                // 3-2 forward
  Turn(142.6,150);                // 2 turn left
  forward(42.80187,150);          // 2-13 forward
  Turn(-142.6,150);               // 13 turn right
  
exit(0);
}


void drop(){                                                 // control servo to drop marker
  for (servo_pos = 146; servo_pos >= 67; servo_pos -= 1) {   // goes from marker pickup to drop position in steps of 1 degree
   myservo.write(servo_pos);                                 // tell servo to go to position in variable 'pos'
   delay(1);                                                 // delay between each degree the servo travels
  }
  delay(200);
  for (servo_pos = 67; servo_pos <= 146; servo_pos += 1) {   // goes from drop position to pickup next marker
    myservo.write(servo_pos);                                // tell servo to go to position in variable 'pos'
    delay(1);                                                // delay between each degree the servo travels
  }
}


void forward(float a, int _speed){                                                          // This function moves the platform forward by distance a at speed _speed
  float Wheel_Distance_CM = a;                                                              // Wheel travel distance variable   
  encoder1();                                                                               // Calls a function that reads value of encoder 1
  encoder2();                                                                               // Calls a function that reads value of encoder 2

    while(abs(encoder1()) <= Wheel_Distance_CM && abs(encoder2()) <= Wheel_Distance_CM){   // while statement to check the status of the traveled distance
      Wire.beginTransmission(MD25ADDRESS);                                                 // Sets the acceleration to register 1 (6.375s)
      Wire.write(ACCELERATION);
      Wire.write(2);
      Wire.endTransmission();
  
      Wire.beginTransmission(MD25ADDRESS);                      // Sets motor 1 speed value
      Wire.write(SPEED1);
      Wire.write(_speed);
      Wire.endTransmission();                                                
  
      Wire.beginTransmission(MD25ADDRESS);                      // Sets motor 2 speed value
      Wire.write(SPEED2);
      Wire.write(_speed);
      Wire.endTransmission();
   }
  stopMotor();                                                // Calls function that stops the platform 
  encodeReset();                                              // Calls function that resets the encoder values to 0 l
  delay(stop_delay);                                          // Delay after movement
  
}

void Turn(float c,int _speed){                               // This function turns the platform
                                                             //c is angle in degrees(+ve for left -ve for right,
                                                             //_speed is the speed of turn
  encoder1();                                                // Calls a function that reads value of encoder 1
  encoder2();                                                // Calls a function that reads value of encoder 2
  float z = radius * ((c)*(pi/180));                         // Convert angle to distance wheels need to move.
  int n = abs(c)/c;                                          // n value sets directions of wheels -1 if angle -ve +1 if angle +ve
  Wheel_Distance_CM = z;                                     // Sets wheel travel distance value - cm, for upcoming turn motion function

    while (abs(encoder1()) <= abs(Wheel_Distance_CM) && abs(encoder2()) <= abs(Wheel_Distance_CM)){     // while statement to check the status of the traveled distance
      Wire.beginTransmission(MD25ADDRESS);                                                              // Sets the acceleration to register 2
      Wire.write(ACCELERATION);
      Wire.write(2);
      Wire.endTransmission();
  
      Wire.beginTransmission(MD25ADDRESS);                     //Sets speed of motor 2
      Wire.write(SPEED2);                                      
      Wire.write(n*_speed);
      Wire.endTransmission();
  
      Wire.beginTransmission(MD25ADDRESS);                     //Sets speed of motor 1 (-ve motor 2)
      Wire.write(SPEED1);                                      
      Wire.write(-n*_speed);
      Wire.endTransmission();
    }
  stopMotor();                                                // Calls a function that stops the platform 
  encodeReset();                                              // Calls a function that resets the encoder values to 0 l
                                                              
  tone(8,NOTE_E6,125);                                        // Play sound
  delay(500);                                                 // for 0.5s
  delay(stop_delay);
}

void Forwardandturn(float r, float g, int vout){                                        // This function moves the robot while provoking an arc trajectory due to speed differences,
                                                                                        //r is the radius of the circle covered in the anti clockwise direction(-ve for clockwise),
                                                                                        //g is the fraction of the radius completed, vout is the velocity of the outer wheel.
  encoder1();                                                                           // Calls a function that reads value of encoder 1
  encoder2();                                                                           // Calls a function that reads value of encoder 2
  float Wheel_1_Distance_CM = (r-radius)*2*pi*g;                                        // Wheel 1 travel distance based on, radius of turn, radius between wheel and center of robot,
                                                                                        // and fraction of circle traveled
  float Wheel_2_Distance_CM = (r+radius)*2*pi*g;                                        // Wheel 2 travel distance ""           ""
    
    while (encoder1() <= Wheel_1_Distance_CM && encoder2() <= Wheel_2_Distance_CM){     // while statement to check the status of the traveled distance
      Wire.beginTransmission(MD25ADDRESS);                                              // Sets motor 2 speed value
      Wire.write(SPEED2);
      Wire.write(vout);
      Wire.endTransmission();
  
      Wire.beginTransmission(MD25ADDRESS);                                              // Sets motor 1 speed value
      Wire.write(SPEED1);
      Wire.write(round(((vout-128)*(Wheel_1_Distance_CM/Wheel_2_Distance_CM))+128));    // calculates speed of wheel 1 so that both wheels travel respective distances in the same time
      Wire.endTransmission();                                               
    }
  stopMotor();                                              // Calls a function that stops the platform 
  encodeReset();                                            // Calls a function that resets the encoder values to 0 
  delay(200);
}
void Backwardandturn(float r, float g, int vout){                                       // This function moves the robot while provoking an arc trajectory due to speed differences,
                                                                                        //r is the radius of the circle covered in the anti clockwise direction(-ve for clockwise),
                                                                                        //g is the fraction of the radius completed, vout is the velocity of the outer wheel.
  encoder1();                                                                           // Calls a function that reads value of encoder 1
  encoder2();                                                                           // Calls a function that reads value of encoder 2
  float Wheel_1_Distance_CM = -(r+radius)*2*pi*g;                                       // Wheel 1 travel distance based on, radius of turn, radius between wheel and center of robot,
                                                                                        // and fraction of circle traveled
  float Wheel_2_Distance_CM = -(r-radius)*2*pi*g;                                       // Wheel 2 travel distance ""           ""
    while (encoder1() >= Wheel_1_Distance_CM && encoder2() >= Wheel_2_Distance_CM){     // while statement to check the status of the traveled distance
      Wire.beginTransmission(MD25ADDRESS);                                              // Sets a combined motor speed value
      Wire.write(SPEED1);
      Wire.write(vout);
      Wire.endTransmission();
  
      Wire.beginTransmission(MD25ADDRESS);                                              // Sets a combined motor speed value
      Wire.write(SPEED2);
      Wire.write(114);
      Wire.endTransmission();                                               
    }
  stopMotor();                                              // Calls a function that stops the platform 
  encodeReset();                                            // Calls a function that resets the encoder values to 0 
  delay(stop_delay);
}

void encodeReset(){                                         // This function resets the encoder values to 0
  Wire.beginTransmission(MD25ADDRESS);
  Wire.write(CMD);
  Wire.write(0x20);                                                                    
  Wire.endTransmission(); 
  delay(1);
}

float encoder1(){                                           // Function to read and display value of encoder 1 as a long
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to get a reading from encoder 1
  Wire.write(ENCODERONE);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 4);                         // Request 4 bytes from MD25
  while(Wire.available() < 4);                              // Wait for 4 bytes to arrive
  long poss1 = Wire.read();                                 // First byte for encoder 1, HH.
  poss1 <<= 8;
  poss1 += Wire.read();                                     // Second byte for encoder 1, HL
  poss1 <<= 8;
  poss1 += Wire.read();                                     // Third byte for encoder 1, LH
  poss1 <<= 8;
  poss1  +=Wire.read();                                     // Fourth byte for encoder 1, LLalue
  delay(5);                                                 // Wait for everything to make sure everything is sent
  return(poss1*0.09);                                       // Convert encoder value to cm
}

float encoder2(){                                           // Function to read and display velue of encoder 2 as a long
  Wire.beginTransmission(MD25ADDRESS);           
  Wire.write(ENCODERTWO);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 4);                         // Request 4 bytes from MD25
  while(Wire.available() < 4);                              // Wait for 4 bytes to become available
  long poss2 = Wire.read();                                 // First byte for encoder 2, HH
  poss2 <<= 8;
  poss2 += Wire.read();                                     // Second byte for encoder 2, HL             
  poss2 <<= 8;
  poss2 += Wire.read();                                     // Third byte for encoder 2, LH             
  poss2 <<= 8;
  poss2  +=Wire.read();                                     // Fourth byte for encoder 2, LLalue
  delay(5);                                                 // Wait to make sure everything is sent
  return(poss2*0.09);                                       // Convert encoder value to cm
}

void stopMotor(){                                           // Function to stop motors
  
  Wire.beginTransmission(MD25ADDRESS);                      // Sets the acceleration to register 5
  Wire.write(ACCELERATION);
  Wire.write(5);
  Wire.endTransmission();

  Wire.beginTransmission(MD25ADDRESS);                      // Stops motor 1 
  Wire.write(SPEED1);
  Wire.write(128);
  Wire.endTransmission();

  Wire.beginTransmission(MD25ADDRESS);                      // Stops motor 2
  Wire.write(128);
  Wire.endTransmission();
  delay(25);

  encoder1();                                                // Calls a function that reads value of encoder 1
  encoder2();                                                // Calls a function that reads value of encoder 2
