#include <Arduino.h>

const int IN1 = 8;
const int IN2 = 7;
const int ENA = 9;

const int IN3 = 5;
const int IN4 = 4;
const int ENB = 10;

const int solSensor = 2;
const int sagSensor = 3;

const int normalHiz = 120;
const int donusHizi = 80;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(solSensor, INPUT);
  pinMode(sagSensor, INPUT);
}

void ileri() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, normalHiz);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, normalHiz);
}

void sagaDon() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, donusHizi);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW); 
  analogWrite(ENB, 0);
}

void solaDon() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, donusHizi);
}

void dur() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void loop() {
  int solDurum = digitalRead(solSensor);
  int sagDurum = digitalRead(sagSensor);

  if (solDurum == 0 && sagDurum == 0) {
    ileri();
  } else if (solDurum == 0 && sagDurum == 1) {
    sagaDon();
  } else if (solDurum == 1 && sagDurum == 0) {
    solaDon();
  } else if (solDurum == 1 && sagDurum == 1) {
    dur();
  }
}