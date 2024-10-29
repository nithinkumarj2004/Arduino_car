//LED light PIn
const int led = 4;
// Front motor
const int ena = 5;
const int in1 = 7;
const int in2 = 8;

// Back motor
const int in3 = 12;
const int in4 = 13;
const int enb = 6;

// Speed control
int bSpeed = 150;
int fSpeed = 200; 

// Command variable
int command;

// IR pins
const int irRightEchoPin = A0;
const int irLeftEchoPin = A1;

// ULTRA SONIC PINS
const int rightTrigPin = 9;
const int rightEchoPin = A2;

const int middleTrigPin = 10;
const int middleEchoPin = A3;

const int leftTrigPin = 11;
const int leftEchoPin = A4;

// Define distance threshold
const int threshold = 70;
const int turn = 20;

// Define variables for sensor readings
long rightDuration;
int rightDistance;
long leftDuration;
int leftDistance;
long middleDuration;
int middleDistance;

// Auto-driving state
bool autoDriving = false;
//simple auto drive state
bool simpleAutoDriving = false;
// Line Follower state
bool lineFollower = false;
// Obstacle avoidance
bool obstacleDetected = false;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);
  
  // Set up ultrasonic sensor pins
  pinMode(rightTrigPin, OUTPUT);
  pinMode(middleTrigPin, OUTPUT);
  pinMode(leftTrigPin, OUTPUT);

  pinMode(leftEchoPin, INPUT);
  pinMode(rightEchoPin, INPUT);
  pinMode(middleEchoPin, INPUT);

  //Set up IR sensor pins
   pinMode(irRightEchoPin,INPUT);
   pinMode(irLeftEchoPin,INPUT);

   pinMode(led,OUTPUT);


   Serial.begin(9600);
}

void loop() {
  // Check for new commands
  if (Serial.available() > 0) {
    command = Serial.read();
    checkObstacle();
    handleCommand(command);
  }
  // Auto-driving logic
  if (autoDriving) {

    autoDrivingOnn();
  }
  if(simpleAutoDriving){
    simpleAutoDrivingOnn();
  }
  if(lineFollower){
     lineFollowerOnn();
  }
}

void handleCommand(int cmd) {
 
  switch (cmd) {
    case 'R':
        turnRight();
        break;
      case 'L':
        turnLeft();
        break;
      case 'F':
        
        if (!obstacleDetected) {
          moveForward();
        } else {
          stop();
          Serial.println("Obstacle ahead! Can't move forward.");
        }
        break;

      case 'B':
        moveBackward();
        break;
      case 'G':
         if (!obstacleDetected) {
          moveForwardLeft();
        } else {
          stop();
          Serial.println("Obstacle ahead! Can't move forward.");
        }
        break;
      case 'I':
        if (!obstacleDetected) {
          moveForwardRight();
        } else {
          stop();
          Serial.println("Obstacle ahead! Can't move forward.");
        }
        break;

      case 'H':
        moveBackwardLeft();
        break;
      case 'J':
        moveBackwardRight();
        break;
      case 'V':
        digitalWrite(led,HIGH);
        break;
      case 'v':
        digitalWrite(led,LOW);
        break;
    case 'A':
      autoDriving = true;  // Enable auto-driving
      Serial.println("Entering auto-drive mode");
      break;
    case 'a':
      autoDriving = false;  // Disable auto-driving
      stop();  // Stop the car
      Serial.println("Exiting auto-drive mode");
      break;
      case 'T':
      simpleAutoDriving = true;  // Enable auto-driving
      Serial.println("Entering auto-drive mode");
      break;
    case 't':
      simpleAutoDriving = false;  // Disable auto-driving
      stop();  // Stop the car
      Serial.println("Exiting auto-drive mode");
      break;
    case 'X':
      lineFollower = true;
      Serial.println("Entering Line follower mode");
      break;

    case 'x':
      lineFollower = false;
      Serial.println("Exiting Line follower mode");
      stop();
      break;
    

    case '0':
      bSpeed = 0;
      break;
    case '1':
      bSpeed = 100;
      break;
    case '2':
      bSpeed = 140;
      break;
    case '3':
      bSpeed = 170;
      break;
    case '4':
      bSpeed = 190;
      break;
    case '5':
      bSpeed = 200;
      break;
    case '6':
      bSpeed = 220;
      break;
    case '7':
      bSpeed = 230;
      break;
    case '8':
      bSpeed = 240;
      break;
    case '9':
      bSpeed = 250;
      break;
    case '10':
      bSpeed = 250;
      break;
    case 's':
      stop();
      break;
    default:
      stop();
      break;
  }
}

