
/**
   @author xose.perez@gmail.com
   @brief Reads and sends values from a BME680 sensor in CayenneLPP format
   @version 1.0
   @date 2022-09-06
   @copyright Copyright (c) 2022
**/

// ----------------------------------------------------------------------------
// Dependencies
// ----------------------------------------------------------------------------

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "CayenneLPP.h"
#include <LoRaWan-RAK4630.h>
#include "keys.h"

// ----------------------------------------------------------------------------
// Configuration
// ----------------------------------------------------------------------------

// Uncomment to show debug messages
//#define DEBUG

// Uncomment if you don't have the actual sensor 
// the program will mock the values
//#define SENSOR_MOCK

// How often to send data in milliseconds,
// mind regulations and fair use practices here
#define SEND_EVERY                      20000

// LoRaWAN configuration
#define LORAWAN_DATARATE                DR_3
#define LORAWAN_ADR                     LORAWAN_ADR_OFF
#define LORAWAN_REGION                  LORAMAC_REGION_EU868
#define LORAWAN_TX_POWER                TX_POWER_15
#define JOINREQ_NBTRIALS                3
#define LORAWAN_CLASS                   CLASS_A
#define LORAWAN_PORT                    1
#define LORAWAN_CONFIRM                 LMH_UNCONFIRMED_MSG

// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------

CayenneLPP payload(51);
uint32_t last_block = 9999;

// ----------------------------------------------------------------------------
// Sensor
// ----------------------------------------------------------------------------

#ifdef SENSOR_MOCK

void setupSensor() { return; }
bool sensorRead() { return true; }
float sensorTemperature() { return random(100, 300) / 10.0; }
float sensorPressure() { return random(9800, 10200) / 10.0; }
float sensorHumidity() { return random(40, 80); }
float sensorGasResistance() { return random(0, 2000); }

#else

Adafruit_BME680 bme;

void setupSensor() {

    if (!bme.begin(0x76)) {
        #ifdef DEBUG
            Serial.println("Could not find a valid BME680 sensor, check wiring!");
        #endif
        while (1);
    }

    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms

}

bool sensorRead() {
    return bme.performReading();
}

float sensorTemperature() {
    return bme.temperature;
}

float sensorPressure() {
    return bme.pressure / 100.0;
}

float sensorHumidity() {
    return bme.humidity;
}

float sensorGasResistance() {
    return bme.gas_resistance;
}

#endif

// ----------------------------------------------------------------------------
// LoraWAN
// ----------------------------------------------------------------------------

void lorawan_rx_handler(lmh_app_data_t * data) {

    #ifdef DEBUG
        Serial.printf(
            "[LORA] LoRa Packet received on port %d, size:%d, rssi:%d, snr:%d, data:%s\n",
            data->port, data->buffsize, data->rssi, data->snr, data->buffer
        );
    #endif

}

/**
 * @brief LoRa function for handling HasJoined event.
 */
void lorawan_has_joined_handler(void) {

    lmh_error_status ret = lmh_class_request((DeviceClass_t) LORAWAN_CLASS);
    if (ret == LMH_SUCCESS) {

        #ifdef DEBUG
            Serial.println("[LORA] LoRaWAN Joined");
        #endif

    }

}

static void lorawan_join_failed_handler(void) {
    #ifdef DEBUG
        Serial.println("[LORA] OTAA join failed!");
        Serial.println("[LORA] Check your EUI's and Keys's!");
        Serial.println("[LORA] Check if a Gateway is in range!");
    #endif
}

void lorawan_confirm_class_handler(DeviceClass_t Class) {

    #ifdef DEBUG
        Serial.printf("[LORA] Switch to class %c done\n", "ABC"[Class]);
    #endif

    // Inform the server that switch has occurred ASAP
    lmh_app_data_t m_lora_app_data = {nullptr, 0, LORAWAN_PORT, 0, 0};
    lmh_send(&m_lora_app_data, LORAWAN_CONFIRM);

}

static lmh_callback_t lora_callbacks = {
    BoardGetBatteryLevel, BoardGetUniqueId, BoardGetRandomSeed,
        lorawan_rx_handler, lorawan_has_joined_handler, lorawan_confirm_class_handler, lorawan_join_failed_handler
};

bool lorawanSetup() {

    lora_rak4630_init();
    
    // Setup the EUIs and Keys
    unsigned char deveui[] = LORAWAN_LOCAL_DEVEUI;
    unsigned char appeui[] = LORAWAN_LOCAL_APPEUI;
    unsigned char appkey[] = LORAWAN_LOCAL_APPKEY;
    lmh_setDevEui(deveui);
    lmh_setAppEui(appeui);
    lmh_setAppKey(appkey);

    // Init structure
    lmh_param_t lora_param_init = {
        LORAWAN_ADR, LORAWAN_DATARATE, LORAWAN_PUBLIC_NETWORK, JOINREQ_NBTRIALS, LORAWAN_TX_POWER, LORAWAN_DUTYCYCLE_OFF
    };

    // Initialize LoRaWan
    unsigned long err_code = lmh_init(&lora_callbacks, lora_param_init, true, (DeviceClass_t) LORAWAN_CLASS, (LoRaMacRegion_t) LORAWAN_REGION);
    #ifdef DEBUG
        if (err_code != 0) {
            Serial.printf("[LORA] LoRa init failed with error: %d\n", err_code);
        } else {
            unsigned char deveui[] = LORAWAN_LOCAL_DEVEUI;
            Serial.printf("[LORA] Device EUI: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n",
                deveui[0], deveui[1], deveui[2], deveui[3],
                deveui[4], deveui[5], deveui[6], deveui[7]
            );
        }
    #endif

    // Start Join procedure
    if (err_code == 0) {
        lmh_join();
    }

    return (err_code == 0);

}

bool lorawanSend(uint8_t * data, uint8_t len) {

    if (lmh_join_status_get() != LMH_SET) {
        //Not joined, try again later
        return false;
    }

    #ifdef DEBUG
        Serial.print("[LORA] Sending frame: ");
        for (unsigned char i=0; i<len; i++) {
            Serial.printf("%02X", data[i]);
        }
        Serial.println();
    #endif

    // Build message structure
    lmh_app_data_t m_lora_app_data = {data, len, LORAWAN_PORT, 0, 0};
    lmh_error_status error = lmh_send(&m_lora_app_data, LORAWAN_CONFIRM);

    return (error == LMH_SUCCESS);

}

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------

void send() {

    if (! sensorRead()) {
        #ifdef DEBUG
            Serial.println("Failed to perform reading :(");
        #endif
        return;
    }

    payload.reset();
    payload.addTemperature(1, sensorTemperature());
    payload.addRelativeHumidity(2, sensorHumidity());
    payload.addBarometricPressure(3, sensorPressure());
    payload.addAnalogOutput(4, sensorGasResistance());

    lorawanSend(payload.getBuffer(), payload.getSize());

}

void setup() {
    
    // Initialize the built in LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Init debug line
    Serial.begin(115200);
    while ((!Serial) && (millis()<5000)) delay(100);
    
    #ifdef DEBUG
        Serial.println("BME680 sensor example");
    #endif

    // Init sensor
    setupSensor();

    // Init Radio
    lorawanSetup();

}

void loop() {

    uint32_t block = millis() / SEND_EVERY;
    if (block != last_block) {
        last_block = block;
        send();
    }

}