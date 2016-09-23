// Add libraries
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"
#include "HttpClient.h"

unsigned int nextTime = 0;
HttpClient http;

http_header_t headers[] = {
  { "Accept" , "*/*"},
  { NULL, NULL }
};

http_request_t request;
http_response_t response;

int switchPin = D1;

Adafruit_ILI9340 display = Adafruit_ILI9340(A0, A2, A1);

void setup() {
  Serial.begin(9600);
  display.begin();
  pinMode(switchPin, INPUT_PULLUP);

  Time.zone(+1); //set timezone for bst
  display.setRotation(3); // rotate screen for landscape orientation
  display.fillScreen(ILI9340_YELLOW); // flash screen yellow to ensure screen is working

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
    // Serial.println(response.status);

    // Response Body
   	// Serial.print("Application>\tHTTP Response Body: ");
    // Serial.println(response.body);

    //Print result on the OLED screen
    printText(response.body);
}

void printText(String text) {
    display.fillScreen(ILI9340_BLACK);
    display.setCursor(0, 0);
    display.setTextColor(ILI9340_WHITE);
    display.setTextSize(3);
    display.println(text);
}
