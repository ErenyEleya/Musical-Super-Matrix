#include<LiquidCrystal.h>
#include<EEPROM.h>
#include <ESP.h>
#include<math.h>
#define LUSensor 39
#define LDSensor 36
#define RUSensor 34
#define RDSensor 35
#define button 14
#define maxSize 200
#define sensitivity 15 // the sensitivity  (it could be changed)

String gameOptions[] = {"CHOOSE GAME", "Time mode", "trials mode", "speed up mode", "Time reaction"};
unsigned char rows[] = {33, 22, 23};
unsigned char columns[] = {13, 12, 18, 19, 21, 32};
unsigned char trials[maxSize], row = 1, column = 1, oldColumn = 1, oldRow = 1, trial = 0;
unsigned int reactionTime[maxSize], distance[maxSize];
unsigned short sensorValue[4]; // global array for values of sensors
unsigned short LUValue, LDValue, RUValue, RDValue; // values of sensors that we read from the memory
unsigned short selectedNumber = 0; // selected number ( time or number of trials )
unsigned short averageSampleSize = 0; // this value would be 500 read in game mode for fast performance and 2000 in callibration for good values
const int rs = 15, en = 2, d4 = 4, d5 = 16, d6 = 17, d7 = 5;
bool calibrationMode = false, storedMode = false, successTrial = false, startGame = false, gameOver = false;
bool serviceMode = false, increase = true, setNumber = false, out = false;
// setNumberis used for setting number of trials or time the option while selecting the game options
// serviceMode to know if you are in the service mode or in the game mode
// increase variable is used to know if you are using the button to increase the set number or decrease it

void stored_mode(void);
void calibration_mode(void);
unsigned short average_value(unsigned char pin);
//Write EEPROM enable;
unsigned char iteration = 0;
unsigned int score = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  EEPROM.begin(8); // saving 5 positions in EEPROM folded in LU , RU , LD , RD starting from 0 address to 6 (2 Bytes for each)and 7,8 is free
  pinMode(button, INPUT);

  for (int i = 0; i < 3; i++) {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], LOW);
  }

  for (int i = 0; i < 6; i++) {
    pinMode(columns[i], OUTPUT);
    digitalWrite(columns[i], HIGH);
  }

  lcd.begin(16, 2);
  lcd.setCursor(6, 0);
  lcd.print("SMRB ");
  //  delay(1000);
  lcd.setCursor(2, 1);
  lcd.print("hands power");
  get_sensor_EEPROM(); // getting sensor values from EEPROM it is void as it takes the values of the global variables as pointers
  //  delay(3000);
  lcd.clear();
  //  generate_cell();
}

void loop() {

  if (digitalRead(button) == LOW) {
    button_handler(); // checking if the button is pressed or not and in which mode should system go (change system mode)
  }
  else if (!out) {
    if (storedMode)
      stored_mode();
    else if (calibrationMode)
      calibration_mode();
    else if (!serviceMode) {
      if (!startGame)
        choose_mode();
      else if (gameOver)
        result_view();
      else
        game_mode();
    }

  }
}

void choose_mode() {
  out = true;
  lcd.setCursor(0, 0);
  lcd.print(gameOptions[iteration]);
  lcd.setCursor(0, 1);

  switch (iteration) {
    case 0:
      lcd.setCursor(14, 1);
      lcd.write(0x2D); // making dash sign
      lcd.write(0x7E); // making right arrow sign
      return;
    case 2:
      lcd.print("Trials : ");
      break;
    default :
      lcd.print("TIME : ");
      break;
  }
  lcd.setCursor(9, 1);
  lcd.print(selectedNumber);
  if (setNumber) {
    lcd.setCursor(14, 1);
    if (increase)
    { lcd.write(0x2D);
      lcd.write(0x7E);
    }
    else {
      lcd.write(0x7F);
      lcd.write(0x2D);
    }
  }

}

void game_mode() {
  averageSampleSize = 200;
  generate_cell();
  switch (iteration) {
    case 1 : time_mode();
      break;
    case 2 : trials_mode();
      break;
    case 4 : reaction_time_mode();
      break;

  }
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("GAME OVER");
  lcd.setCursor(0, 1);
  lcd.print("SCORE : ");
  lcd.print(score);

}

void result_view() {
  unsigned int i = 0;
  while (1) {
    bool pressed = false;
    unsigned long long pressedTime = millis();
    if (!digitalRead(button)) { // checking for debouncing
      delay(30);
      if (!digitalRead(button))
        pressed = true;
    }
    while (!digitalRead(button) && (millis() - pressedTime) < 7100 ) {

    }
    if (pressed) {
      pressedTime = millis() - pressedTime;
      if (pressedTime > 7000) { // restart from the begining to make all values back to ideal values and ready for new game
        ESP.restart();
      }
      else if (pressedTime < 7000) {
        // turn off the old cell
        digitalWrite(rows[row], LOW); // TURN OFF THE TRANSISTOR TO GET HIGH IN THE GND
        digitalWrite(columns[2 * column], HIGH); // TURN ON THE TRANSISTOR TO GET LOW IN POSITIVE PIN
        digitalWrite(columns[2 * column + 1], HIGH);

        lcd.clear();
        lcd.print(i);
        row = (unsigned char )(trials[i] / 10);  // get the row
        column = trials[i] % 10;   // get the column
        lcd.setCursor(3, 0);
        lcd.print("RT:");
        lcd.print(reactionTime[i]);
        lcd.setCursor(12, 0);
        lcd.print("d:");
        lcd.print(distance[i]);
        lcd.setCursor(0, 1);
        lcd.print("TN:");
        lcd.print(trial);
        lcd.setCursor(7, 1);
        lcd.print("V:");
        lcd.print(  ( distance[i] * 1000.0 / ( reactionTime[i]  ) ));
        //
        //        //turn on the selected cell
        digitalWrite(rows[row], HIGH); // we are reversed as we connected trnaisitor
        digitalWrite(columns[2 * column], LOW); // we are reversed as we connected transistor npn
        digitalWrite(columns[2 * column + 1], LOW); // we are reversed as we connected transistor npn
        ++i %= trial;
      }

    }
  }
}
