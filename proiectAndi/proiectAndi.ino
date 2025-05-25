#include <SPI.h> //library - ultrasonic
#include <Stepper.h> //library - stepper

// variables input/output
int buzzer = 3;
int led = 2;
bool ledState = LOW;
int irL = 6;
int irR = 7;
int trigPin = 4;
int echoPin = 5;
//1cm -> buzz, back 2s

// motor
int stepsPerRev = 2048; //steps of motor
Stepper motorL = Stepper(stepsPerRev, 8,10,9,11); 
Stepper motorR = Stepper(stepsPerRev, A4,A6,A5,A7);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("Distance sensor initialized!");

  pinMode(buzzer, OUTPUT);
  Serial.println("Buzzer initialized!");

  pinMode(led, OUTPUT);
  Serial.println("LED initialized!");

  pinMode(irL,INPUT);
  pinMode(irR,INPUT);
  Serial.println("IR sensor initialized!");

  digitalWrite(led, LOW);
  digitalWrite(buzzer, LOW);
  Serial.println("-------------");

}

void loop() {
  lineFollowing();
  obstacleFound();
}

void ledBlink(){
  // light LED if on line with sensor
  if(digitalRead(irL) == HIGH or digitalRead(irR) == HIGH)
    ledState = HIGH;
  else
    ledState = LOW;
  digitalWrite(led, ledState);  
}

long distanceCalculation(){ // read ultrasonic sensor
  digitalWrite(trigPin,LOW); // makes sure sensor is not transmiting 
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH); // sensor HIGH for 10 microseconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin,HIGH);
  Serial.println("distance is:");
  Serial.print((duration/2) / 29.1); // convert duration to distance[cm] using speed of sound
  return (duration/2) / 29.1;  
} 

void BuzzTime(int stay){ // buzz for specified time
  digitalWrite(buzzer, HIGH);
  delay(stay);
  digitalWrite(buzzer,LOW);
}

void motorController(int motor, int motorSpeed, int front){ // function for motor control
  if(motor == 1){//motor L
    motorL.setSpeed(motorSpeed);
    motorL.step(front*stepsPerRev);
  } else if(motor == 0){ // motor R
    motorR.setSpeed(motorSpeed);
    motorR.step(front*stepsPerRev);
  }
}

void obstacleFound(){ //algorithm for obstacle
  if(distanceCalculation() < 1){
    motorController(1,0,1); //stop robot
    motorController(0,0,1);
    BuzzTime(2000); // buzz
    motorController(1,5,1); // go back 2 seconds
    motorController(0,5,-1);
    delay(2000);
    motorController(1,0,1); // stop robot
    motorController(0,0,1);
  }
}

void lineFollowing(){
  ledBlink(); // blink led if on line
  if(digitalRead(irL)==HIGH){ // robot rotates to the left if left sensor high
    motorController(1,0,1);
    motorController(0,5,1);
  }
  else if(digitalRead(irR)==HIGH){ // robot rotates to the right if right sensor high
    motorController(0,0,1);
    motorController(1,5,-1);
  } else { // robot goes forward
    motorController(0,5,1);
    motorController(1,5,-1);
  }
}
