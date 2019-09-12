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
#include <AESLib.h>

const static uint8_t RADIO_ID = 0;             // Our radio's id.
const static uint8_t DESTINATION_RADIO_ID = 1; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN = 10;
uint8_t key[] = {33,85,123,201,178,144,127,222,1,92,7,35,33,69,108,173};

char data;

NRFLite _radio;
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
    
}

void loop()
{
    
    while(_radio.hasData())
    {
        _radio.readData(&data);
        aes128_dec_single(key, &data);
        Serial.print(data); 
    }

    data = (char)Serial.read();
    if (data > 0) {      
      // By default, 'send' transmits data and waits for an acknowledgement.  If no acknowledgement is received,
      // it will try again up to 16 times.  You can also perform a NO_ACK send that does not request an acknowledgement.
      // The data packet will only be transmitted a single time so there is no guarantee it will be successful.  Any random
      // electromagnetic interference can sporatically cause packets to be lost, so NO_ACK sends are only suited for certain
      // types of situations, such as streaming real-time data where performance is more important than reliability.
      //   _radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData), NRFLite::NO_ACK)
      //   _radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData), NRFLite::REQUIRE_ACK) // THE DEFAULT
      
        aes128_enc_single(key, &data);
      _radio.send(DESTINATION_RADIO_ID, &data, sizeof(data), NRFLite::NO_ACK);
    }

}
