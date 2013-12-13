
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

#define IRQ   (2)
#define RESET (3)  // Not connected by default on the NFC Shield

Adafruit_NFCShield_I2C nfc(IRQ, RESET);

uint8_t keyuniversal[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

const int STORAGE_MAX_SIZE = 3;
int lastScannedID          = -1;
int tagScanCounter         = 0;

// this is the UID of the card that is stuck to the bottom of the reader for transfering data
uint8_t transUID[4] = { 109, 248, 249, 200 };

void setup(void)
{
  // initialize the digital pin as an output.
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  
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
  
  // reset storage card
  resetStorage();
  
  //beep 3 times to tell the user that the program is initialized
  beep(50);
  beep(50);
  beep(50);
}


void loop(void)
{
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if ( success )
  {
    if ( uidLength != 4 )
    {
       Serial.println("Error: only Mifare Classic cards are supported.");
       return;
    }
    
    if ( isTransCard(uid) )
    {
       addToStorage(lastScannedID, uid, uidLength);
       lastScannedID = -1;
    } else
    {
       // read plant ID     
       lastScannedID = readPlantID( uid, uidLength );
       Serial.print("Found plant tag ");
       Serial.println( lastScannedID );
    }
  }
  
  if ( tagScanCounter >= 1 )
  {
    tagScanCounter = 0; 
    
    nfc.toggleReset();
    delay(100);
    nfc.begin();
    nfc.SAMConfig();
  } else
  {
    tagScanCounter++;
  }
}


bool addToStorage( int plantID, uint8_t uid[], uint8_t uidLength )
{  
  bool success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 4, 1, keyuniversal);
  if ( !success )
  {
     Serial.println("Error: unable to authenticate section for read/write access");
     return false; 
  }
  
  int curBlock = 4;
  uint8_t data[16];
  success = nfc.mifareclassic_ReadDataBlock(curBlock, data);
  if ( !success )
  {
     Serial.print("Error: unable to read block ");
     Serial.println(curBlock);
     return false;
  }

  if ( data[7] == 85 )    //85 == 0x55 == 'U', which identifies the Well Known Type (WKT) for an URI record
  {
    int len = data[6];
    char tagData[len];
    int i;
    for ( i = 0; i < len; ++i )
      tagData[i] = -1;
      
    if ( plantID != -1 )
    {
      Serial.print("Attempting to add ");
      Serial.print(plantID);
      Serial.println(" to storage");
    }
   
    if ( (len - 1) >= STORAGE_MAX_SIZE + 1 )
    {
       //Serial.println("Storage tag full"); 
       return false;
    }
    
    int tagIt  = 0;
    int dataIt = 9;        // skip WKT identifier, URI identifier code and plant block start identifier of the first block of the section
    bool processing = true;
    for ( i = dataIt; i < 16; ++i )
    {
      if ( data[i] == 254 )    // 254 == 0xFE == TLV block terminator
      {
         processing = false;
         break; 
      }
      
      if ( plantID != -1 && data[i] == plantID )
      {
         Serial.println("Plant already stored in tag");
         return false;
      }
      
      tagData[tagIt] = data[i];
      tagIt++;
    }
    
 
    
    if ( plantID != -1 )
    {
      // add new plantID to the list
      tagData[len - 1] = plantID;
      
      success = nfc.mifareclassic_WriteNDEFURI(1, NDEF_URIPREFIX_NONE, tagData);
      if ( !success )
      {
        Serial.println("Error: unable to write to block");
        return false;  
      }
      
      // beep to signal that the plant has been saved on the tag
      beep(200);
      
      Serial.print("tagData: ");
      for ( i = 0; i < len; ++i )
      {
        Serial.print( int(tagData[i]), DEC );
        Serial.print(" ");
      }
      Serial.println();
      
      tagIt++;
    }
    
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);

    if ( tagIt - 1 >= 1 )
      digitalWrite(4, HIGH);
    if ( tagIt - 1 >= 2 )
      digitalWrite(5, HIGH);
    if ( tagIt - 1 >= 3 )
      digitalWrite(6, HIGH);

    return true;
  }
  
  return false;
}


int readPlantID( uint8_t uid[], uint8_t uidLength )
{
  bool success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 4, 1, keyuniversal);
  if ( !success )
  {
     Serial.println("Error: unable to authenticate block 4");
     return -1; 
  }
  
  uint8_t data[16];
  success = nfc.mifareclassic_ReadDataBlock(4, data);
  if ( !success )
  {
     Serial.println("Error: unable to read block 4");
     return -1;
  }
  
  if ( data[7] == 85 )    //85 == 0x55 == 'U', which identifies the Well Known Type (WKT) for an URI record
  {
    return data[9];
  }
  
  return -1;
}


bool isTransCard( uint8_t uid[] )
{
  bool isTransCard = true;
  int i;
  for ( i = 0; i < 4; ++i )
    if ( uid[i] != transUID[i] )
      isTransCard = false;
      
  return isTransCard;
}


bool resetStorage()
{
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  
  uint8_t success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if ( !isTransCard(uid) )
    return false;
  
  success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 4, 1, keyuniversal);
  if ( !success )
  {
     Serial.println("Error: unable to authenticate section for write access");
     return false; 
  }

  char tagData[2] = { 66, '\0' };  
  success = nfc.mifareclassic_WriteNDEFURI(1, NDEF_URIPREFIX_NONE, tagData);
  if ( !success )
  {
    Serial.println("Error: unable to write to block");
    return false;  
  }
  
  return true;
}


void beep(unsigned char delayms)
{
  analogWrite(9, 20);
  delay(delayms);
  analogWrite(9, 0);
  delay(delayms);
}
