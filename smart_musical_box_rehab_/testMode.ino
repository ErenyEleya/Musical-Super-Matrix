
void stored_mode() {
  out=true;
  lcd.clear();
  calibrationMode=false;
  unsigned short value;
  EEPROM.get(0, value);
  lcd.print(value);
  lcd.print("   ");
  EEPROM.get(2, value);
  lcd.print(value);
  lcd.setCursor(0, 1);
  EEPROM.get(4, value);
  lcd.print(value);
  lcd.print("   ");
  EEPROM.get(6, value);
  lcd.print(value);

}

void calibration_mode() {
  delay(100);
  lcd.setCursor(0, 0);
  //  unsigned short savingValue = sensorValue;
  switch (iteration%6) {
    case 0:
      lcd.print(average_value(LUSensor));
      lcd.setCursor(6, 0);
      lcd.print(average_value(RUSensor));
      lcd.setCursor(0, 1);
      lcd.print(average_value(LDSensor));
      lcd.setCursor(6, 1);
      lcd.print(average_value(RDSensor));
      return;
    case 1: lcd.print("LUSensor  ");
      sensorValue[0] = average_value(LUSensor);
      lcd.print(sensorValue[0]); // we print the value here as the return will stop the function
      return;
    case 2:
      lcd.print("RUSensor  ");
      sensorValue[1] = average_value(RUSensor);
      lcd.print(sensorValue[1]);
      break;
    case 3: lcd.print("LDSensor  ");
      sensorValue[2] = average_value(LDSensor);
      lcd.print(sensorValue[2]);
      break;
    case 4 : lcd.print("RDSensor  ");
      sensorValue[3] = average_value(RDSensor);
      lcd.print(sensorValue[3]);
      break;
    default:
      iteration = 0;
      for (unsigned char i = 0; i < 4; i++) {
        EEPROM.put(i * 2, sensorValue[i]);
        EEPROM.commit();
        lcd.clear();
      }
  }
}


unsigned short average_value(unsigned char pin) // number of pin that we need to get the average of the analog value of it
{
  unsigned long long average = 0;
  for (unsigned int i = 0; i < averageSampleSize; i++) {
    average += analogRead(pin);
  }
  return (average / averageSampleSize);
}
