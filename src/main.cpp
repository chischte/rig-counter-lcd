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

// BUTTONS:
const byte COUNTER_BUTTON_PIN = 2;
const byte RESET_BUTTON_PIN = 0;

Debounce counter_button(COUNTER_BUTTON_PIN);
Debounce reset_button(RESET_BUTTON_PIN);


// EEPROM

EEPROM_Counter counter_storage;

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
  
  // PIN MODE
  pinMode(COUNTER_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
  
  // EEPROM
  counter_storage.setup(eepromMinAddress, eepromMaxAddress, numberOfValues);
  
  // VARIOUS
  Serial.begin(9600); // start serial connection
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

  if(counter_button.switched_low()){
    Serial.println("COUNT");
  }

  if(reset_button.switched_low()){
    Serial.println("RESET BTN PUSH");
  }
/*
  // DETECT IF MACHINE HAS BEEN SWITCHED OFF:
  if (!machineRunning) {
    if (machineRunning == !previousMachineState) {
      timeout.set_flag_activated(machineRunning);
      Serial.println("MACHINE SWITCHED OFF");
      previousMachineState = machineRunning;
    }
  }
  // GET SIGNAL FROM TEST SWITCH AND COUNT IT:
  bool debouncedButtonState = counter_button.get_raw_button_state();
  if (previousButtonState != debouncedButtonState) {
    if (debouncedButtonState == LOW) {
      counter_storage.count_one_up(longTimeCounter);
      // updateDisplayCounter();
      timeout.reset_time();
      buttonBlinkEnabled = false;
    }
    previousButtonState = debouncedButtonState;
  }

  // RESET COUNT IF LONG BUTTON PUSH

  // DISPLAY COUNT VALUE ON LCD
*/
}
