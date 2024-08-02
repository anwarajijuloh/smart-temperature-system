#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN D3            // GPIO0 (D3)
#define DHTTYPE DHT11
#define RED_PIN D1           // GPIO5 (D1)
#define GREEN_PIN D2         // GPIO4 (D2)
#define BLUE_PIN D5          // GPIO14 (D5)
#define FANPIN D6            // GPIO12 (D6)
#define LEDPIN D7            // GPIO13 (D7)

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

DHT dht(DHTPIN, DHTTYPE);

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

  dht.begin();
}

void loop() {
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

  // Tentukan kategori kenyamanan suhu berdasarkan THI dan tampilkan ke LCD
  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("THI:");
  lcd2.print(THI);
  lcd2.write(byte(223));
  lcd2.print("C ");
  lcd2.write((uint8_t)0);
  lcd2.setCursor(0, 1);

  if (THI < 29) {
    lcd2.print("Suhu Nyaman");
    setColor(0, 255, 0);        // Warna hijau
    analogWrite(FANPIN, 85);    // Fan kecepatan rendah
    blinkLED(3000);             // Blink LED dengan delay 3 detik
  } else if (THI >= 29 && THI <= 30.5) {
    lcd2.print("Kurang Nyaman");
    setColor(255, 255, 0);      // Warna kuning
    analogWrite(FANPIN, 170);   // Fan kecepatan sedang
    blinkLED(500);              // Blink LED dengan delay 0.5 detik
  } else {
    lcd2.print("Tidak Nyaman");
    setColor(255, 0, 0);        // Warna merah
    analogWrite(FANPIN, 255);   // Fan kecepatan tinggi
    blinkLED(100);              // Blink LED dengan delay 0.1 detik
  }

  delay(2000);  // Delay 2 detik sebelum loop berikutnya
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
