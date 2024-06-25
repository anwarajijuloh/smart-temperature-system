// #include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

// #define DHTPIN 2
// #define DHTTYPE DHT11
// #define LED_YELLOW 5
// #define LED_RED 6
#define PIN_RECEIVER 2

IRrecv receiver(PIN_RECEIVER);

LiquidCrystal_I2C lcd(0x27, 16, 2);
decode_results results;
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  // pinMode(LED_YELLOW, OUTPUT);
  // pinMode(LED_RED, OUTPUT);
  lcd.print("PRESS BUTTON");
  receiver.enableIRIn();
}

void loop() {
  // DHT dht(DHTPIN, DHTTYPE);
  // float humidity = dht.readHumidity();
  // float temperature = dht.readTemperature();

  // if (isnan(humidity) || isnan(temperature)) {
  //   Serial.println("Gagal membaca data! $humidity");
  //   return;
  // }

  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("Suhu Ru: ");
  // lcd.print(temperature);
  // lcd.print(" C");
  // lcd.setCursor(0, 1);
  // lcd.print("Lembab: ");
  // lcd.print(humidity);
  // lcd.print("%");

  // if (temperature > 30) {
  //   digitalWrite(LED_RED, HIGH);
  //   digitalWrite(LED_YELLOW, LOW);
  // } else {
  //   digitalWrite(LED_RED, LOW);
  //   digitalWrite(LED_YELLOW, HIGH);
  // }

  if (receiver.decode(&results)) {
    Serial.println(results.value);
    translateIR();
    receiver.resume();  // Receive the next value
  }

  // delay(2000);
}

void lcdPrint(char* text)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("button pressed:");
  lcd.setCursor(0, 1);
  lcd.print(text);
  // lcd.print(" code: ");
  // lcd.print(results.value);
}

void translateIR()
{
  // Takes command based on IR code received
  switch (results.value) {
    case 2211893762:
      lcdPrint("POWER OFF");
      break;
    case 3382614754:
      lcdPrint("POWER ON");
      break;
    case 3831649278:
      lcdPrint("AUTO");
      break;
    case 3584192114:
      lcdPrint("KIPAS");
      break;
    case 2478141394:
      lcdPrint("KURKEL");
      break;
    case 2826608453:
      lcdPrint("COOLER");
      break;
    case 1854301516:
      lcdPrint("18C");
      break;
    case 2660893317:
      lcdPrint("19C");
      break;
    case 313148098:
      lcdPrint("20C");
      break;
    case 1489647367:
      lcdPrint("21C");
      break;
    case 1958633924:
      lcdPrint("22C");
      break;
    case 2125963091:
      lcdPrint("23C");
      break;
    case 3693790928:
      lcdPrint("24C");
      break;
    case 1837556553:
      lcdPrint("25C");
      break;
    case 2306543110:
      lcdPrint("26C");
      break;
    case 3653377443:
      lcdPrint("27C");
      break;
    case 589813784:
      lcdPrint("28C");
      break;
    case 2408390481:
      lcdPrint("29C");
      break;
    case 2877377038:
      lcdPrint("30C");
      break;
    default:
      lcd.clear();
      lcd.print(receiver.decodedIRData.command);
      lcd.print(" other button");
  }
}

