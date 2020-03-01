#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_BME280.h>
#include <Adafruit_TCS34725.h>

#define SS_PIN 10
#define RST_PIN 9

#define LED_PIN 3

#define PING_TRIG_PIN A1
#define PING_ECHO_PIN A0

#define IR_D_PIN A3
#define IR_A_PIN A2

MFRC522 rfid_sens(SS_PIN, RST_PIN);
Adafruit_BME280 TPH_sens;
float temp_c, pressure_pa, humidity_percent;
Adafruit_TCS34725 RGB_sens;
uint16_t red, green, blue, clear_light;

float ping_dist_cm;
uint16_t ir_proximity;

uint32_t count;

bool card_present; 

bool is_card_present(MFRC522 tag)
{
  bool test = tag.PICC_IsNewCardPresent();
  test = (tag.PICC_IsNewCardPresent() || test);
  return test;
}

float ping_get_distance_cm()
{
  long echo_time;
  float distance_cm;
  digitalWrite(PING_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(PING_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(PING_TRIG_PIN, LOW);

  echo_time = pulseIn(PING_ECHO_PIN, HIGH, 10000);
  distance_cm = (echo_time * 0.0343) / 2.0;
  return distance_cm;
}

uint16_t ir_get_proximity()
{
  if (digitalRead(IR_D_PIN) == HIGH)
  {
    return analogRead(IR_A_PIN);
  }
  else
  {
    return 0;
  }
}

void setup() {
  unsigned int TBH_status, RGB_status;
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  SPI.begin();
  rfid_sens.PCD_Init();
  delay(10);

  TBH_status = TPH_sens.begin();
  RGB_status = RGB_sens.begin();

  pinMode(PING_TRIG_PIN, OUTPUT);
  pinMode(PING_ECHO_PIN, INPUT);
}

void loop() {
  temp_c = TPH_sens.readTemperature();
  pressure_pa = TPH_sens.readPressure();
  humidity_percent = TPH_sens.readHumidity();

  RGB_sens.getRawData(&red, &green, &blue, &clear_light);

  if (is_card_present(rfid_sens))
  {
    card_present = true;
    //Serial.println("Card present..");
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    card_present = false;
    digitalWrite(LED_PIN, LOW);
  }

  ping_dist_cm = ping_get_distance_cm();
  ir_proximity = ir_get_proximity();

  //Serial.print(count);
  //Serial.print(" - ");
  Serial.print(temp_c);
  Serial.print(",");
  Serial.print(pressure_pa);
  Serial.print(",");
  Serial.print(humidity_percent);
  //Serial.print(", R: ");
  Serial.print(",");
  Serial.print(red);
  //Serial.print(", G: ");
  Serial.print(",");
  Serial.print(green);
  //Serial.print(", B: ");
  Serial.print(",");
  Serial.print(blue);
  Serial.print(",");
  //Serial.print(", C: ");
  Serial.print(clear_light);
  //Serial.print(", RFID: ");
  Serial.print(",");
  Serial.print(card_present);
  //Serial.print(", PING: ");
  Serial.print(",");
  Serial.print(ping_dist_cm);
  //Serial.print(", IR: ");
  Serial.print(",");
  Serial.print(ir_proximity);
  Serial.println(";");
  count++;
  delay(50);
}
