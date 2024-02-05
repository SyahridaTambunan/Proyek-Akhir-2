#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Password.h>

#define buzzer 12

Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

String newPasswordString;
char newPassword[6];
byte a = 5;
bool value = true;

Password password = Password("9999");

byte maxPasswordLength = 6;
byte currentPasswordLength = 0;
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

char keys[ROWS][COLS] = {
  {'2', '1', '3', 'A'},
  {'5', '4', '6', 'B'},
  {'8', '7', '9', 'C'},
  {'0', '*', '#', 'D'},
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  servo.attach(11);
  servo.write(0); // Initialize servo position to 0 degrees
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("WELCOME TO");
  lcd.setCursor(0, 1);
  lcd.print("DOOR LOCK SYSTEM");
  delay(3000);
  lcd.clear();
}

void loop() {
  lcd.setCursor(1, 0);
  lcd.print("ENTER PASSWORD");

  char key = keypad.getKey();
  if (key != NO_KEY) {
    delay(60);
    if (key == 'C') {
      resetPassword();
    } else if (key == 'D') {
      if (value == true) {
        doorlocked();
        value = false;
      } else if (value == false) {
        dooropen();
        value = true;
      }
    } else {
      processNumberKey(key);
    }
  }
}

void processNumberKey(char key) {
  lcd.setCursor(a, 1);
  lcd.print("*");
  a++;
  if (a == 11) {
    a = 5;
  }
  currentPasswordLength++;
  password.append(key);

  if (currentPasswordLength == maxPasswordLength) {
    doorlocked();
    dooropen();
  }
}

void dooropen() {
  if (password.evaluate()) {
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    servo.write(45); // Move the servo to 45 degrees
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("CORRECT PASSWORD");
    lcd.setCursor(0, 1);
    lcd.print("OPEN THE DOOR...");
    delay(2000);
    lcd.clear();
    a = 5;
  } else {
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    lcd.setCursor(0, 0);
    lcd.print("WRONG PASSWORD!");
    lcd.setCursor(0, 1);
    lcd.print("PLEASE TRY AGAIN");
    delay(2000);
    lcd.clear();
    a = 5;
  }
  resetPassword();
}

void resetPassword() {
  password.reset();
  currentPasswordLength = 0;
  lcd.clear();
  a = 5;
}

void doorlocked() {
  if (password.evaluate()) {
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    servo.write(0); // Move the servo to 0 degrees (locked position)
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("CORRECT PASSWORD");
    lcd.setCursor(2, 1);
    lcd.print("DOOR LOCKED");
    delay(2000);
    lcd.clear();
    a = 5;
  } else {
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    lcd.setCursor(0, 0);
    lcd.print("WRONG PASSWORD!");
    lcd.setCursor(0, 1);
    lcd.print("PLEASE TRY AGAIN");
    delay(2000);
    lcd.clear();
    a = 5;
  }
  resetPassword();
}
