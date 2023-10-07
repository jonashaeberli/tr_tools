#include <SPI.h>
#include <stdlib.h>

const int encoderNCS = 10; // Chip select (slave select) pin for the encoder
const int encoderMISO = 12; // MISO pin of the encoder
const int encoderMOSI = 11; // MOSI pin of the encoder
const int encoderSCK = 13; // SCK pin of the encoder

uint16_t angle = 0x00; // Init variable to store 14b abs position
uint16_t multiturn = 0x00; // Init variable to store multiturn position

const uint8_t angleRequest = 0x00;
const uint8_t serialNumberRequest = 0x21;
const uint8_t temperatureRequest = 0x08;
const uint8_t detailedStatusRequest = 0x3F;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  pinMode(encoderNCS, OUTPUT);
  digitalWrite(encoderNCS, HIGH); // Set NCS high initially
}

uint64_t spiTransaction(int SpiCommand[], uint8_t requestedSize) {

  uint64_t response = 0x0;

  digitalWrite(encoderNCS, LOW);
  delayMicroseconds(10);

  SPI.beginTransaction(SPISettings(3000, MSBFIRST, SPI_MODE1)); // CPOL=0, CPHA=1

  for (int i = 0; i < requestedSize; i++) {
    response = (response << 8) | SPI.transfer(SpiCommand[i]);
  }

  SPI.endTransaction();

  digitalWrite(encoderNCS, HIGH); // Disable the encoder by pulling NCS high
  return response;
}

void loop() {
  int readVector[] = {0x00, 0x00, 0x00, 0x00, 0x00};
  int arraySize = sizeof(readVector) / sizeof(readVector[0]);
  uint64_t response = spiTransaction(readVector, arraySize);
  uint16_t MultiTurn = response >> 24;
  uint16_t EncoderPosition = (response & 0xFFFC00) >> 10;
  float EncoderAngle = (float)EncoderPosition / 16383.0f;
  uint8_t GeneralStatus = response & 0x300 >> 8;
  uint8_t CRC = response & 0xFF;

  Serial.print("Positition:");
  Serial.println(MultiTurn + EncoderAngle);

  // Serial.println("Multiturn: " + String(MultiTurn) + " Encoder Position: " + String(EncoderAngle) + " General Status: " + String(GeneralStatus));
  
  delayMicroseconds(25);
}
