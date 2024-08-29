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

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

// CREATE INSOMNIA TIMERS:
int counter_reset_time_short = 800;
int counter_reset_time_long = 6000;

Insomnia reset_timeout_short(counter_reset_time_short);
Insomnia reset_timeout_long(counter_reset_time_long);

//*****************************************************************************
// DECLARATION OF VARIABLES
//*****************************************************************************
// bool (true/false)
// byte (0-255)
// int   (-32,768 to 32,767)
// long  (-2,147,483,648 to 2,147,483,647)
// float (6-7 Digits)
//*****************************************************************************

unsigned long counter_value_short = 666;
unsigned long counter_value_long = 777;

// BUTTONS:
const byte COUNTER_BUTTON_PIN = 2;
const byte RESET_BUTTON_PIN = 0;

Debounce counter_button(COUNTER_BUTTON_PIN);
Debounce reset_button(RESET_BUTTON_PIN);

// EEPROM

EEPROM_Counter counter_storage;

enum counter {
  stored_shorttime, //
  stored_longtime, //
  endOfEnum //
};
int numberOfValues = endOfEnum;

int eepromMinAddress = 0; //  has to be 0 or bigger
int eepromMaxAddress = 1023; // max EEPROM size -1

// OTHER VARIABLES:
bool previousButtonState;
bool previousMachineState;
bool machineRunning = false;
bool buttonBlinkEnabled = false;

//*****************************************************************************

void update_lcd() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(counter_value_short);
  lcd.setCursor(0, 1);
  lcd.print(counter_value_long);
}

void monitor_reset() {

  if (reset_button.switched_low()) {
    reset_timeout_short.set_flag_activated(true);
    reset_timeout_long.set_flag_activated(true);

    reset_timeout_short.reset_time();
    reset_timeout_long.reset_time();
  }

  if (reset_timeout_short.is_marked_activated() && reset_timeout_short.has_timed_out()) {
    counter_value_short = 0;
  }

  if (reset_timeout_long.is_marked_activated() && reset_timeout_long.has_timed_out()) {
    counter_value_short = 0;
    counter_value_long = 0;
  }

  if (reset_button.switched_high()) {
    reset_timeout_short.set_flag_activated(false);
    reset_timeout_long.set_flag_activated(false);
  }
}

void monitor_count_button() {
  if (counter_button.switched_low()) {
    counter_value_long++;
    counter_value_short++;
  }
}

bool value_has_changed() {

  bool has_changed = false;

  // SHORT:
  static unsigned long prev_counter_value_short = counter_value_short;
  if (prev_counter_value_short != counter_value_short) {
    has_changed = true;
    prev_counter_value_short = counter_value_short;
  }

  // LONG:
  static unsigned long prev_counter_value_long = counter_value_long;
  if (prev_counter_value_long != counter_value_long) {
    has_changed = true;
    prev_counter_value_long = counter_value_long;
  }

  return has_changed;
}

void update_eeprom() {
  counter_storage.set_value(stored_shorttime, counter_value_short);
  counter_storage.set_value(stored_longtime, counter_value_long);
}

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

  counter_value_short = counter_storage.get_value(stored_shorttime);
  counter_value_long = counter_storage.get_value(stored_longtime);

  // DISPLAY
  lcd.init(); // initialize the lcd
  lcd.backlight();
  update_lcd();

  // VARIOUS
  Serial.begin(9600); // start serial connection
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

  monitor_reset();

  monitor_count_button();

  if (value_has_changed()) {
    update_eeprom();
    update_lcd();
  }
}
