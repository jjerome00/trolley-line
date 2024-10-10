/**
 * For use with trolley motor, (2) IR sensors, and a button
 * 
 * Use the 2 IR sensors on each side of track
 * Motor control to move the trolley (plus direction)
 * Button to start
 * 
 * Left and Right are positions as if you are 
 * standing in front of the display
 * 
 * FORWARD is going towards the Left side
 * BACKWARD is going towards the Right side
 */

// Motor controller
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();  

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
// (End) Motor controller

#define LEDPIN 13
#define SENSOR_PIN_RIGHT 4 // right (ONE)
#define SENSOR_PIN_LEFT 2  // left (TWO)
#define BUTTON_PIN 7
#define BUTTON_PIN_LED 8
#define MUSIC_PIN 12

// current set direction of the motor (forward or reverse)
int direction = FORWARD; //forward is towards Left

// current & last state of both ir sensors (left & right)
int irSensorRight = 0, irSensorRightLast = 0;
int irSensorLeft = 0, irSensorLeftLast = 0;

// current state of the trolley/motor, moving or not
bool moving = false;

// used for detecting idle state while debugging (see loop())
bool idleLogFlag = false;

// time limit: just in case a sensor doesn't work 
// adjust to your needs, it takes current trolley <20 seconds to make it between sensors
unsigned long total_time = 20000;
unsigned long current_time = 0;
unsigned long previous_time = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LEDPIN, OUTPUT); // ready pin (on arudino board - debug only)
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // start button
  pinMode(BUTTON_PIN_LED, OUTPUT); // start button's LED
  pinMode(SENSOR_PIN_RIGHT, INPUT); // sensor one / right
  pinMode(SENSOR_PIN_LEFT, INPUT); // sensor two / left
  pinMode(MUSIC_PIN, OUTPUT); // music

  digitalWrite(LEDPIN, HIGH);
  digitalWrite(BUTTON_PIN, HIGH);
  digitalWrite(BUTTON_PIN_LED, HIGH);
  digitalWrite(SENSOR_PIN_RIGHT, HIGH);
  digitalWrite(SENSOR_PIN_LEFT, HIGH);
  
  playMusic(false);

  Serial.println("Hello!");
  
  // setup motor
  AFMS.begin();
  myMotor->setSpeed(155);

  Serial.println("Ready!");
  Serial.println("Starting trolley on first run");
  startTrolley();
}


void loop() {
  if (moving) {
    current_time = millis();

    if (previous_time == 0) {
      // if previous_time is set to zero, set it to current_time
      previous_time = current_time;
    }
    
    // continue moving (or start if this is the first time we're here) 
    myMotor->run(direction);

    // only check the sensor that the trolley is moving towards
    if (direction == FORWARD) {
      checkSensor(2, SENSOR_PIN_LEFT, irSensorLeft, irSensorLeftLast, BACKWARD);
    }
    else if (direction == BACKWARD) {
      checkSensor(1, SENSOR_PIN_RIGHT, irSensorRight, irSensorRightLast, FORWARD);
    }

    // check to see if we've stopped and update state
    if (!moving) {
      playMusic(false);
      // pause before allowing another button press
      delay(4000);
      setReadyLight(true);
    }
    
  } else {
    // we're not moving - wait for button press

    // ensure we're not playing music
    playMusic(false);

    if (!idleLogFlag) {
      // ensure we only log "idling" once (and not a million times)
      Serial.println("Idling...");
      idleLogFlag = true;  
    }

    if (digitalRead(BUTTON_PIN) == LOW) {
      // start button has been pressed 
      // - the other side of main if statement will be run when the loop continues
      startTrolley();
    }
  }
}

/**
 * set the `moving` flag so that the trolley will start in the next iteration of loop()
 */
void startTrolley() {
  if (direction == FORWARD) {
    Serial.println("button pressed - START going LEFT");
  } else {
    Serial.println("button pressed - START going RIGHT");
  }
  
  resetIRState();
  moving = true;
  setReadyLight(false);
  playMusic(true);
  
  // idleLogFlag is used for debugging - see loop()
  idleLogFlag = false;
  
  // start counting time; used to determine how long it's been moving
  previous_time = current_time;

  // direction is already set, motor will start when it hits the main loop() function
}

void playMusic(bool play) {
  if (play) {
    digitalWrite(MUSIC_PIN, LOW);
  } else {
    digitalWrite(MUSIC_PIN, HIGH);
  }
}

/**
 * reset all the "last state" variables
 */
void resetIRState() {
  irSensorRightLast = 1;
  irSensorLeftLast = 1;
}

/**
 * Control the state of the "ready" LED
 * This function keeps the state of the Start Button LED & the Ardunio LED in sync
 * 
 * The LED Pin is used for debugging - it's the LED on the aurdino board itself
 * The Start Button has an LED inside it that the user will see
 */
void setReadyLight(bool onState) {
  if (onState) {
    digitalWrite(LEDPIN, HIGH);
    digitalWrite(BUTTON_PIN_LED, HIGH);
  } else {
    digitalWrite(LEDPIN, LOW);
    digitalWrite(BUTTON_PIN_LED, LOW);
  }
}

/**
 * stop everything, wait one second
 * called from checkSensor(...)
 */
void stopEverything() {
  moving = false;
  playMusic(false);
  myMotor->run(RELEASE);
  current_time = 0;
  previous_time = 0;
  delay(1000);
}

/**
 * Check current state of sensor given. If sensor tripped stop the motor & change direction of motor (do not start it)
 * These state variables are passed in by reference, so make sure these are correct.
 */
void checkSensor(int id, int sensorPin, int &irCurrentState, int &irLastState, int newDirection) {
  // get current state
  irCurrentState = digitalRead(sensorPin);

  bool timeIsUp = (current_time - previous_time >= total_time);
  bool sensorTripped = (irCurrentState != irLastState);

  // check against the last reading of state (this indicates if the sensor has been tripped)
  if (sensorTripped) {
    //stop motor
    if (id == 2) {
      Serial.println("STOPPED: IR Sensor tripped on LEFT (2)");
    } else {
      Serial.println("STOPPED: IR Sensor tripped on RIGHT (1)");
    }
    
    // stop everything, wait one second
    stopEverything();

    // change direction (to be ready for next button press)
    direction = newDirection;
  }
  else if (timeIsUp) {
    //stop motor - done separately to keep from screwing up the state
    if (id == 2) {
      Serial.println("STOPPED: time is up; sensor was LEFT (2)");
    } else {
      Serial.println("STOPPED: time is up; sensor was RIGHT (1)");
    }

    // stop everything, wait one second
    stopEverything();

    // change direction (to be ready for next button press)
    direction = newDirection;
  }

  // save current state until the next time we check it
  irLastState = irCurrentState;
}
