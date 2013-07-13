
// ***************** Configure I2C bus and PCA LED driver ***************
void Configure_I2C(){
  I2c.begin();       // Start I2C bus
  //  I2c.pullup(0);     // Disable internal pullups on I2C lines
  I2c.timeOut(10);   // Timeout I2C bus after 10ms if not responding
  TWBR = 12;         // Put I2C bus in 400hz mode

  // Configure PCA96x5 Expander
  I2c.write(PCA_RESET, 0xA5, 0x5A); // Send software reset **WORKING**

  if(debug){
    Serial.println();
    Serial.println(F("PCA software reset sent..."));
    Serial.println(F("Configuring PCA..."));
    Serial.println(F("Waking up PCA96x5"));
    Serial.print(F("read MODE1 Register: "));
    I2c.read(PCA96x5, PCA_MODE1, 8);                    
    int temp_read = I2c.receive();
    Serial.println(temp_read);
    Serial.print(F("read MODE2 Register: "));
    I2c.read(PCA96x5, PCA_MODE2, 8);                    
    temp_read = I2c.receive();
    Serial.println(temp_read);
  }

  pca_wake();

  if (PCA_NFET_OUT){
    I2c.write(PCA96x5, PCA_MODE2, 0x15); // Sets Optimum config for N-type driver (INVRT = 1 & OUTDRV = 1)
  }

  if(debug){ 
    Serial.print(F("read MODE1 Register: "));
    I2c.read(PCA96x5, PCA_MODE1, 8);                    
    int temp_read = I2c.receive();
    Serial.println(temp_read);
    Serial.print(F("read MODE2 Register: "));
    I2c.read(PCA96x5, PCA_MODE2, 8);                    
    temp_read = I2c.receive();
    Serial.println(temp_read);
    Serial.println();
  } 
}



void pca_sleep(){
  // Read MODE1 Register and set SLEEP bit (Sleep enabled when set)
  I2c.read(PCA96x5, PCA_MODE1, 8);                    
  int temp_write = I2c.receive();
  temp_write = temp_write | 0b00010000; 
  I2c.write(PCA96x5, PCA_MODE1, temp_write);    
}

void pca_wake(){
  // Read MODE1 Register and clear SLEEP bit (Normal operation when cleared)
  I2c.read(PCA96x5, PCA_MODE1, 8);                    
  int temp_write = I2c.receive();
  temp_write = temp_write & 239; // 0b11101111
  I2c.write(PCA96x5, PCA_MODE1, temp_write);  
}

///************************* PCA TEST ROUTINE *********************************
//void _test_pca(){
//
//
//set_lightring(1);
//set_strips(1);
//
//for (int x = 0x02; x <= 0x11; x++){
//Serial.println(x);
//I2c.write(PCA96x5, x, 255);
//delay(1500);
//}
//
//}

// PCA test routine
void test_pca(byte testmode){

  unsigned char testdelay = 75;

  if (testmode == 1){
    for (int x = 0x14; x <= 0x17; x++){
      I2c.write(PCA_ALLCALL, x, 0b01010101);      // LDRx = 01 —LED driver x is fully on (individual brightness and group dimming/blinking not controlled)
    }
    delay(500);
    for (int x = 0x14; x <= 0x17; x++){
      I2c.write(PCA_ALLCALL, x, 0xFF);            // LDRx = 11 —LED driver x is individual brightness and group dimming/blinking controlled
    }
    delay(500);
  }
  if (testmode == 2){

    for (int x = 0x02; x <= 0x11; x++){
      //      Serial.println(x, HEX);
      I2c.write(PCA96x5, x, 0x00);
      delay(testdelay);
    }

    for (int x = 0x02; x <= 0x11; x++){
      //      Serial.println(x, HEX);
      I2c.write(PCA96x5, x, 0xFF);
      delay(testdelay);
    }


  }
  else if (testmode == 3){
    for (int x = 0x02; x <= 0x11; x++){
      //      Serial.println(x, HEX);
      I2c.write(PCA96x5, x, 0x00);
    }
  }
  else if (testmode == 4){
    for (int x = 0x02; x <= 0x11; x++){
      //      Serial.println(x, HEX);
      I2c.write(PCA96x5, x, 0xFF);
    }
  }
  else if (testmode == 5){
  }
}





