#include <LiquidCrystal.h>

#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2

#define TRIGGER_PIN1 10
#define TRIGGER_PIN2 9
#define TRIGGER_PIN3 8

const float DISTANCE_CONVERSION_FACTOR = 0.01723;
const int DISTANCE_THRESHOLD = 100;
const int DELAY_TIME = 500;

LiquidCrystal_I2C lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  Serial.begin(9600);
}

long readDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

void loop() {
  float d1 = DISTANCE_CONVERSION_FACTOR * readDistance(TRIGGER_PIN1, TRIGGER_PIN1);
  float d2 = DISTANCE_CONVERSION_FACTOR * readDistance(TRIGGER_PIN2, TRIGGER_PIN2);
  float d3 = DISTANCE_CONVERSION_FACTOR * readDistance(TRIGGER_PIN3, TRIGGER_PIN3);

  Serial.print("d1 = ");
  Serial.print(d1);
  Serial.println(" cm");

  Serial.print("d2 = ");
  Serial.print(d2);
  Serial.println(" cm");

  Serial.print("d3 = ");
  Serial.print(d3);
  Serial.println(" cm");

  if (d1 > DISTANCE_THRESHOLD && d2 > DISTANCE_THRESHOLD && d3 > DISTANCE_THRESHOLD) {
    lcd.setCursor(0, 0);
    lcd.print("3 Slots Free");
    lcd.setCursor(0, 1);
    lcd.print("Slot 1 2 3 Free");
  } else if ((d1 > DISTANCE_THRESHOLD && d2 > DISTANCE_THRESHOLD) ||
             (d2 > DISTANCE_THRESHOLD && d3 > DISTANCE_THRESHOLD) ||
             (d3 > DISTANCE_THRESHOLD && d1 > DISTANCE_THRESHOLD)) {
    lcd.setCursor(0, 0);
    lcd.print("2 Slots Free");
    lcd.setCursor(0, 1);
    if (d1 > DISTANCE_THRESHOLD && d2 > DISTANCE_THRESHOLD)
      lcd.print("Slot 1 & 2 Free");
    else if (d1 > DISTANCE_THRESHOLD && d3 > DISTANCE_THRESHOLD)
      lcd.print("Slot 1 & 3 Free");
    else
      lcd.print("Slot 2 & 3 Free");
  } else if (d1 < DISTANCE_THRESHOLD && d2 < DISTANCE_THRESHOLD && d3 < DISTANCE_THRESHOLD) {
    lcd.setCursor(0, 0);
    lcd.print("No Slot Free");
    lcd.setCursor(0, 1);
    lcd.print("Parking Full");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("1 Slot Free");
    lcd.setCursor(0, 1);
    if (d1 > DISTANCE_THRESHOLD)
      lcd.print("Slot 1 is Free");
    else if (d2 > DISTANCE_THRESHOLD)
      lcd.print("Slot 2 is Free");
    else
      lcd.print("Slot 3 is Free");
  }

  delay(DELAY_TIME);
}