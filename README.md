# TR_calibration_tools

If u still decide to use this crappy library instead of the offical Development kit with pc software. Please currently use devel branch wich is in "Working conditions". Everthing still quite messy and without guarantee just change the two config Values. Make binary convert to Hex and insert into SevenConfig and EightConfig at the start of the code. If u need to reconfigure different registers you can change the Addresses accordingle but be aware of Hardcodeded Values. THIS LIBRARY IS CURRENTLY JUST FOT TESTING AND NOT EASY RECONFIGURATION WITH ALL EDGE CASE SCENARIOSE CONSIDERED!!!!! USE AT UR OWN RISK!!!!!!!!!!!!!!!

TODO:
- Finish aeat-9922 calibration tool
  - Add feedback what command is sent over spi
  - Add feedback and return received 2 Bytes in function readMemory()
  - Make table with all configurations of the encoder for ezzy config
  - Make selection reconfiger completly means:
    - Add function to unlock memory (pls only with additional user confirmation)
    - Add function to write the values from the configuration table directly to encoder
    - Add function that reads all values again using readMemory() function and compares them if they are identical to configuration table (all printed for debugging)
    - After comparing without difference ask user to Confirm writing to permanent memory
  - Add selection on initial menu for selection reading error memory and then read it plus print all out
  - fix all other noticed bugs + make user experience better...
