/*

Demonstrates simple RX and TX operation.
Any of the Basic_RX examples can be used as a receiver.
Please read through 'NRFLite.h' for a description of all the methods available in the library.

Radio    Arduino
CE    -> 9
CSN   -> 10 (Hardware SPI SS)
MOSI  -> 11 (Hardware SPI MOSI)
MISO  -> 12 (Hardware SPI MISO)
SCK   -> 13 (Hardware SPI SCK)
IRQ   -> No connection
VCC   -> No more than 3.6 volts
GND   -> GND

*/

#include <SPI.h>
#include <NRFLite.h>

const static uint8_t RADIO_ID = 1;             // Our radio's id.
const static uint8_t DESTINATION_RADIO_ID = 0; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;
const static uint8_t PIN_RADIO_IRQ = 3;

NRFLite _radio;
byte serial_rx_buf[64];
volatile uint8_t serial_rx_len;

void radioInterrupt();

void setup()
{
    Serial.begin(115200);
    
    // By default, 'init' configures the radio to use a 2MBPS bitrate on channel 100 (channels 0-125 are valid).
    // Both the RX and TX radios must have the same bitrate and channel to communicate with each other.
    // You can run the 'ChannelScanner' example to help select the best channel for your environment.
    // You can assign a different bitrate and channel as shown below.
    //   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE250KBPS, 0)
    //   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE1MBPS, 75)
    //   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE2MBPS, 100) // THE DEFAULT
    
    while (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE2MBPS, 100))
    {
        Serial.println("Cannot communicate with radio");
        delay(1000);   
    }

    attachInterrupt(digitalPinToInterrupt(PIN_RADIO_IRQ), radioInterrupt, FALLING);
    _radio.startRx();
    Serial.println("Can communicate with radio yay");
}

void loop()
{
    static unsigned long time_of_last_tx;
    unsigned long now = millis();
    if (serial_rx_len == sizeof(serial_rx_buf) || now > (time_of_last_tx + 500)) {
        _radio.startSend(DESTINATION_RADIO_ID, serial_rx_buf, serial_rx_len, NRFLite::NO_ACK);
        serial_rx_len = 0;
        _radio.startRx();
        time_of_last_tx = now;
    }
}

void radioInterrupt()
{
    uint8_t txOk, txFail, rxReady;
    _radio.whatHappened(txOk, txFail, rxReady);
    if (rxReady) {
        uint8_t length = _radio.hasDataISR();
        byte buf[32];
        _radio.readData(buf);
        Serial.write(buf, length);
    }
}

void serialEvent()
{
    size_t available = Serial.available();
    if (available) {
        uint8_t buf_remaining = sizeof(serial_rx_buf) - serial_rx_len;
        size_t length = Serial.readBytes(serial_rx_buf + serial_rx_len, available > buf_remaining ? buf_remaining : available);
        serial_rx_len += length;
    }
}
