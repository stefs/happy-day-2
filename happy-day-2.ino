#include <LiquidCrystal.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const unsigned char rs = 4;
const unsigned char en = 0;
const unsigned char d4 = 14;
const unsigned char d5 = 12;
const unsigned char d6 = 13;
const unsigned char d7 = 15;
const unsigned char led = 10;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const char *ssid = "<SSID>";
const char *password = "<PASSWORD>";

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "fritz.box", 7200, 3600000);

const float start_ = 6.0f;
const float end_ = 20.75f;
const char template_zzzz[] = "    z z z z     ";
float hours = 0.0f;
float progress = 0.0f;
byte parts = 0;
char bar[17];
byte z = 4;

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(1, 1);
  lcd.print("Connecting ...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  timeClient.begin();
}

void loop() {
  timeClient.update();

  hours = float(timeClient.getHours()) + float(timeClient.getMinutes()) / 60.0f + float(timeClient.getSeconds()) / 3600.0f;
  //hours = float(int(hours * 1000.0f * 2880.0f) % 24000) / 1000.0f;
  progress = min(max((hours - start_) / (end_ - start_), 0.0f), 1.0f);
  parts = progress * 17.0f;

  if (hours < start_ || hours > end_) {
    // line 0
    lcd.setCursor(2, 0);
    lcd.print("   ");
    lcd.print(timeClient.getFormattedTime().substring(0, 5));
    lcd.print("     ");
    // line 1
    strcpy(bar, template_zzzz);
    bar[z] = 'Z';
    switch (z) {
      case 4:
        z = 6;
        break;
      case 6:
        z = 8;
        break;
      case 8:
        z = 10;
        break;
      default:
        z = 4;
        break;
    }
    lcd.setCursor(0, 1);
    lcd.print(bar);
  } else {
    // line 0
    lcd.setCursor(2, 0);
    lcd.print(timeClient.getFormattedTime());
    lcd.print(' ');
    lcd.print(int(progress * 100.0f));
    lcd.print("%  ");
    // line 1
    lcd.setCursor(0, 1);
    for (int i = 0; i < 16; i++) {
      lcd.print(i < parts ? '#' : '-');
    }
  }

  delay(1000);
}
