#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const unsigned long studyTime = 26UL * 60UL * 1000UL;
const unsigned long breakTime = 7UL * 60UL * 1000UL;
const unsigned long countdownInterval = 1000;

unsigned long countdownStartTime = 0;
unsigned long lastDisplayTime = 0;

int currentPhase = 1;

void setup() {
  lcd.begin();
  lcd.backlight();

  // Greeting
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Hello Sanjana");
  delay(5000);

  // Starting message
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Starting...");
  delay(3000);

  countdownStartTime = millis();
  lastDisplayTime = millis() - countdownInterval;
  currentPhase = 2;
}

void loop() {
  unsigned long currentTime = millis();

  if (currentPhase == 2) {
    displayStudyCountdown(currentTime);
  }
  else if (currentPhase == 3) {
    displayBreakCountdown(currentTime);
  }
}

void displayStudyCountdown(unsigned long currentTime) {

  unsigned long elapsedTime = currentTime - countdownStartTime;

  if (elapsedTime >= studyTime) {
    currentPhase = 3;
    countdownStartTime = millis();
    lastDisplayTime = millis() - countdownInterval;
    return;
  }

  if (currentTime - lastDisplayTime >= countdownInterval) {

    lastDisplayTime = currentTime;

    unsigned long remainingTime = studyTime - elapsedTime;

    int minutes = remainingTime / 60000;
    int seconds = (remainingTime % 60000) / 1000;

    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Study Time");

    lcd.setCursor(5, 1);

    if (minutes < 10) lcd.print("0");
    lcd.print(minutes);
    lcd.print(":");

    if (seconds < 10) lcd.print("0");
    lcd.print(seconds);
  }
}

void displayBreakCountdown(unsigned long currentTime) {

  unsigned long elapsedTime = currentTime - countdownStartTime;

  if (elapsedTime >= breakTime) {
    currentPhase = 2;
    countdownStartTime = millis();
    lastDisplayTime = millis() - countdownInterval;
    return;
  }

  if (currentTime - lastDisplayTime >= countdownInterval) {

    lastDisplayTime = currentTime;

    unsigned long remainingTime = breakTime - elapsedTime;

    int minutes = remainingTime / 60000;
    int seconds = (remainingTime % 60000) / 1000;

    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Break Time");

    lcd.setCursor(5, 1);

    if (minutes < 10) lcd.print("0");
    lcd.print(minutes);
    lcd.print(":");

    if (seconds < 10) lcd.print("0");
    lcd.print(seconds);
  }
}
  
