#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_BME280.h>
#include <Adafruit_TCS34725.h>

#define SS_PIN 10
#define RST_PIN 9

#define LED_PIN 3


MFRC522 rfid_sens(SS_PIN, RST_PIN);
Adafruit_BME280 TPH_sens;
float temp_c, pressure_pa, humidity_percent;
Adafruit_TCS34725 RGB_sens;
uint16_t red, green, blue, clear_light;

uint32_t count;

bool is_card_present(MFRC522 tag)
{
  bool test = tag.PICC_IsNewCardPresent();
  test = (tag.PICC_IsNewCardPresent() || test);
  return test;
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
}

void loop() {
  temp_c = TPH_sens.readTemperature();
  pressure_pa = TPH_sens.readPressure();
  humidity_percent = TPH_sens.readHumidity();

  RGB_sens.getRawData(&red, &green, &blue, &clear_light);

  if (is_card_present(rfid_sens))
  {
    Serial.println("Card present..");
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
  Serial.print(count);
  Serial.print(" - ");
  Serial.print(temp_c);
  Serial.print(", ");
  Serial.print(pressure_pa);
  Serial.print(", ");
  Serial.print(humidity_percent);
  Serial.print(", R: ");
  Serial.print(red);
  Serial.print(", G: ");
  Serial.print(green);
  Serial.print(", B: ");
  Serial.print(blue);
  Serial.print(", C: ");
  Serial.print(clear_light);
  Serial.println("");
  count++;
  delay(50);
}
