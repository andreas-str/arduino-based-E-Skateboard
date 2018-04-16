#include <SoftwareSerial.h>

SoftwareSerial mySerial(5, 4);

int potpin = A1; //potentiometer
int batmespin = A0; //internal battery measure analog pin
int trigppin = 2; //trigger button pin
int lightpin = 3; //lights button pin
int batlevelpin1 = 13; //leds for batt level
int batlevelpin2 = 12; //^
int batlevelpin3 = 11; //^
int batconpin1 = 10; //batleven controller
int bluetoothcontrol = A2; //bt AT mode
bool trigSafe = false;
bool batSafe = true;
int serialdata;
int lightstate = 0;
int ledState = HIGH;
unsigned long previousMillis = 0;
const long interval = 750;

void setup() {
  pinMode(trigppin, INPUT_PULLUP);
  pinMode(lightpin, INPUT_PULLUP);
  pinMode(batlevelpin1, OUTPUT);
  pinMode(batlevelpin2, OUTPUT);
  pinMode(batlevelpin3, OUTPUT);
  pinMode(batconpin1, OUTPUT);
  pinMode(bluetoothcontrol, OUTPUT);
  digitalWrite(bluetoothcontrol, LOW);
  mySerial.begin(9600);
  delay(50);
}

void loop() {
  unsigned long currentMillis = millis();
  int trigVal = digitalRead(trigppin); //read trigger button
  int lightVal = digitalRead(lightpin); // read lights button

  analogReference(DEFAULT);
  for (int i = 1; i < 5; i++) {
    analogRead(potpin);
    delay(i);
  }
  int throttle = analogRead(potpin); // read analog in
  analogReference(INTERNAL);
  for (int i = 1; i < 5; i++) {
    analogRead(batmespin);
    delay(i);
  }
  int batlevel = analogRead(batmespin); //read internal battery

  throttle = map(throttle, 506, 1023, 1100, 1800); //converted before printing in serial, 506-1023 converted to 1.1ms-1.8ms

  if (trigVal == HIGH) {
    trigSafe = false;
  }
  if (trigVal == LOW && batSafe == true) {
    trigSafe = true;
  }
  if (trigSafe == true) {
    mySerial.println(throttle);
    delay(100);
  } else {
    mySerial.println("1100");
    delay(100);
  }

  //code for sending lights on off with no delay in them
  if (lightVal == LOW && lightstate == 1) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      mySerial.println("6");
      lightstate = 0;
    }
  }
  if (lightVal == LOW && lightstate == 0) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      mySerial.println("9");
      lightstate = 1;
    }
  }
  if (mySerial.available() > 0) {
    serialdata = mySerial.read();
    if (serialdata == 49) {
      digitalWrite(batlevelpin1, HIGH);
      digitalWrite(batlevelpin2, HIGH);
      digitalWrite(batlevelpin3, HIGH);
    }
    if (serialdata == 50) {
      digitalWrite(batlevelpin1, HIGH);
      digitalWrite(batlevelpin2, HIGH);
      digitalWrite(batlevelpin3, LOW);
    }
    if (serialdata == 51) {
      digitalWrite(batlevelpin2, HIGH);
      digitalWrite(batlevelpin1, LOW);
      digitalWrite(batlevelpin3, LOW);
    }
    if (serialdata == 52) {
      batSafe = false;
      digitalWrite(batlevelpin2, LOW);
      digitalWrite(batlevelpin1, HIGH);
      digitalWrite(batlevelpin3, LOW);
    }
  }

  float voltage = batlevel * (4.340 / 1023.000);
  if (voltage > 3.90) {
    ledState = HIGH;
  } else if ( voltage <= 3.90 && voltage > 3.75) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
    }
  } else {
    ledState = LOW;
    batSafe = false;
  }
  digitalWrite(batconpin1, ledState);
}
