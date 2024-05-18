// USED BOOLEANS ARE SERVICEMODE , CALIBRATIONMODE,GAMEMODE

void button_handler() {
  delay(50);
  if (digitalRead(button)) {
    return ;
  }
  out = false;
  unsigned long long lowTime = millis();
  while (!digitalRead(button) && millis() - lowTime < 7500) {
    if (serviceMode) {
      if (millis() - lowTime > 7000) { // calibration mode
        lcd.clear();
        lcd.setCursor(3, 0);
        averageSampleSize=5000;
        calibrationMode = !calibrationMode; // change from calibration mode to game mode and vice versa
        if (calibrationMode) {
          lcd.print("Calibration");
          lcd.setCursor(6, 1);
          lcd.print("mode");
        }
        else {
          lcd.print("Game mode");
          get_sensor_EEPROM(); // GET THE VALUES OF THE EEPROM IF THERE'S ANY CHANGES HAPPENED AT THE END OF THE CALIBRATION
          serviceMode = false;
        }
        delay(2000);
        lcd.clear();
        storedMode = false;
        iteration = 0; // reseting the value of the iteration
        return ;
      }

      else if ( (millis() - lowTime ) > 3500) {
        lcd.setCursor(2, 0);
        lcd.print("Stored value");
        lcd.setCursor(6, 1);
        lcd.print("Mode");
        storedMode = true;

      }
    }

  }


  lcd.clear();
  lowTime = millis() - lowTime; // storing the last value that the button pressed down in them


  if (!serviceMode) {
    if (iteration == 4 && lowTime > 7000) {
      serviceMode = true;
      lcd.setCursor(4, 0);
      lcd.print("SERVICE");
      lcd.setCursor(6, 1);
      lcd.print("MODE");
      delay(2000);
    }
    else if (lowTime > 2500 && selectedNumber != 0 && iteration > 0) {
      startGame = true;
      lcd.noBlink();
      lowTime = millis();
      while (millis() - lowTime < 3000) {
        lcd.setCursor(5, 0);
        lcd.print("START IN ");
        lcd.setCursor(7, 1);
        lcd.print( (char) ( (lowTime - millis() ) / 1000) + 4 );

      }
      lcd.clear();
      return ;
    }

    else if (lowTime > 1100 && iteration > 0)
    {
      lcd.blink();
      if (!setNumber) {
        selectedNumber = 0;
        setNumber = true;
        increase = true;
      }
      else if (increase) increase = false;
      else setNumber = false;
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Set");
      delay(800);
    }
    else if (setNumber) {
      if (increase)
        selectedNumber += 5;

      else
        selectedNumber -= 5;
    }
    else { // if not set number and time is less than 1.5 second (it is just a click )
      iteration++;
      lcd.clear();
      lcd.noBlink();
    }


  }
  else iteration++;

  // enter service mode options

  if (  iteration  > 4 )
    iteration = 0;


}




void get_sensor_EEPROM() {

  EEPROM.get(0, LUValue); // LUSensor location
  EEPROM.get(2, RUValue); // RUSensor location
  EEPROM.get(4, LDValue); // LDSensor location
  EEPROM.get(6, RDValue); // RDSensor location
}
