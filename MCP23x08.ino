// --- Configure SPI bus and MCP23S08 port expander ---
void Configure_MCP(){

  // initialize SPI:
  SPI.begin(); 
  SPI.setClockDivider (SPI_CLOCK_DIV2); // Self-explanatory
  SPI.setBitOrder(MSBFIRST);            // Self-explanatory
  SPI.setDataMode(SPI_MODE0);           // See SPI library page

  // Configures MCP Expander
  //  MCP_write(MCP_IODIR, 0xFF);    // Sets all ports as inputs ** Not neededfor all input (Power-On default)
  MCP_write(MCP_GPPU, 0xFF);     // Enable pull-ups on all ports but 0 and 1
  //  MCP_write(MCP_DEFVAL, 0xFF);   // Configure default compare to value ** CURRENTLY NOT USED **
  //  MCP_write(MCP_INTCON, 0x00);   // Configures all port to interrupt on change, not DEFVAL (Power-On default)
  MCP_write(MCP_GPINTEN, 0xFF);  // Enable interrupts on all ports but 1 (Pin B)(Must be done last !!)

  if (debug){
    Serial.println(F("Ports GP0-GP7 set as input"));
    Serial.println(F("Ports GP0-GP7 pull-ups enabled"));
    Serial.print(F("Reading Configuration register: "));
    Serial.println(MCP_read(MCP_IOCON));
    Serial.print(F("Read and set GPINTEN register: "));
    Serial.println(MCP_read(MCP_GPINTEN));
    Serial.print(F("Read INTCAP register: "));
    Serial.println(MCP_read(MCP_INTCAP));
    Serial.println();
  }

  for (int x = 0; x <= 7; x++){
    MCP_port_state[x] = bitRead(MCP_read(MCP_INTCAP), x);
  }

  MCP_read(MCP_INTCAP);        // Read INTCAP to clear any interrupts

}

void MCP_interrupt(){ // Before changes for encoder
  //   Reading the MCP here results in way quicker reads of the interrupt registers.
  //   It should improve responsiveness and maybe solve the occasional hanging, at
  //   the expense of about 18 bytes ( putting the variables volatile mostly )
  //
  //   The difference is -> obvious <- with only a LED on the interrupt pin...
  //

  // New testing (micros: 248-256)
  PORTB = PORTB & B11111011;       // Set pin 10 low (PB2)
  SPI.transfer(MCP23x08 +1);       // Send OP-Code (addr + R/W bit set)
  SPI.transfer(MCP_INTF);      // Send register address
  MCP_INTF_read = SPI.transfer(0x00);    // Read value
  MCP_INTCAP_read = SPI.transfer(0x00);    // Read value
  PORTB = PORTB | B00000100;       // Set pin 10 high (PB2)
  // New end */

  MCP_int_triggered = 1;

}


byte MCP_read(byte MCP_register){
  byte reading;
  PORTB = PORTB & B11111011;       // Set pin 10 low (PB2)
  SPI.transfer(MCP23x08 +1);       // Send OP-Code (addr + R/W bit set)
  SPI.transfer(MCP_register);      // Send register address
  reading = SPI.transfer(0x00);    // Read value
  PORTB = PORTB | B00000100;       // Set pin 10 high (PB2)
  return(reading);
}


void MCP_write(byte MCP_register, byte value){
  PORTB = PORTB & B11111011;      // Set pin 10 low (PB2)
  SPI.transfer(MCP23x08);         // Send OP-Code (addr + R/W bit cleared)
  SPI.transfer(MCP_register);     // Send register address
  SPI.transfer(value);            // Send value
  PORTB = PORTB | B00000100;      // Set pin 10 high (PB2)
}









