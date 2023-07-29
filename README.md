# TR_calibration_tools
TODO:
- Finish aeat-9922 calibration tool
  - Add feedback what command is sent over spi
  - Add feedback and return received 2 Bytes in function readMemory()
  - Make table with all configurations of the encoder for ezzy config
  - Make selection reconfiger completly means:
    - Add function to unlock memory (pls only with additional user confirmation)
    - Add function to write the values from the configuration table directly to encoder
    - Add function that reads all values again using readMemory() function and compares them if they are identical to configuration table (all printed for debugging)
    - After comparing ask user to Confirm writing to permanent memory
  - Add selection on initial menu for selection reading error memory and then read it plus print all out
  - fix all other noticed bugs + make user experience better...
