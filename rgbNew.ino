#define FASTLED_ESP8266_D1_PIN_ORDER

#include "FastLED.h"
#define NUM_LEDS 60
#define DATA_PIN 6
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char* ssid = "WLAN557957";
const char* password = "alaska00";
CRGB leds[NUM_LEDS];
ESP8266WebServer server(80);



int fadeAmount = 5;
int type = 10;

struct RGBStruct {
   int r;
   int g;
   int b;      
};

class Moving{
  public:
    void setRGB(RGBStruct rgbVals){
      rgbMoving.r = rgbVals.r;
      rgbMoving.g = rgbVals.g;
      rgbMoving.b = rgbVals.b;
    }

    void beat(){
      for(int i = 0; i < NUM_LEDS; i++ ){
        leds[i].setRGB(rgbMoving.r,rgbMoving.g,rgbMoving.b);
        leds[i].fadeLightBy(brightness);
      }
      FastLED.show();
    
      brightness = brightness + fadeAmount;
      if(brightness == 0 || brightness == 255)
      {
        fadeAmount = -fadeAmount ;
      }  
      delay(20);
   }

  void backForth(){
    if (forth == true){
      for(int i = 0; i < NUM_LEDS; i++ ){
          if (bfLEDs[0] == i){
            leds[i].setRGB(rgbMoving.r,rgbMoving.g,rgbMoving.b);
          } else if (bfLEDs[1] == i){
            leds[i].setRGB(rgbMoving.r,rgbMoving.g,rgbMoving.b);
          } else if (bfLEDs[2] == i){
            leds[i].setRGB(rgbMoving.r,rgbMoving.g,rgbMoving.b);
          } else {
            leds[i].setRGB(0,0,0);
          }
      }
      FastLED.show();
      bfLEDs[0] += 1;
      bfLEDs[1] += 1;
      bfLEDs[2] += 1;
      if (bfLEDs[2] == NUM_LEDS){
        forth = false;
      }
    } else {
        for(int i = NUM_LEDS; i > 0; i-- ){
          if (bfLEDs[0] == i){
            leds[i].setRGB(rgbMoving.r,rgbMoving.g,rgbMoving.b);
          } else if (bfLEDs[1] == i){
            leds[i].setRGB(rgbMoving.r,rgbMoving.g,rgbMoving.b);
          } else if (bfLEDs[2] == i){
            leds[i].setRGB(rgbMoving.r,rgbMoving.g,rgbMoving.b);
          } else {
            leds[i].setRGB(0,0,0);
          }
      }
      FastLED.show();
      bfLEDs[0] -= 1;
      bfLEDs[1] -= 1;
      bfLEDs[2] -= 1;
      if (bfLEDs[2] == 0){
        forth = true;
      }
    }
    delay(100);
  }

  void loopColor(){
    for(int i = 0; i < NUM_LEDS; i++ ){
          if (bfLEDs[0] == i){
            leds[i].setRGB(rgbMoving.r,rgbMoving.g,rgbMoving.b);
          } else if (bfLEDs[1] == i){
            leds[i].setRGB(rgbMoving.r,rgbMoving.g,rgbMoving.b);
          } else if (bfLEDs[2] == i){
            leds[i].setRGB(rgbMoving.r,rgbMoving.g,rgbMoving.b);
          } else {
            leds[i].setRGB(0,0,0);
          }
    }
    FastLED.show();
    bfLEDs[0] += 1;
    bfLEDs[1] += 1;
    bfLEDs[2] += 1;
    if (bfLEDs[0] == NUM_LEDS){
      bfLEDs[0] = 0;
    } else if (bfLEDs[1] == NUM_LEDS){
      bfLEDs[1] = 0;
    } else if (bfLEDs[2] == NUM_LEDS){
      bfLEDs[2] = 0;
    }
    delay(100);
  }

  void rainbow(){
    int hue = 0;
    int color_dif;

    color_dif = 360 / NUM_LEDS;
    
    for (int i = 0; i < NUM_LEDS; i++) {
        if (times > 60){
          times = 0;
        }
        hue = (i + times) * color_dif;
        if (hue >= 360){
          hue = hue - 360;  
        }
        leds[i] = CHSV(hue, 255, 255);
      }
      times++;
      delay(50);
      FastLED.show();
  }

  private: 
    RGBStruct rgbMoving;
    int times = 0;
    int brightness;
    int bfLEDs [3] = {0,1,2};
    bool forth = true;
};
  
Moving moving;

void staticColor(RGBStruct rgbS){
  for(int i = 0; i < NUM_LEDS; i++ ){
        leds[i].setRGB(rgbS.r,rgbS.g,rgbS.b);
  }
  FastLED.show();
  return;
}

void staticRainbow(){
   int hue = 0;
   for (int i = 0; i < NUM_LEDS; i++) {
        hue = i * NUM_LEDS / 6;
        leds[i] = CHSV(hue, 255, 255);
      }
  FastLED.show();
  return;
}

void randomColor(){
  for (int i = 0; i < NUM_LEDS; i++){
      leds[i].setRGB(random(255), random(255), random(255));
  }
  FastLED.show();
  return;
}

void off(){
  FastLED.clear();  
  FastLED.show();
}
 
void typeSwitch() {
    RGBStruct rgbS;
    
    type = server.arg("type").toInt();
    rgbS.r = server.arg("r").toInt();
    rgbS.g = server.arg("g").toInt();
    rgbS.b = server.arg("b").toInt();

    moving.setRGB(rgbS);
    
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "Ok.");
    
    switch (type) {
      case 0: staticColor(rgbS);
              break; 
      case 5: staticRainbow();
              break;
      case 6: randomColor();
              break;
      case 7: off();
              break;
    }
}

void restServerRouting() {
    server.on(F("/"), HTTP_GET, typeSwitch);
}


IPAddress local_IP(192, 168, 2, 20);

IPAddress gateway(192, 168, 2, 1);

IPAddress subnet(255, 255, 255, 0);

void setup() {
 Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  Serial.println("");
 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  restServerRouting();
  server.begin();
  Serial.println("HTTP server started");

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  Serial.println("Strip added");
  Moving * moving = new Moving();
}

void loop() {
  server.handleClient();
  switch (type) {
      case 1: moving.beat();
              break; 
      case 2: moving.backForth();
              break;
      case 3: moving.loopColor();
              break;
      case 4: moving.rainbow();
              break;
      default: break;
  }
    
}
