#pragma once

#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#include "Types.h"

#define DHT_TYPE DHT22

namespace SerialCom {
    constexpr static const uint8_t SDS_TX_PIN = 0; // D3 on Wemos D1 Mini -> SDS011 TX pin
    constexpr static const uint8_t SDS_RX_PIN = 2; // D4 on Wemos D1 Mini -> SDS011 RX pin
    constexpr static const uint8_t DHT_PIN = 5;    // D1 on Wemos D1 Mini -> DHT22 Data Pin

    SoftwareSerial sensorSerial(SDS_RX_PIN, SDS_TX_PIN);
    SdsDustSensor sds(sensorSerial); //  additional parameters: retryDelayMs and maxRetriesNotAvailable
    const int MINUTE = 60000;
    const int WAKEUP_WORKING_TIME = 30000; // 30 seconds.
    const int MEASUREMENT_INTERVAL = MINUTE;
    const int CYCLE_MINUTES = 10;

    DHT dht(DHT_PIN, DHT_TYPE);
        
    void setup() {
      Serial.begin(115200);
      delay(5000);
      /* SDS011 Dust Sensor */
      sds.begin();
      dht.begin();
      delay(5000);
      // Prints SDS011 firmware version:
      Serial.println("SDS011 Initialize");
      Serial.println(sds.queryFirmwareVersion().toString());
      // Ensures SDS011 is in 'query' reporting mode:
      Serial.println(sds.setQueryReportingMode().toString());
      WorkingPeriodResult workingPeriodResult = sds.setCustomWorkingPeriod(CYCLE_MINUTES);
      Serial.print("SDS011 Working Period Configuration: ");
      Serial.println(workingPeriodResult.toString()); 
    }

    void collectParticles(particleSensorState_t& state) {
        // Wake up SDS011
        sds.wakeup();
        delay(WAKEUP_WORKING_TIME);
        // Get data from SDS011
        PmResult pm = sds.queryPm();
        if (pm.isOk()) {
          state.pm25 = pm.pm25;
          state.pm10 = pm.pm10;
          state.valid = true;
        } else {
          state.valid = false;
          Serial.print("Could not read values from sensor, reason: ");
          Serial.println(pm.statusToString());
        }
        // Put SDS011 back to sleep
        WorkingStateResult workingStateResult = sds.sleep();
        if (workingStateResult.isWorking()) {
          Serial.println("Problem with sleeping the SDS011 sensor.");
        } else {
          Serial.println("SDS011 sensor is now sleeping");
        }
    }

    void collectTemp(particleSensorState_t& state) { 
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      float hif = dht.computeHeatIndex(t, h);
      state.temp = t;
      state.humidity = h;
      state.heatIndex = hif;
    }
} // namespace SerialCom
