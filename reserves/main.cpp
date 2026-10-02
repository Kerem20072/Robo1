#include <Arduino.h>

#define PIN_SOL_SENSOR PA0
#define PIN_SAG_SENSOR PA1
#define PIN_ON_ENGEL PA2

#define PIN_MOTOR_IN1 A0
#define PIN_MOTOR_IN2 A1
#define PIN_MOTOR_IN3 A2
#define PIN_MOTOR_IN4 A3

typedef union {
  uint8_t tum_durum;

  struct {
    uint8_t sol_sensor : 1;
    uint8_t sag_sensor : 1;
    uint8_t on_engel_sensor : 1;

    uint8_t sol_motor : 2;
    uint8_t sag_motor : 2;

    uint8_t sistem_hata : 1;
  } kayit;

} durum;

typedef enum { MOD_UYANIS, MOD_DEFAULT, MOD_DURAKLAMA, MOD_MANEVRA } MOD;

durum arac;

MOD anlik_durum = MOD_UYANIS;
MOD onceki_durum = MOD_UYANIS;

unsigned long int kronometre = 0;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_SOL_SENSOR, INPUT);
  pinMode(PIN_SAG_SENSOR, INPUT);
  pinMode(PIN_ON_ENGEL, INPUT);

  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);
  pinMode(PIN_MOTOR_IN3, OUTPUT);
  pinMode(PIN_MOTOR_IN4, OUTPUT);
}

void loop() {
  arac.kayit.sag_sensor = digitalRead(PIN_SAG_SENSOR);
  arac.kayit.sol_sensor = digitalRead(PIN_SOL_SENSOR);
  arac.kayit.on_engel_sensor = digitalRead(PIN_ON_ENGEL);

  switch (anlik_durum) {
  case MOD_UYANIS:
    arac.kayit.sag_motor = 0;
    arac.kayit.sol_motor = 0;

    if (millis() - kronometre >= 3000) {
      onceki_durum = anlik_durum;
      anlik_durum = MOD_DEFAULT;
      kronometre = millis();
    }

    break;

  case MOD_DEFAULT:

    if (arac.kayit.sag_sensor == 1 && arac.kayit.sol_sensor == 1) {
      arac.kayit.sag_motor = 1;
      arac.kayit.sol_motor = 1;
    }

    if (arac.kayit.on_engel_sensor == 1) {
      anlik_durum = MOD_DURAKLAMA;
      kronometre = millis();
    }

    if (arac.kayit.sag_sensor == 0 && arac.kayit.sol_sensor == 1) {
      arac.kayit.sag_motor = 2;
      arac.kayit.sol_motor = 1;
    }

    if (arac.kayit.sag_sensor == 1 && arac.kayit.sol_sensor == 0) {
      arac.kayit.sag_motor = 1;
      arac.kayit.sol_motor = 2;
    }

    if (arac.kayit.sag_sensor == 0 && arac.kayit.sol_sensor == 0) {
      arac.kayit.sistem_hata = 1;
    }

    break;

  case MOD_DURAKLAMA:
    arac.kayit.sag_motor = 0;
    arac.kayit.sol_motor = 0;

    if (millis() - kronometre >= 2000) {
      anlik_durum = MOD_MANEVRA;
      kronometre = millis();
    }
    break;

  case MOD_MANEVRA:

    arac.kayit.sol_motor = 1;
    arac.kayit.sag_motor = 2;

    if (millis() - kronometre >= 1000) {
      if (arac.kayit.sag_sensor == 1 || arac.kayit.sol_sensor == 1) {
        onceki_durum = anlik_durum;
        anlik_durum = MOD_DEFAULT;
      }
    }

    break;
  }

  if (arac.kayit.sistem_hata == 0) {
    if (arac.kayit.sol_motor == 1) {
      digitalWrite(PIN_MOTOR_IN1, HIGH);
      digitalWrite(PIN_MOTOR_IN2, LOW);
    } else if (arac.kayit.sol_motor == 2) {
      digitalWrite(PIN_MOTOR_IN1, LOW);
      digitalWrite(PIN_MOTOR_IN2, LOW);
    }
    else{
      digitalWrite(PIN_MOTOR_IN1, LOW);
      digitalWrite(PIN_MOTOR_IN2, LOW);
    }

    if (arac.kayit.sag_motor == 1) {
      digitalWrite(PIN_MOTOR_IN3, HIGH);
      digitalWrite(PIN_MOTOR_IN4, LOW);
    } else if (arac.kayit.sag_motor == 2) {
      digitalWrite(PIN_MOTOR_IN3, LOW);
      digitalWrite(PIN_MOTOR_IN4, LOW);
    }
    else {
      digitalWrite(PIN_MOTOR_IN3, LOW);
      digitalWrite(PIN_MOTOR_IN4, LOW);
    }
  } else {
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, LOW);
    digitalWrite(PIN_MOTOR_IN3, LOW);
    digitalWrite(PIN_MOTOR_IN4, LOW);
  }
}