#include <SPI.h>
#include <stdlib.h>

const int encoderNCS = 10; // Chip select (slave select) pin for the encoder
const int encoderMISO = 12; // MISO pin of the encoder
const int encoderMOSI = 11; // MOSI pin of the encoder
const int encoderSCK = 13; // SCK pin of the encoder

uint16_t encoderPosition = 0x00; // Init variable to store 14b abs position
uint16_t multiturn = 0x00; // Init variable to store multiturn position

void setup() {
  Serial.begin(9600);
  SPI.begin();
  pinMode(encoderNCS, OUTPUT);
  digitalWrite(encoderNCS, HIGH); // Set NCS high initially
  
  setMultiturn();
  setZeroOffset();
  safeConfig();
}

void loop() {
  readAngle();
  delay(1000);
}

uint16_t readAngle(){
  int readVector[] = {0x00, 0x00, 0x00, 0x00, 0x00};
  int arraySize = sizeof(readVector) / sizeof(readVector[0]);
  uint64_t response = spiTransaction(readVector, arraySize);
  multiturn = response >> 24;
  encoderPosition = (response & 0xFFFC00) >> 10;
  float EncoderAngle = (float)encoderPosition / 16383.0f;
  uint8_t GeneralStatus = response & 0x300 >> 8;
  uint8_t CRC = response & 0xFF;

  Serial.print("Multiturn: ");
  Serial.print(multiturn);
  Serial.print(" Encoder Position: ");
  Serial.print(EncoderAngle);
  Serial.print(" General Status: ");
  Serial.println(GeneralStatus);
}

void setMultiturn(){
  int readVector[] = {0xCD, 0xEF, 0x89, 0xAB, 0x4D, 0x00, 0x00, 0x80, 0x00}; // Set multiturn to 32768 (0x8000) -> this is zero in motorcontroller
  int arraySize = sizeof(readVector) / sizeof(readVector[0]);
  uint64_t writeMultiturn = spiTransaction(readVector, arraySize);
}

void setZeroOffset(){
  //First we have to update encoder position, we do this by calling readAngle()
  readAngle();

  //Then we set the zero offset with this value
  //We first have to convert encoderPosition to two one byte hexadecimal values (high and low)
  int highByte = encoderPosition >> 8;
  int lowByte = encoderPosition & 0xFF;

  int readVector[] = {0xCD, 0xEF, 0x89, 0xAB, 0x5A, 0x00, 0x00, highByte, lowByte}; // Set zero offset to recently read encoder position
  int arraySize = sizeof(readVector) / sizeof(readVector[0]);
  uint64_t writeMultiturn = spiTransaction(readVector, arraySize);
}

void safeConfig(){
  int readVector[] = {0xCD, 0xEF, 0x89, 0xAB, 0x63}; // safe the configured settings to non-volatile memory
  int arraySize = sizeof(readVector) / sizeof(readVector[0]);
  uint64_t writeMultiturn = spiTransaction(readVector, arraySize);
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