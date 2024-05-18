void generate_cell() {
  // turn off the old cell
  digitalWrite(rows[row], LOW); // TURN OFF THE TRANSISTOR TO GET HIGH IN THE GND
  digitalWrite(columns[2 * column], HIGH); // TURN ON THE TRANSISTOR TO GET LOW IN POSITIVE PIN
  digitalWrite(columns[2 * column + 1], HIGH);
  unsigned char oldColumn = column;

  row = random(600) / 200; // generate random number from 0 to 2
  column = random(600) / 200; // generate random number from 0 to 2 for choosing the column ( 2 sequentials for 1 column)
  if (row > 2)
    row = 2;
  if (column > 2)
    column = 2;

  while (oldColumn == column) { // making sure that not the same cell has chosen
    column = ceil(random(29) / 10);
  }

  lcd.setCursor(13, 0);
  lcd.print(row);
  lcd.setCursor(15, 0);
  lcd.print(column);

  if (row > 2)
    row = 2;
  if (column > 2)
    column = 2;

  digitalWrite(rows[row], HIGH); // we are reversed as we connected trnaisitor
  digitalWrite(columns[2 * column], LOW); // we are reversed as we connected transistor npn
  digitalWrite(columns[2 * column + 1], LOW); // we are reversed as we connected transistor npn
  successTrial = false;

}
void light_mixer() {
  analogWrite(columns[2 * column], random(0, 240)); // leds is connected to transistors so we are in reverse orderes
  analogWrite(columns[2 * column + 1], random(0, 240)); // led is onnected to transistors so we are in reverse orderes
}


void check_position() {
  unsigned char pawnRow = 1; // put pawn at the center of the table at position 1 for row and 1 for column
  unsigned char pawnCol = 1;
  sensorValue[0] = average_value(LUSensor);
  sensorValue[1] = average_value(RUSensor);
  sensorValue[2] = average_value(LDSensor);
  sensorValue[3] = average_value(RDSensor);

  if ((LUValue - sensorValue[0]) > sensitivity || ( LDValue - sensorValue[2] ) > sensitivity)
    pawnCol -= 1;

  if ((RUValue - sensorValue[1]) > sensitivity || (RDValue - sensorValue[3]) > sensitivity) {
    pawnCol += 1;
  }
  if ((LUValue - sensorValue[0]) > sensitivity || (RUValue - sensorValue[1]) > sensitivity)
    pawnRow += 1;

  if ((RDValue - sensorValue[3]) > sensitivity || (LDValue - sensorValue[2]) > sensitivity )
    pawnRow -= 1;
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print((LUValue - sensorValue[0]));
//  lcd.setCursor(8, 0);
//  lcd.print((RUValue - sensorValue[1]));
//  lcd.setCursor(0, 1);
//  lcd.print((LDValue - sensorValue[2]));
//  lcd.setCursor(8, 1);
//  lcd.print((RDValue - sensorValue[3]));
//  delay(500);
  // checking the mathcing of pawn place and targeted place
  if (pawnRow == row && pawnCol == column) {
    successTrial = true;
    score += 10;
  }


}
