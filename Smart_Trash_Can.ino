#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3c

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo servo;

const int redPin = 4;
const int greenPin = 5;
const int bluePin = 6;
const int servoPin = 7;
const int echoPin1 = 8;
const int trigPin1 = 9;
const int echoPin2 = 10;
const int trigPin2 = 11;


float distanceMeasurement(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned long duration = pulseIn(echoPin, HIGH);
  return ((float)(340 * duration) / 10000) / 2;
}


void displaySpace(int distance2)
{
  display.clearDisplay();

  int spaceValue = map(distance2, 0, 40, 100, 0);

  if (spaceValue < 0)
    spaceValue = 0;

  display.setCursor(0, 40);
  display.setTextSize(2);
  display.print("Space:");
  delay(500);
  display.print(spaceValue);
  display.print("%");
}


void setup()
{  
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  servo.attach(servoPin);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  servo.write(45);
}


void loop()
{
  float distance1 = distanceMeasurement(trigPin1, echoPin1);
  float distance2 = distanceMeasurement(trigPin2, echoPin2);

  if (distance1 < 10)
  {
    for (int angle = 45; angle <= 180; angle++)
    {
      servo.write(angle);
      delay(5);
    }

    while (distanceMeasurement(trigPin1, echoPin1) < 10)
    {
      // 대기
    }

    delay(2000);

    for (int angle = 180; angle >= 45; angle--)
    {
      servo.write(angle);
      delay(10);
    }
  }

  displaySpace(distance2);
  
  if (distance2 < 10)
  {
    analogWrite(redPin, 150);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);

    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Please empty");
    display.println("");
    display.print("The trash can !");
    display.display();
    display.print("!");
    delay(500);
    display.display();
    display.println("!");
    delay(500);
  }
  else if ( (distance2 >= 10) && (distance2 <= 15) )
  {
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 10);

    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Almost Full");
  }
  else
  {
    analogWrite(redPin, 0);
    analogWrite(greenPin, 10);
    analogWrite(bluePin, 0);

    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Good Condition");
  }

  display.display();
}