/*
Implement a 4 digit key press counter with persistence using Internal EEPROM. 

Output Requirements:
1] As soon as the board is powered up or reset, counter should display 0000 on SSDs.
2] On every key press  counter value should increment by 1.
3] On a  long key press (2 seconds), Count should  reset to zero.
4] On pressing STORE switch, the current count should be  stored in internal EEPROM.
5] On subsequent power ups or reset the counter should start from the previous stored value in the EEPROM.

Inputs:
1] DKS 1 as Count input .
2] DKS 1 Long press to reset the count .
3] DKS 2 Store Input.
*/
