// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "HttpClient.h"

// use hardware SPI
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

unsigned int nextTime = 0;
HttpClient http;

http_header_t headers[] = {
  { "Accept" , "*/*"},
  { NULL, NULL }
};

http_request_t request;
http_response_t response;

int switchPin = D1;

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC);
  pinMode(switchPin, INPUT_PULLUP);

  Time.zone(+1); //set timezone for bst

  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer

  getTrainTime(); //display nextTrainTime on startup to make sure its working correctly, then clear the display
}


void loop() {
  if(Time.weekday() >= 2 && Time.weekday() <= 6) { //check to make sure it is a weekday
    if(Time.hour() == 8) { // check if the hour is 8:XX AM
      if (nextTime > millis()) {
        return;
      } else {
        getTrainTime();
        nextTime = millis() + 180000;
      }
    }
  }

  if(digitalRead(switchPin) == LOW) {
    getTrainTime();
  }
}

void getTrainTime() {
  // GET request
  request.hostname = "floating-ravine-80137.herokuapp.com";
  request.port = 80;
  request.path = "/Sandymount/Northbound/30";
  http.get(request, response, headers);

  // Response Status
  // Serial.print("Application>\tResponse status: ");
  // Serial.println(response.status);`

  // Response Body
  // Serial.print("Application>\tHTTP Response Body: ");
  // Serial.println(response.body);

  //Print result on the OLED screen
  printText(response.body);
}

void printText(String text) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println(text);
  display.display();
}
