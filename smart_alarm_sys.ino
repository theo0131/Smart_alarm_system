#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <string.h>

#define pin_servo 3
#define pin_pir1 13
#define pin_pir2 2
#define pin_rpi 4
#define pin_led 5

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo myservo;
int servoStep = 5;
int servoAngle = 90;
bool detectLeft = false;
bool detectRight = false;

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0, A1, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad
int LCDRow = 0;
int no_key = 0;
int correct_pwsd = 0;
char pwsd[] = "0000";
int buzzer_on = 0;
int counter_led = 0;
int direction_led = 1;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup(){
  pinMode(pin_pir1, INPUT);
  pinMode(pin_pir2, INPUT);
  pinMode(pin_rpi, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(pin_led, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(LCDRow, 0);
  noTone(12);
  myservo.attach(pin_servo);
  myservo.write(servoAngle);
  delay(500);
}

void PIRSensor(int pin){
  if (digitalRead(pin) == HIGH) {
    Serial.print("Motion detected on ");
    Serial.println(pin);
    if (pin == pin_pir1)
    	detectLeft = true;
    else
    	detectRight = true;
  }
  else {
    if (pin == pin_pir1)
    	detectLeft = false;
    else
    	detectRight = false;
  }
}

void Photo()
{
  Serial.println("Take picture");
  digitalWrite(pin_rpi, HIGH);
}

void ServoMotion()
{
  Serial.print("Angle: ");
  Serial.println(servoAngle);
  if (detectLeft == false && detectRight == false)
  	return;
  else if (detectLeft == true && detectRight == true){
        Photo();
       // delay(3000);
       if(buzzer_on == 0)
       {
         buzzer_on = 1;
       }
      	return;
  }
  // move to the left
  if (detectLeft && !detectRight){
    if (servoAngle - servoStep < 0)
      return;
    servoAngle -= servoStep;
    myservo.write(servoAngle);
    detectLeft = false;
  }
  
  // move to the right
  if (!detectLeft && detectRight){
    if (servoAngle + servoStep > 180)
      return;
    servoAngle += servoStep;
    myservo.write(servoAngle);
    detectRight = false;
  }

  //delay(100);
}
  
void loop(){
 // delay(100);
  PIRSensor(pin_pir1);
  PIRSensor(pin_pir2);
  ServoMotion();

  if (buzzer_on == 1) {
    tone(12, 700);
    if (direction_led > 1)
      counter_led += 1;
    else
      counter_led -= 1;
    if (counter_led == 255 || counter_led == 0)
      direction_led *= -1;
    analogWrite(pin_led, counter_led);
    delay(15);
  }
  else {
    analogWrite(pin_led, 0);
    noTone(12);
  }


  char key = keypad.getKey();
  if (key) {
    if (no_key == 0) {
    	lcd.clear();
    }
    if (no_key < 4){
      lcd.setCursor(LCDRow, 0);
      Serial.println(key);
      lcd.print(key);
      lcd.setCursor (++LCDRow, 0);
      pwsd[no_key] = key;
    }
    if (no_key == 3){
     
      Serial.println(pwsd);
      if (strcmp(pwsd, "1234") == 0) {
          correct_pwsd = 1;
      }
    }
    no_key++;
  }
  
  if (correct_pwsd == 1) {
  	lcd.clear();
    lcd.print("Correct");
    buzzer_on = -1;
    correct_pwsd = 0;
    no_key = 0;
    LCDRow = 0;
  }
  else if (no_key == 4 && correct_pwsd == 0)
  {
    lcd.clear();
    lcd.print("Incorrect");
    no_key = 0;
    LCDRow = 0;
  }
  
}