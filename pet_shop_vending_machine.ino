#include <LiquidCrystal_I2C.h>
#include <Wire.h>
const int items = 4;
const int coins = 5;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

volatile bool insert = false, choosing_item = true;
volatile int pulse = 0;
volatile int sum = 0;
volatile int item = 0;

const int a[items] = {280, 300, 120, 220},
          b[coins] = {10, 20, 50, 100, 200};

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choose Item: ");
  lcd.print(item);

  pinMode(3, INPUT_PULLUP);
  digitalWrite(3, HIGH);
  //pinMode(2, INPUT_PULLUP);
  delay(1);
  attachInterrupt(digitalPinToInterrupt(2), coinInterrupt, FALLING);
}

void loop()
{
  if (!digitalRead(3) && choosing_item)
  {
    item++;
    if (item > items ) item = 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Choose Item: ");
    lcd.print(item);
    lcd.setCursor(0, 1);
    lcd.print("Price: ");
    lcd.print((float)a[item - 1] / 100);
    delay(500);
  }
  if (insert)
  {
    delay(700);
    sum += b[pulse - 1];
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Chosen Item: ");
    lcd.print(item);
    lcd.setCursor(0, 1);
    lcd.print("Remaining: ");
    lcd.print((float)(a[item - 1] - sum) / 100);
    insert = false;
    pulse = 0;
    choosing_item = false;
    if (sum >= a[item - 1])
    {
      //rikse to antikeimeno
      sum = 0;
      choosing_item = true;
      item = 0;
      lcd.clear();
      lcd.print("Thank you!!!");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Choose Item: ");
      lcd.print(item);
    }
  }
}

void coinInterrupt()
{
  pulse++;
  insert = true;
}
