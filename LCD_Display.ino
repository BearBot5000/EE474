/*
Authors: Brenotn Mizell and Eric Martin
EE 474 Lab4: Final Project
06/07/2023
GREENHOUSE: Display
*/


// ------------------LCD SETUP-------------------
void LCDsetup() {
  // Set up the LCD's number of columns and rows
  lcd.begin(16, 2);

  // Display the initial message
  lcd.print("Planty Mcplanter");
}

// Function to print text to LCD
// The function will clear and rewrite if new text is different from current
void printToLCD(String line1, String line2) {
  if (currentLCDLine1 != line1 || currentLCDLine2 != line2) {
    lcd.clear();
    lcd.home();
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
    currentLCDLine1 = line1;
    currentLCDLine2 = line2;
  }
}