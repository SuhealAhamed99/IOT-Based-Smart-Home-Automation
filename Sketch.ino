#define BLYNK_TEMPLATE_ID "TMPL3i1hkDMTL"
#define BLYNK_TEMPLATE_NAME "IoT Based Home Automation"
#define BLYNK_AUTH_TOKEN "A4wZ28RzQpBXvy2hcwvE2wU0DTGf1dml"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <U8g2lib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// === WiFi ===
char ssid[] = "Wokwi-GUEST";
char password[] = "";

// === Pins ===
#define DHTPIN 15
#define DHTTYPE DHT22
#define MQ2_PIN 14
#define PIR_PIN 13
#define LDR_ANALOG 36
#define LDR_DIGITAL 32
#define ROOM_LED 26
#define FAN_BTN 33
#define LIGHT_BTN 34
#define TRIG_PIN 12
#define ECHO_PIN 27
#define BUZZER_PIN 5
#define SERVO_FAN_PIN 23
#define SERVO_DOOR_PIN 22
#define RGB_R 19
#define RGB_G 18
#define RGB_B 21
#define LDR_LED 25

// === Objects ===
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo fanServo, doorServo;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);
BlynkTimer timer;

// === States ===
bool fanState = false, lightState = false;
bool lightAutoMode = false;  // NEW: Light control mode (false = manual, true = auto via LDR)
int lastFanBtn = HIGH, lastLightBtn = HIGH;
unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 50;

float temp = 0, hum = 0;
int gas = 0, ldrVal = 0, tankLevel = 0;
bool motion = false, ldrDark = false;
bool lcdToggle = false;

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  dht.begin();
  timeClient.begin();

  pinMode(MQ2_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_ANALOG, INPUT);
  pinMode(LDR_DIGITAL, INPUT);
  pinMode(ROOM_LED, OUTPUT);
  pinMode(FAN_BTN, INPUT_PULLUP);
  pinMode(LIGHT_BTN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  pinMode(LDR_LED, OUTPUT);

  fanServo.attach(SERVO_FAN_PIN);
  doorServo.attach(SERVO_DOOR_PIN);
  fanServo.write(90);  // OFF
  doorServo.write(90); // Closed

  Wire.begin(16, 17);
  lcd.init(); lcd.backlight();
  lcd.print("Smart Home Init");
  delay(1000);
  lcd.clear();
  u8g2.begin();

  timer.setInterval(1000L, updateSensors);
  timer.setInterval(2000L, updateLCD);
}

void loop() {
  Blynk.run();
  timer.run();
  timeClient.update();

  int fanReading = digitalRead(FAN_BTN);
  int lightReading = digitalRead(LIGHT_BTN);

  if (millis() - lastDebounce > debounceDelay) {
    if (fanReading == LOW && lastFanBtn == HIGH) {
      fanState = !fanState;
      fanServo.write(fanState ? 0 : 90);
      Blynk.virtualWrite(V7, fanState);
      Serial.println(fanState ? "Fan ON (via Button)" : "Fan OFF (via Button)");
      lastDebounce = millis();
    }

    if (!lightAutoMode && lightReading == LOW && lastLightBtn == HIGH) {
      lightState = !lightState;
      digitalWrite(ROOM_LED, lightState);
      Blynk.virtualWrite(V6, lightState);
      Serial.println(lightState ? "Light ON (via Button)" : "Light OFF (via Button)");
      lastDebounce = millis();
    }
  }

  lastFanBtn = fanReading;
  lastLightBtn = lightReading;
}

void updateSensors() {
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  gas = digitalRead(MQ2_PIN);
  motion = digitalRead(PIR_PIN);
  ldrVal = analogRead(LDR_ANALOG);
  ldrDark = digitalRead(LDR_DIGITAL) == LOW;

  doorServo.write(motion ? 0 : 90);
  digitalWrite(LDR_LED, ldrDark);

  if (lightAutoMode) {
    digitalWrite(ROOM_LED, ldrDark);
    Blynk.virtualWrite(V6, ldrDark);
  } else {
    digitalWrite(ROOM_LED, lightState);
  }

  if (temp > 30 && hum > 70) {
    digitalWrite(RGB_R, HIGH); digitalWrite(RGB_G, LOW); digitalWrite(RGB_B, LOW);
  } else if (temp > 30 || hum > 70) {
    digitalWrite(RGB_R, LOW); digitalWrite(RGB_G, LOW); digitalWrite(RGB_B, HIGH);
  } else {
    digitalWrite(RGB_R, LOW); digitalWrite(RGB_G, HIGH); digitalWrite(RGB_B, LOW);
  }

  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float distance = pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;
  tankLevel = constrain(map(distance, 0, 400, 100, 0), 0, 100);

  digitalWrite(BUZZER_PIN, (tankLevel >= 100 && digitalRead(RGB_R)) ? HIGH : LOW);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 12, "Smart Home");
  u8g2.drawStr(0, 30, motion ? "Door: Open" : "Door: Closed");
  u8g2.drawStr(0, 48, timeClient.getFormattedTime().c_str());
  u8g2.sendBuffer();

  Serial.printf("Temp: %.1fC | Hum: %.1f%% | Gas: %s | Motion: %s | Tank: %d%% | LDR: %d \n",
    temp, hum,
    gas ? "Safe" : "Leak",
    motion ? "Yes" : "No",
    tankLevel, ldrVal,
    (lightState || ldrDark) ? "ON" : "OFF");

  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, gas);
  Blynk.virtualWrite(V3, ldrVal);
  Blynk.virtualWrite(V4, tankLevel);
  Blynk.virtualWrite(V5, motion);
  Blynk.virtualWrite(V6, lightAutoMode ? ldrDark : lightState);
  Blynk.virtualWrite(V7, fanState);
}

void updateLCD() {
  lcd.clear();
  if (lcdToggle) {
    lcd.setCursor(0, 0);
    lcd.print("T:"); lcd.print(temp); lcd.print(" H:"); lcd.print(hum);
    lcd.setCursor(0, 1);
    lcd.print("Gas:"); lcd.print(gas ? "Safe " : "Leak!");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Tank Level:");
    lcd.setCursor(0, 1);
    lcd.print(tankLevel); lcd.print("% Full");
  }
  lcdToggle = !lcdToggle;
}

// === Blynk Virtual Pins ===
BLYNK_WRITE(V6) {
  if (!lightAutoMode) {
    lightState = param.asInt();
    digitalWrite(ROOM_LED, lightState);
    Serial.println(lightState ? "Light ON (via Blynk)" : "Light OFF (via Blynk)");
  }
}

BLYNK_WRITE(V7) {
  fanState = param.asInt();
  fanServo.write(fanState ? 0 : 90);
  Serial.println(fanState ? "Fan ON (via Blynk)" : "Fan OFF (via Blynk)");
}

BLYNK_WRITE(V8) {
  lightAutoMode = param.asInt();
  Serial.println(lightAutoMode ? "Light Control: AUTO (LDR)" : "Light Control: MANUAL (Button/Blynk)");
}
