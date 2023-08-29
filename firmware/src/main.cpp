#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_1_HW_I2C oled(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

const char *ssid = "SSID";
const char *password = "PASSWORD";

float heater_bed;
float heater_bed_target;
float extruder;
float extruder_target;

const unsigned char nozzle[] PROGMEM = {
    0x00,
    0x00,
    0x00,
    0x00,
    0xE0,
    0x07,
    0x00,
    0x00,
    0xE0,
    0x07,
    0x00,
    0x00,
    0xE0,
    0x07,
    0xE0,
    0x07,
    0xFC,
    0x3F,
    0xFC,
    0x3F,
    0xFC,
    0x3F,
    0xF8,
    0x1F,
    0xF0,
    0x0F,
    0xC0,
    0x03,
    0x80,
    0x01,
    0x00,
    0x00,
};

const unsigned char bed[] PROGMEM = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x20,
    0x09,
    0x20,
    0x09,
    0x40,
    0x12,
    0x40,
    0x12,
    0x20,
    0x09,
    0x20,
    0x09,
    0x00,
    0x00,
    0xE0,
    0x3F,
    0x10,
    0x20,
    0x08,
    0x10,
    0x04,
    0x08,
    0xFC,
    0x07,
    0x00,
    0x00,
    0x00,
    0x00,
};

void setup(void)
{
  oled.begin();

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to wifi...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnected successfully");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

int menuDownOffset = 18;

void loop(void)
{
  // Serial.println(extruderTemp);
  oled.firstPage();
  do
  {
    oled.drawXBMP(0, 8, 16, 16, nozzle);
    oled.setFont(u8g2_font_ncenB10_tr);
    oled.setCursor(18, 22);
    oled.println(String(extruder) + " / " + String(extruder_target));

    oled.drawXBMP(0, 8 + menuDownOffset, 16, 16, bed);
    oled.setFont(u8g2_font_ncenB10_tr);
    oled.setCursor(18, 22 + menuDownOffset);
    oled.println(String(heater_bed) + " / " + String(heater_bed_target));
  } while (oled.nextPage());

  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient client;
    client.begin("http://ender.local:3000/temps");
    int httpCode = client.GET();
    if (httpCode > 0)
    {
      String payload = client.getString();
      char json[300];
      payload.replace("\n", "");
      payload.replace(" ", "");
      payload.trim();
      // payload.remove(0, 1);
      payload.toCharArray(json, 300);
      // Serial.println(json);

      StaticJsonDocument<300> doc;
      deserializeJson(doc, json);

      heater_bed = doc["bed"]["temp"];
      heater_bed_target = doc["bed"]["target"];
      extruder = doc["extruder"]["temp"];
      extruder_target = doc["extruder"]["target"];
    }
    else
    {
      Serial.println("Error");
    }
  }
  else
  {
    WiFi.reconnect();
  }
}