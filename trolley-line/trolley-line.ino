/**
 * For use with trolley motor, (2) IR sensors, and a button
 * 
 * Use the 2 IR sensors on each side of track
 * Motor control to move the trolley (plus direction)
 * Button to start
 * 
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
#define SENSOR_PIN_ONE 4  //right
#define SENSOR_PIN_TWO 2  //left
#define BUTTON_PIN 7
#define BUTTON_PIN_LED 8
#define MUSIC_PIN 13

// current set direction of the motor (forward or reverse)
int direction = FORWARD;

// current & last state of both ir sensors (one & two)
int irSensorOne = 0, irOneLastState = 0;
int irSensorTwo = 0, irTwoLastState = 0;

// current state of the trolley/motor, moving or not
bool moving = false;

// used for detecting idle state while debugging (see loop())
bool idleLogFlag = false;

// time limit: adjust to your needs, it takes current trolley about 22 seconds to make it between sensors
unsigned long total_time = 25000;
unsigned long current_time = 0;
unsigned long previous_time = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LEDPIN, OUTPUT); // ready pin (on arudino board - debug only)
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // start button
  pinMode(BUTTON_PIN_LED, OUTPUT); // start button's LED
  pinMode(SENSOR_PIN_ONE, INPUT); // sensor one
  pinMode(SENSOR_PIN_TWO, INPUT); // sensor two
  pinMode(MUSIC_PIN, OUTPUT); // music

  digitalWrite(LEDPIN, HIGH);
  digitalWrite(BUTTON_PIN, HIGH);
  digitalWrite(BUTTON_PIN_LED, HIGH);
  digitalWrite(SENSOR_PIN_ONE, HIGH);
  digitalWrite(SENSOR_PIN_TWO, HIGH);
  digitalWrite(MUSIC_PIN, HIGH);
  
  // setup motor
  AFMS.begin();
  myMotor->setSpeed(175);

  Serial.println("Hello! Ready!");
}


void loop() {
  if (moving) {
    current_time = millis();

    if (previous_time == 0) {
      // if previous_time is set to zero, set it to current_time
      // (this is a guard - just in case someone forgot to set previous_time)
      previous_time = current_time;
    }
    
    // continue moving (or start if this is the first time we're here) 
    myMotor->run(direction);

    // only check the sensor that the trolley is moving towards
 
    if (direction == FORWARD) {
      checkSensor(2, SENSOR_PIN_TWO, irSensorTwo, irTwoLastState, BACKWARD);
    }

    if (direction == BACKWARD) {
      checkSensor(1, SENSOR_PIN_ONE, irSensorOne, irOneLastState, FORWARD);
    }

    if (!moving) {
      digitalWrite(MUSIC_PIN, HIGH);
      // if we have stopped, pause before allowing another button press
      delay(4000);
      setReadyLED(HIGH);
    }
    
  } else {
    // we're not moving - wait for button press

    if (!idleLogFlag) {
      // this is used so that we only log idling once (and not a million times)
      Serial.println("Idling...");
      idleLogFlag = true;  
    }

    if (digitalRead(BUTTON_PIN) == LOW) {
      // start button has been pressed - other side of if statement will be run when the loop continues
      startTrolley();
    }
  }
}

/**
 * set the `moving` flag so that the trolley will start in the next iteration of loop()
 */
void startTrolley() {
  Serial.println("button pressed - STARTING!");
  resetIRState();
  moving = true;
  setReadyLED(LOW);
  digitalWrite(MUSIC_PIN, LOW);

  // start counting time
  previous_time = current_time;
}

/**
 * reset all the "last state" variables
 * idleLogFlag is used for debugging - see loop()
 */
void resetIRState() {
  irOneLastState = 1;
  irTwoLastState = 1;
  idleLogFlag = false;
}

/**
 * Control the state of the "ready" LED
 * This function keeps the state of the Start Button LED & the Ardunio LED in sync
 * 
 * The LED Pin is used for debugging - it's the LED on the aurdino board itself
 * The Start Button has an LED inside it that the user will see
 */
void setReadyLED(int newState) {
  digitalWrite(LEDPIN, newState);
  digitalWrite(BUTTON_PIN_LED, newState);
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
  if (sensorTripped || timeIsUp) {
    //stop motor
      
    // log result
    if (sensorTripped) {
      Serial.print("STOPPED: IR Sensor tripped: ");
      Serial.println(id);
    }
    if (timeIsUp) {
      Serial.println("STOPPED: time is up");
    }

    // stop everything, wait one second
    moving = false;
    digitalWrite(MUSIC_PIN, HIGH);
    myMotor->run(RELEASE);
    current_time = 0;
    previous_time = 0;
    delay(1000);

    // change direction (to be ready for next button press)
    direction = newDirection;
  }

  irLastState = irCurrentState;
}