void lineFollowerOnn(){
  bSpeed = 55;
 //LED indicator
      digitalWrite(led,HIGH);
      delay(200);
      digitalWrite(led,LOW);
      delay(250);
      digitalWrite(led,HIGH);
      delay(200);
      digitalWrite(led,LOW);

  while(lineFollower){
    if (Serial.available() > 0) {
      int cmd = Serial.read();
      if (cmd == 'x' || cmd == 'F'|| cmd == 'B' || cmd == 'L' || cmd == 'L'
      || cmd == 'G' || cmd == 'I' || cmd == 'H'|| cmd == 'J' || cmd == 'A'  ) {
        lineFollower = false;  // Disable auto-driving
        stop();  // Stop the car
        //LED Indicator
        digitalWrite(led,HIGH);
        delay(200);
        digitalWrite(led,LOW);
        delay(200);
        Serial.println("Exiting line Follower mode");
        return;  // Exit the function to process new commands
      } else {
        handleCommand(cmd);  // Process other commands
        return;  // Exit the function to process new commands
      }
    } //if end
   checkObstacle(); // Check for obstacle in front
    int LEFT_SENSOR = digitalRead(irLeftEchoPin);
    int RIGHT_SENSOR = digitalRead(irRightEchoPin);
        Serial.print("Left = ");
        Serial.println(LEFT_SENSOR);
        Serial.print("Right = ");
        Serial.println(RIGHT_SENSOR);

    if (obstacleDetected) {
    // Obstacle detected, stop the forward movement
    stop();
    Serial.println("Obstacle detected! Stopping line following.");

    } 
    if(RIGHT_SENSOR==0 && LEFT_SENSOR==0) {
          Serial.println("move forward");
          moveForward(); //FORWARD
          
           }

      else if(RIGHT_SENSOR==1 && LEFT_SENSOR==0) {
          Serial.println("Right");
          moveForwardRight(); //Move Right
          
             }
      else if(RIGHT_SENSOR==0 && LEFT_SENSOR==1) {
          Serial.println("Left");
          moveForwardLeft(); //Move Left
        
              }

      else if(RIGHT_SENSOR==1 && LEFT_SENSOR==1) {
          Serial.println("stop");
          stop();  //STOP
        }
    
  }//while loop end
}//funtion end


void autoDrivingOnn() {
  //LED indicator
      digitalWrite(led,HIGH);
      delay(200);
      digitalWrite(led,LOW);
      delay(250);
      digitalWrite(led,HIGH);
      delay(200);
      digitalWrite(led,LOW);
      delay(250);
  // Check for new commands before proceeding
  bSpeed = 85;
  while (autoDriving) {
    // Check for commands every loop iteration
    if (Serial.available() > 0) {
      int cmd = Serial.read();
      if (cmd == 'a' || cmd == 'T'|| cmd == 'F'|| cmd == 'B' || cmd == 'L' || cmd == 'L'
      || cmd == 'G' || cmd == 'I' || cmd == 'H'|| cmd == 'J' || cmd == 'X' || cmd == 'x'  ) {
        autoDriving = false;  // Disable auto-driving
        stop();  // Stop the car
        //LED Indicator
        digitalWrite(led,HIGH);
        delay(200);
        digitalWrite(led,LOW);
        delay(200);
        Serial.println("Exiting auto-drive mode");
        return;  // Exit the function to process new commands
      } else {
        handleCommand(cmd);  // Process other commands
        return;  // Exit the function to process new commands
      }
    }

    // Proceed with auto-driving logic if no new commands
    rightDistance = getDistance(rightTrigPin, rightEchoPin);
    leftDistance = getDistance(leftTrigPin, leftEchoPin);
    middleDistance = getDistance(middleTrigPin, middleEchoPin);

    // Debugging output
    Serial.print("Right Distance: ");
    Serial.println(rightDistance);
    Serial.print("Middle Distance: ");
    Serial.println(middleDistance);
    Serial.print("Left Distance: ");
    Serial.println(leftDistance);

    // Decision making


  if (middleDistance < 25 && rightDistance < 15 && leftDistance < 15) {
      instanceBreak();
       // break function
      Serial.println("Break");
      digitalWrite(led,HIGH);
      delay(150);
      digitalWrite(led,LOW);
      delay(1000);
    }else {
    if (middleDistance > threshold && rightDistance > 15 && leftDistance > 15) {
      moveForward(); // Move forward if there's enough space
      Serial.println("Moving forward");
    } else if (rightDistance > leftDistance && middleDistance > 15) {
      moveForwardRight(); // Turn right if right side has more space
      Serial.println("Turning right");
    } else if (rightDistance < leftDistance && middleDistance > 15) {
      moveForwardLeft(); // Otherwise, turn left
      Serial.println("Turning left");
    }else{
      Serial.println("Stop");
      instanceBreak();
      delay(500);
    }

    }

    // Add a small delay to prevent excessive polling
    delay(100);
  }
}

