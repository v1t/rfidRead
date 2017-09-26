/*******************************************
   function:get the id of RFID key
   RFID	                  SunFounder Uno
   VCC	                      3.3V
   RST	                      2
   GND	                      GND
   MISO	                      3
   MOSI	                      4
   SCK	                      5
   NSS	                      6
   IRQ	                      7
 ****************************************/

#include"rfid1.h"
#include "SPI.h"
RFID1 rfid;//create a variable type of RFID1

uchar serNum[5];  // array to store your ID


uchar lastTag1[8];
uchar lastTag2[8];

unsigned long prevTime1;
unsigned long prevTime2;


unsigned long time;
unsigned long prevTime;


void setup()
{
  Serial.begin(9600); //initialize the serial
  rfid.begin(7, 13, 11, 12, 6, 2);  ////rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISO_PIN,NSS_PIN,RST_PIN)
  delay(100);//delay 1s
  rfid.init(); //initialize the RFID
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
  prevTime = 0;
}
void loop()
{ 
  checkRFID(0);
  checkRFID(1);
  rfid.halt(); //command the card into sleep mode
}

void checkRFID(int i) {

  delay(10);//delay 1s

  if (i == 0) {
    rfid.begin(7, 13, 11, 12, 10, 2);
  }
  if (i == 1) {
    rfid.begin(7, 13, 11, 12, 9, 2);
  }

  delay(1);
  rfid.init();
  uchar status;
  uchar str[MAX_LEN];
  // Search card, return card types
  status = rfid.request(PICC_REQIDL, str);
  if (status != MI_OK)
  {
    return;
  }
  // Show card type
  //rfid.showCardType(str);
  //Prevent conflict, return the 4 bytes Serial number of the card


  status = rfid.anticoll(str);
  if (status == MI_OK)
  {
    memcpy(serNum, str, 5);
    uchar returnId[8];
    // rfid.showCardID(serNum);//show the card ID    //uncomment to print tag ID straight to serial.
    rfid.getCardID(returnId, 8, serNum);  //stores tag ID to a char array.


    if (i == 0 && !compareTags(returnId, lastTag1)) {
      memcpy(lastTag1, returnId, 8);
      for (int j = 0; j < 8; j++) {
        Serial.print(lastTag1[j], HEX);
      }
      Serial.println();
    }
  }
  rfid.halt(); //command the card into sleep mode

  
}


bool compareTags(uchar* tag1, uchar* tag2) {
  //compare two tag codes
  if (strncmp(tag1, tag2, 8) == 0) {
    return true;
  } else {
    return false;
  }
}


