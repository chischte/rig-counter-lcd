/*
 * *****************************************************************************
 * RIG-COUNTER-LCD
 * *****************************************************************************
 * Program that counts the number of times a switch has been pushed
 * The count value will be displayed on an lcd display
 * *****************************************************************************
 * Michael Wettstein
 * August 2024, Zürich
 * *****************************************************************************
 */

#include <Arduino.h>
#include <Debounce.h> // https://github.com/chischte/debounce-library.git
#include <EEPROM_Counter.h> // https://github.com/chischte/eeprom-counter-library.git
#include <Insomnia.h> // https://github.com/chischte/insomnia-delay-library.git

// CREATE THE EEPROM COUNTER:
EEPROM_Counter switchCounter;

// CREATE THE TIMEOUT TIMER:
Insomnia timeout(5000);

// CREATE A BLINK DELAY:
Insomnia blinkDelay;
//*****************************************************************************
// DECLARATION OF VARIABLES
//*****************************************************************************
// bool (true/false)
// byte (0-255)
// int   (-32,768 to 32,767)
// long  (-2,147,483,648 to 2,147,483,647)
// float (6-7 Digits)
//*****************************************************************************

// KNOBS AND POTENTIOMETERS:
const byte TEST_SWITCH_PIN = 2;
Debounce testSwitch(TEST_SWITCH_PIN);
const byte MOTOR_RELAY_PIN = 50;

// SENSORS:
// n.a.

// EEPROM
enum counter {
  longTimeCounter, // example value name
  shortTimeCounter, // example value name
  toolIdentNumber, // example value name
  somethingElse, // example value name
  endOfEnum // add additional values before this one...
  // ...this has to be the last one!
};
int numberOfValues = endOfEnum;

int eepromMinAddress = 500; //  has to be 0 or bigger
int eepromMaxAddress = 1000; // max EEPROM size -1

// OTHER VARIABLES:
bool previousButtonState;
bool previousMachineState;
bool machineRunning = false;
bool buttonBlinkEnabled = false;

//*****************************************************************************

//*****************************************************************************
//******************######**#######*#######*#******#*######********************
//*****************#********#**********#****#******#*#*****#*******************
//******************####****#####******#****#******#*######********************
//***********************#**#**********#****#******#*#*************************
//*****************######***######*****#*****######**#*************************
//*****************************************************************************
void setup() {
  switchCounter.setup(eepromMinAddress, eepromMaxAddress, numberOfValues);
  Serial.begin(9600); // start serial connection
  pinMode(TEST_SWITCH_PIN, INPUT_PULLUP);
  pinMode(MOTOR_RELAY_PIN, OUTPUT);
  digitalWrite(MOTOR_RELAY_PIN, HIGH);
  timeout.set_flag_activated(false); // timeout will be set active later
  // updateDisplayCounter();
  Serial.println("EXIT SETUP");
}
//*****************************************************************************
//********************#*********#####***#####***######*************************
//********************#********#*****#*#*****#**#*****#************************
//********************#********#*****#*#*****#**######*************************
//********************#********#*****#*#*****#**#******************************
//********************#######***#####***#####***#******************************
//*****************************************************************************

void loop() {

  // DETECT IF MACHINE HAS BEEN SWITCHED OFF:
  if (!machineRunning) {
    if (machineRunning == !previousMachineState) {
      timeout.set_flag_activated(machineRunning);
      Serial.println("MACHINE SWITCHED OFF");
      previousMachineState = machineRunning;
    }
  }
  // GET SIGNAL FROM TEST SWITCH AND COUNT IT:
  bool debouncedButtonState = testSwitch.get_raw_button_state();
  if (previousButtonState != debouncedButtonState) {
    if (debouncedButtonState == LOW) {
      switchCounter.count_one_up(longTimeCounter);
      // updateDisplayCounter();
      timeout.reset_time();
      buttonBlinkEnabled = false;
    }
    previousButtonState = debouncedButtonState;
  }

  // RESET COUNT IF LONG BUTTON PUSH

  // DISPLAY COUNT VALUE ON LCD
}