void simpleAutoDrivingOnn() {
  //LED indicator
      digitalWrite(led,HIGH);
      delay(200);
      digitalWrite(led,LOW);
      delay(250);
      digitalWrite(led,HIGH);
      delay(200);
      digitalWrite(led,LOW);
      delay(250);
  // Check for new commands before proceeding
  bSpeed = 90;
  while (simpleAutoDriving) {
    // Check for commands every loop iteration
    if (Serial.available() > 0) {
      int cmd = Serial.read();
      if (cmd == 'a' || cmd == 't'|| cmd == 'F'|| cmd == 'B' || cmd == 'L' || cmd == 'L'
      || cmd == 'G' || cmd == 'I' || cmd == 'H'|| cmd == 'J' || cmd == 'X' ) {
        simpleAutoDriving = false;  // Disable auto-driving
        stop();  // Stop the car
        //LED Indicator
        digitalWrite(led,HIGH);
        delay(200);
        digitalWrite(led,LOW);
        delay(200);
        Serial.println("Exiting auto-drive mode");
        return;  // Exit the function to process new commands
      } else {
        handleCommand(cmd);  // Process other commands
        return;  // Exit the function to process new commands
      }
    }

    // Proceed with auto-driving logic if no new commands
    rightDistance = getDistance(rightTrigPin, rightEchoPin);
    leftDistance = getDistance(leftTrigPin, leftEchoPin);
    middleDistance = getDistance(middleTrigPin, middleEchoPin);

    // Debugging output
    Serial.print("Right Distance: ");
    Serial.println(rightDistance);
    Serial.print("Middle Distance: ");
    Serial.println(middleDistance);
    Serial.print("Left Distance: ");
    Serial.println(leftDistance);

    // Decision making
    

    if(middleDistance >= threshold && (rightDistance > 15 && leftDistance > 15)){
    if (middleDistance > threshold && (rightDistance > 20 && leftDistance > 20)) {
      moveForward();
      Serial.println("forward");
    } else if ((rightDistance > leftDistance && middleDistance > 20) || 
               (rightDistance > middleDistance && leftDistance < 20) || 
               (middleDistance < 35 && leftDistance < 20 && rightDistance > 40)) {
      if (rightDistance > 20 && leftDistance > 10 && middleDistance > 20) {
        moveForwardRight();
        Serial.println("right");
      }
    } else if ((leftDistance > rightDistance && middleDistance > 20) || 
               (leftDistance > middleDistance && rightDistance < 20) || 
               (middleDistance < 35 && rightDistance < 20 && leftDistance > 40)) {
      if (leftDistance > 20 && rightDistance > 10 && middleDistance > 20) {
        moveForwardLeft();
        Serial.println("left");
      }
    } 
    }
    else {
      stop();
      Serial.println("stop");
    }

    // Add a small delay to prevent excessive polling
    delay(30);
  }
}


void instanceBreak(){
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enb, 0);
    delay(100);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enb, 60);
    delay(50);
    stop();
    delay(1000);
}
int getDistance(int trigPin, int echoPin) {
  // Trigger the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the pulse duration
  long duration = pulseIn(echoPin, HIGH);

  // Calculate distance in cm
  int distance = (duration / 2) / 29.1;
  return distance;
}

void checkObstacle() {
      int distance = getDistance(middleTrigPin, middleEchoPin);
     
   if(distance <= 40 && bSpeed >= 230){
     obstacleDetected = true; 
     instanceBreak();
  } else if(distance <= 30 && bSpeed > 200){
     obstacleDetected = true; 
     instanceBreak();
  } else if(distance < 25 && bSpeed < 160){
    obstacleDetected = true; 
  } else if (distance < 20){
     obstacleDetected = true; 
  }
  else {
    obstacleDetected = false;
  }
  delay(100);
}

void moveForward() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enb, bSpeed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(ena, 0);
}

void moveBackward() {
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enb, bSpeed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(ena, 0);
}

void turnRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(ena, fSpeed);
}

void turnLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(ena, fSpeed);
}

void moveForwardRight() {
  moveForward();
  turnRight();
}

void moveForwardLeft() {
  moveForward();
  turnLeft();
}

void moveBackwardLeft() {
  moveBackward();
  turnLeft();
}

void moveBackwardRight() {
  moveBackward();
  turnRight();
}

void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ena, 0);
  analogWrite(enb, 0);
}
