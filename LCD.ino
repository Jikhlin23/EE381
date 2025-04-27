
#include <LiquidCrystal.h>

// Initialize the library with the custom pin mapping
// LiquidCrystal(rs, en, d4, d5, d6, d7)
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

void setup() {
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  // Print something on both lines
  lcd.setCursor(0, 0); // First line
  lcd.print("Hello, Nikhil!");

  lcd.setCursor(0, 1); // Second line
  lcd.print("LCD connected :)");
}

void loop() {
  // Nothing here for now
}
