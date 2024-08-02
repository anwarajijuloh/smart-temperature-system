#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <DHT.h>

#define DHTPIN 3
#define DHTTYPE DHT11
#define PIN_RECEIVER 2
#define RED_PIN 10
#define GREEN_PIN 11
#define BLUE_PIN 12
#define FANPIN 9
#define LEDPIN 6

// const int ledPin = 9;
LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x26, 16, 2);

byte fanChar[8] = {
  B01110,
  B11011,
  B10101,
  B11011,
  B01110,
  B00100,
  B00100,
  B01110
};
byte level1[8] = { B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111 };
byte level2[8] = { B00000, B00000, B00000, B00000, B11111, B11111, B11111, B11111 };
byte level3[8] = { B00000, B00000, B11111, B11111, B11111, B11111, B11111, B11111 };

unsigned int address;
uint8_t command;
void setup() {
  lcd1.init();
  lcd1.backlight();
  lcd2.init();
  lcd2.backlight();

  lcd2.createChar(0, fanChar);
  lcd2.createChar(1, level1);
  lcd2.createChar(2, level2);
  lcd2.createChar(3, level3);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
}

void loop() {
  DHT dht(DHTPIN, DHTTYPE);
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  if (isnan(temp) || isnan(humi)) {
    lcd1.setCursor(0, 0);
    lcd1.print("Error reading");
    lcd1.setCursor(0, 1);
    lcd1.print("sensor data");
    return;
  }

  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("TEMP: ");
  lcd1.print(temp);
  lcd1.write(byte(223));
  lcd1.print("C");
  lcd1.setCursor(0, 1);
  lcd1.print("HUMI: ");
  lcd1.print(humi);
  lcd1.print("%");

  // Rumus index kenyamanan menggunakan persamaan dari Nieuwolt
  float THI = 0.8 * temp + (humi * temp) / 500;

  // Tentukan kategori kenyamanan suhu berdasarkan THI dan tampilkan kedalam LCD

  if (THI < 29) {
    lcd2.clear();
    lcd2.setCursor(0, 0);
    lcd2.print("THI:");
    lcd2.print(THI);
    lcd2.write(byte(223));
    lcd2.print("C ");
    lcd2.write((uint8_t)0);
    lcd2.write((uint8_t)1);
    lcd2.setCursor(0, 1);
    lcd2.print("Suhu Nyaman");
    setColor(0, 255, 0);
    analogWrite(FANPIN, 85);
    blinkLED(3000);
  } else if (THI >= 29 && THI <= 30.5) {
    lcd2.clear();
    lcd2.setCursor(0, 0);
    lcd2.print("THI:");
    lcd2.print(THI);
    lcd2.write(byte(223));
    lcd2.print("C ");
    lcd2.write((uint8_t)0);
    lcd2.write((uint8_t)1);
    lcd2.write((uint8_t)2);
    lcd2.setCursor(0, 1);
    lcd2.print("Kurang Nyaman");
    setColor(255, 255, 0);
    analogWrite(FANPIN, 170);
    blinkLED(500);
  } else {
    lcd2.clear();
    lcd2.setCursor(0, 0);
    lcd2.print("THI:");
    lcd2.print(THI);
    lcd2.write(byte(223));
    lcd2.print("C ");
    lcd2.write((uint8_t)0);
    lcd2.write((uint8_t)1);
    lcd2.write((uint8_t)2);
    lcd2.write((uint8_t)3);
    lcd2.setCursor(0, 1);
    lcd2.print("Tidak Nyaman");
    setColor(255, 0, 0);
    analogWrite(FANPIN, 255);
    blinkLED(100);
  }


  delay(2000);
}

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void blinkLED(int delayTime) {
  digitalWrite(LEDPIN, HIGH);
  delay(delayTime / 4);
  digitalWrite(LEDPIN, LOW);
  delay(delayTime / 2);
}