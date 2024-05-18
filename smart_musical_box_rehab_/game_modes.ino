String greetings[] = {"Great", "Keep going", "Go on", "Amazing", "Wonderful", "Magic", "WooooW"};
unsigned char greetingNumber = 0;
#define greetingSize 7
#define check_pressed() \
  if (!digitalRead(button)) {\
    unsigned long long Time = millis();\
    while (millis() - Time < 5000) {\
      gameOver = true; \
      return; \
    }\
  }
#define success() {\
    lcd.setCursor(0, 1);\
    lcd.print("SCORE : ");\
    lcd.print(score);\
    trials[trial] = row * 10 + column; /* trial is the size of the trials array to know the score achieved*/ \
    reactionTime[trial]=millis()-trialTime;\
    distance[trial++]=sqrt( ( (oldColumn-column)*(oldColumn-column) )*81 + ( (oldRow-row)*(oldRow-row) )*36  );\
    oldColumn=column;\
    oldRow=row;\
    generate_cell(); \
    trialTime=millis();\
  }


#define lcd_refresh(parameter,remained)     \
  if (remained == 99 || remained == 9)\
  { \
    lcd.clear(); \
    lcd.setCursor(0, 0); \
    lcd.print(parameter); \
  }

void time_mode() {
  unsigned long long trialTime = millis();
  unsigned long long startTime = millis();
  while ((millis() - startTime) / 1000 < selectedNumber) // selected number is in seconds
  {
    lcd_refresh("TIME : ", (selectedNumber - (int)((millis() - startTime) / 1000) ) );
    lcd.setCursor(7, 0);
    lcd.print(selectedNumber - (int)((millis() - startTime) / 1000));

    check_position();
    if (successTrial)
      success();

    check_pressed();

  }
  gameOver = true;

}

void trials_mode() {
  unsigned long long trialTime = millis();
  while (trial < selectedNumber ) {
    lcd_refresh("TRIALS: ", trial);
    lcd.setCursor(9, 0);
    lcd.print(trial);

    check_position();
    if (successTrial)
      success();
    check_pressed();
  }
  gameOver = true;
}


void reaction_time_mode() {
  reactionTime[0] = -1; // get the biggest number and store it (unsigned int) 
  trial++;
  unsigned long long trialTime = millis();
  /*the reaction time array is stored in it the last reaction  time that is achieved in the last iteration
    then we loop unitl this number is less than the enetered number */
  while (reactionTime[trial - 1] > selectedNumber*10) // stay looping when the reaction time is bigger than the selected time in ms
  {
    check_position();
    if (successTrial)
      success();

    check_pressed();

  }
  gameOver = true;


}


void lcd_greeting() {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print(greetings[greetingNumber]);
  ++greetingNumber %= greetingSize;
  lcd.setCursor(0, 1);
  lcd.print("SCORE : ");


}
