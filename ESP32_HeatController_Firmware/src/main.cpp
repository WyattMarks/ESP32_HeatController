/*
 *      ESP32 Reflow Plate 
 *      Written by Wyatt Marks 
 *      inbox@wyattmarks.com
 */ 

#include <Arduino.h>
#include <WiFiManager.h>
#include "index.h"

//Pin Definitions
#define RELAY 32
#define THERMISTOR 33

//PID Tuning Settings
#define SAMPLE_PERIOD 300
#define SAMPLES 3
#define kP 12.0
#define kI 0.2
#define kD 3.5
#define PID_I_RANGE 50

//Other Settings
#define WEBSERVER_TIMEOUT 2000
#define VERBOSE 0








WiFiManager wm;
WebServer server(80);

// Time variables
unsigned long currentTime = millis();
unsigned long previousTime_WS = 0; 
unsigned long previousTime_TR = 0;

//PID Variables
float PID_d = 0;
float PID_i = 0;
float PID_perror = 0;

float currentTemperature = 0;
bool relayState = 0;

int PID_Control(float goal, float current) {
    float error = goal - current;

    float PID_p = kP * error; //Proportional Value

    //Calculate the Integral value (Only when within 50 degrees)
    if (error > -PID_I_RANGE && error < PID_I_RANGE) {
        PID_i = PID_i + (kI * error);
    } else {
        PID_i = 0;
    }


    float elapsedTime = (millis() - previousTime_TR) / 1000.0; 
    PID_d = kD * ( (error - PID_perror) / elapsedTime ); //Derivative value, based on rate of change

    PID_perror = error;
    return (PID_i + PID_d + PID_p) > 127; //arbitary
}

int readSample(uint8_t pin) {
    int total = 0;
    for (int i = 0; i < SAMPLES; i++) {
        total += analogRead(pin);
        delay(10); //10ms delay, idk
    }

    return total / SAMPLES;
}

float readTemperature(uint8_t pin) {
    int raw = readSample(pin);

    if (raw == 0)
        return 0;

    double voltage = ((double) raw / 4096.0) * 3.3;
    double resistance = (3300 / voltage) - 1000;

    if (VERBOSE) {
        Serial.print("Raw Analog Reading: "); Serial.println(raw);
        Serial.print("Volage Reading: "); Serial.println(voltage);
        Serial.print("Calculated Resistance: "); Serial.println(resistance);
    }

    //Below is the Steinhart-Hart equation. Points used are (25,100000) (360,50) (420, 25)
    return 1.0 / (.0030494161 - 0.000497438227*log(resistance) + 0.00002796677786 * log(resistance) * log(resistance) * log(resistance));
}

void index_request() {
    String s = landing_page_html; 
    server.send(200, "text/html", s);
}
 
void data_request() {
    String response = "{\"temperature\":";
    response += String(currentTemperature);
    response += ", \"relay\":";
    response += relayState ? "true}" : "false}";
    server.send(200, "text/plane", response); 
}

void setup() {
    WiFi.mode(WIFI_STA);

    Serial.begin(115200);
    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, LOW);

    if (wm.autoConnect("ESP32 Reflow Plate")) {
        Serial.println("Successfully connected to WiFi.");
    } else {
        Serial.println("Error connecting to WiFi.. restarting");
        ESP.restart();
    }

    server.on("/", index_request);
    server.on("/temperature", data_request);
    server.begin();
}


void loop(){
    currentTime = millis();

    server.handleClient();

    //Temperature Control
    if (currentTime - previousTime_TR >= SAMPLE_PERIOD) {
        double temp = readTemperature(THERMISTOR);
        if (VERBOSE) {
            Serial.print("Calculated Temperature: "); Serial.println(temp);
        }

        if (PID_Control(100.0, temp)) {
            digitalWrite(RELAY, HIGH);
            if (!relayState) {
                Serial.println("Relay On");
                relayState = 1;
            }
        } else {
            digitalWrite(RELAY, LOW);
            if (relayState) {
                Serial.println("Relay Off");
                relayState = 0;
            }
        }

        currentTemperature = temp;
        previousTime_TR = currentTime;
    }
}