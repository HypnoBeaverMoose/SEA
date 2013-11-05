
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

#define IRQ   (2)
#define RESET (3)  // Not connected by default on the NFC Shield

Adafruit_NFCShield_I2C nfc(IRQ, RESET);

void setup(void)
{
  Serial.begin(115200);
  Serial.println("=== Plant encoder v0.1 ===");
  Serial.println("Looking for PN532...");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // print version information
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();
}


void loop(void)
{  
  Serial.println("Type a number > 0 to encode plant ID, type 0 to read the current plant ID..");
  
  while ( !Serial.available() );
  
  int output = readIntInput();
  
  if ( output == 0 )
  {
     // reading
     if ( !readPlantID( ) )
     {
        Serial.println("Error reading plant ID."); 
     }
  } else
  {
     // encoding
     if ( !updatePlantID( output ) )
     {
        Serial.println("Error encoding plant ID."); 
     }
  }
}


bool readPlantID ()
{
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  bool authenticated = false;
  uint8_t keyuniversal[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if ( success )
  {
    if ( uidLength != 4 )
    {
       Serial.println("Error: only Mifare Classic cards are supported."); 
       return false;
    }
    
    success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 4, 1, keyuniversal);
    if ( !success )
    {
       Serial.println("Error: unable to authenticate block 4");
       return false; 
    }
    
    uint8_t data[16];
    success = nfc.mifareclassic_ReadDataBlock(4, data);
    if ( !success )
    {
       Serial.println("Error: unable to read block 4");
       return false;
    }
    
    Serial.print("hex data block 4: ");
    nfc.PrintHexChar(data, 16);
    
    Serial.print("payload length: ");
    Serial.println(data[6]);
    
    if ( data[7] == 85 )    //85 == 0x55 == 'U', which identifies the Well Known Type (WKT) for an URI record
    {
      int plantID = data[9];

      Serial.print("plantID: ");
      Serial.println(plantID);
    }
  }
  
  return true;
}


bool updatePlantID( int newID )
{
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  bool authenticated = false;
  
  // authentication key
  uint8_t keyb[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };
  
  // read NDEF formatted NFC card
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if ( success )
  {
    if ( uidLength != 4 )
    {
       Serial.println("Error: only Mifare Classic cards are supported."); 
       return false;
    }
    
    success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 4, 0, keyb);
    if ( !success )
    {
       Serial.println("Unable to authenticate card."); 
       return false;
    }
   
    const char data[2] = { newID, '\0' };    
    success = nfc.mifareclassic_WriteNDEFURI( 1, NDEF_URIPREFIX_NONE, data );
    if (success)
    {
      Serial.println("NDEF URI Record written to sector 1");
      Serial.println("");      
    }
    else
    {
      Serial.println("NDEF Record creation failed! :(");
    }
  }
  
  return true;
}

/**
 * reads a number from the serial port and returns it's value
 */
int readIntInput()
{
  int readBuff[12] = { 0, 0, 0, 0 };
  int it = 0;
  while ( Serial.available() && it < 4 )
  {
    Serial.print("it: ");
    Serial.println(it);
    
    int input = Serial.read(); 
    if ( input < 48 || input > 57 )
    {
       Serial.println("Error: NaN"); 
       Serial.flush();
       while ( Serial.available() ) Serial.read();
       return -1;
    }
    
    readBuff[it] = input - 48;
    
    for ( int i = 0; i < it; ++i )
      readBuff[i] *= 10;
    
    it++;
  }
  
  int output = readBuff[0] + readBuff[1] + readBuff[2] + readBuff[3];

  Serial.flush();
  while ( Serial.available() ) Serial.read();
  
  return output;
}
