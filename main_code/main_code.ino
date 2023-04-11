#include <Wire.h>

/***********************************************/
// Pulse Oximeter Set Up:
/***********************************************/
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     1000

// Create a PulseOximeter object
PulseOximeter pox;

// Time at which the last beat occurred
uint32_t tsLastReport = 0;


/***********************************************/
// Temperature Sensor Set Up:
/***********************************************/
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();


/***********************************************/
// GSR Sensor Set Up:
/***********************************************/
const int GSR=A0;
int sensorValue=0;
int gsr_average=0;


// Arduino Board Set up
void setup() {
    Serial.begin(9600);
    while (!Serial);
  
    // Initialize pulse oximeter sensor
    Serial.print("Initializing pulse oximeter..");
    if (!pox.begin()) {
        Serial.println("FAILED");
    for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    // Configure sensor to use 7.6mA for LED drive
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Initialize temperature sensor
	if (!mlx.begin()) {
		Serial.println("Error connecting to temperature sensor. Check wiring.");
		while (1);
	};

}

void getPulseOxSensorInfo() {
   // Read from the sensor
  pox.update();

  // Grab the updated heart rate and SpO2 levels
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      Serial.print("Heart rate:");
      Serial.print(pox.getHeartRate());
      Serial.print("bpm / SpO2:");
      Serial.print(pox.getSpO2());
      Serial.println("%");
      tsLastReport = millis();
  }
}

void getGsrSensorInfo() {
    long sum=0;
    for(int i=0;i<10;i++)           //Average the 10 measurements to remove the glitch
    {
        sensorValue=analogRead(GSR);
        sum += sensorValue;
        delay(5);
    }
    gsr_average = sum/10;
    Serial.println(gsr_average);
}

void getTemperatureSensorInfo() {
    Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC());
	Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
	Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF());
	Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");
	Serial.println();
}

void loop() {
  getPulseOxSensorInfo();
  getGsrSensorInfo();
  getTemperatureSensorInfo();

  delay(500);
}