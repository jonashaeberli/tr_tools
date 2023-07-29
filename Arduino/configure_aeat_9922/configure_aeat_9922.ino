#include <SPI.h>
#include <stdlib.h>

// Define variables to hold user input
int userChoice = 0;
int configChoice = 0;
int reconfigChoice = 0;
int newNumber = 0;

uint16_t angle = 0x00; // Init variable to store 14b abs position

const int encoderNCS = 10; // Chip select (slave select) pin for the encoder
const int encoderMISO = 12; // MISO pin of the encoder
const int encoderMOSI = 11; // MOSI pin of the encoder
const int encoderSCK = 13; // SCK pin of the encoder

const uint16_t addUnlock = 0x10; // Value to unlock MTP register 8'hAB
const uint16_t addProgram = 0x10; // Value to program MTP to EEPROM 8'hA1
const uint16_t addError = 0x21; // Addres to read Error bits from encoder
const uint16_t addTest = 0x08;
const uint16_t addRead = 0x3F;


void setup() {
  Serial.begin(9600);
  SPI.begin();
  pinMode(encoderNCS, OUTPUT);
  digitalWrite(encoderNCS, HIGH); // Set NCS (CS) high initially
}

void loop() {
  setupEncoder();
  delay(2000);
}

uint16_t createSPICmd(uint8_t spiAddress, char readWrite) {
  // Bit 15: Even Parity Bit
  // Bit 14: Read (1) / Write (0)
  // Bits 13 to 8: Address (6 bits)
  // Bits 7 to 0: 0 (Reserved)

  bool evenParityBit = calculateEvenParity(spiAddress, readWrite); // Calculate the parity bit for the address

  uint16_t spiCmd = (readWrite == 'r') ? 0x4000 : 0x0000; // Set the read/write bit based on the input character

  spiCmd |= (evenParityBit << 15); // Add the even parity bit

  spiCmd |= (spiAddress & 0xFF); // Add the address bits

  return spiCmd;
}

bool calculateEvenParity(uint8_t data, char readWrite) {
  bool parity = false;
  for (int i = 0; i < 8; i++) {
    if (data & (1 << i)) {
      parity = !parity;
    }
  }
  if (readWrite == 'r'){
    switch (parity){
      case 0:
      parity = 1;
      break;
      case 1:
      parity = 0;
    }
  }
  return parity;
}

uint16_t spiTransaction(uint16_t spiCmd) {
  digitalWrite(encoderNCS, LOW); // Enable the encoder by pulling NCS low
  delayMicroseconds(35); // At least 350ns between NCS falling edge and SCK rising edge
  // Send address for reading error

  SPI.beginTransaction(SPISettings(100, MSBFIRST, SPI_MODE1)); // CPOL=0, CPHA=1
  delayMicroseconds(35);
  uint16_t response = SPI.transfer16(spiCmd); // Send spiCmd (16bits)
  delayMicroseconds(35);
  SPI.endTransaction(); // End the SPI communication

  digitalWrite(encoderNCS, HIGH); // Disable the encoder by pulling NCS high
  delayMicroseconds(35); // High time of NCS between two transmissions at least 350ns
  return response;
}

void setupEncoder() {
  Serial.println("1. Read encoder values");
  Serial.println("2. Read configuration");
  Serial.println("3. Apply config from code");
  while (!Serial.available()); // Wait for user input
  userChoice = Serial.parseInt();
  Serial.read(); // Read and discard newline character

  switch(userChoice) {
    case 1 :
      Serial.println("You selectet read encoder values!");
      delay(1000);
      readAngle();
      break;
    case 2 :
      Serial.println("You selected read configuration! Pls enter address to read:");
      while (!Serial.available()); // Wait for user input
      uint16_t readAddress = Serial.parseInt();
      Serial.read(); // Read and discard newline character
      readConfiguration(readAddress);
      break;
    case 3 :
      Serial.println("You selectet apply config from code!");
      break;
    default :
      exit(0);
  }
}

void readAngle() {
  Serial.println("Reading 20 Positions");
  uint16_t spiCmdAngle = createSPICmd(addRead, 'r');
  for (int i = 0; i < 20; i++) {
    angle = spiTransaction(spiCmdAngle);
    Serial.println(angle);
  }
}

void readConfiguration(int address) {
  switch (address) {
    case 7:
      readMemory(0x07);
    break;

    case 8:
      readMemory(0x08);
    break;

    case 9:
      readMemory(0x09);
    break;

    case 10:
      readMemory(0x0A);
    break;

    case 11:
      readMemory(0x0B);
    break;

    case 21:
      readMemory(0x21);
    break;

    default:
      Serial.println("Address not available!");
    break;
  }
}

void reconfigure() {
  // Execute commands to reconfigure the encoder (commands you will add yourself)
  // For example: your_reconfigure_command(reconfigChoice, newNumber);
}

uint16_t readMemory(uint16_t address) {
  uint16_t readCmd = createSPICmd(address, 'r');
  spiTransaction(readCmd);
  uint16_t received = spiTransaction(0x00);
}