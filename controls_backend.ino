/*
Random thoughts:
 
 - timers and stuff (Nick Gammon's site for reference)
 - replace bitRead by a check == 1 on bitshift >> x  ??
 - add special mode (tricky...)
 - disable double click when powered off (solves initial single click issue ?)
 
 */

/* Valid Press types:
 type == 0  ->  Not currently valid
 type == 1  ->  Short press (Level 1)
 type == 2  ->  Medium press (Level 2)
 type == 3  ->  
 type == 8  ->  Hold timed out 
 type == 42 ->  "Double Click"
 */


void button_id_detect(){

  unsigned char MCP_interrupt_port = 0; 

  //  unsigned long test_micros = micros();     // Speed test start
  //  Serial.println(micros() - test_micros);    // Speed test end

  sleep_timeout = millis();  // Resets sleep timeout on button press / release
  MCP_int_triggered = 0;  // Resets Interrupt triggered flag


  for (int x = 0; x <= 7; x++){         // Loop for each bit in the reading

    if (bitRead(MCP_INTF_read, x)){    // Check all bits to find which one caused the interrupt
      MCP_interrupt_port = x;          // Sets MCP_interrupt_port to the GPIO port # that caused the interrupt

      MCP_port_state[x] = bitRead(MCP_INTCAP_read, x);  // Set port state variable according to port state

      if (!MCP_port_state[x]){                      // If port state == Low (Pressed)
        MCP_press_time[x] = millis();
        hold_triggered[x] = 0;
        MCP_press_type = PRESSED;  //Reset Press type
        if (debug_mcp_1){
          Serial.println(F("Pressed!"));
          Serial.print(F("Millis on press: "));      
          Serial.println(MCP_press_time[x]);
        }
      }       // End port state == LOW

      else if (MCP_port_state[x]){  // Port state == High (Released) 

        if ((unsigned long)(millis() - MCP_press_time[x]) <= MCP_short_time) { 

          MCP_press_type = NONE;
          mp_counter[x]++;

          if(debug_led_mcp){
            digitalWrite(ledpin2, HIGH);
          }

          if (MCP_prev_int == MCP_interrupt_port){

            if (mp_counter[x] >= 2){

              MCP_press_type = DOUBLE;
              button_react(x);
              if (debug_led_mcp){
                digitalWrite(ledpin2, LOW);
                ledstate3 = !ledstate3;
                digitalWrite(ledpin3, ledstate3);
              }

              mp_counter[x] = 0;

            }
          }
//          else {
//            MCP_press_type = 0;
//          }
        }
        else if ((unsigned long)(millis() - MCP_press_time[x]) <= (4 * MCP_short_time)) {  // MCP_prev_int
          MCP_press_type = LONG;
          button_react(x);
        }
        else {
          MCP_press_type = NONE;
        }
      }         // End port state == high

    } 

  } // End looping 0-7

  MCP_prev_int = MCP_interrupt_port;

  if(debug){
    print_debug(MCP_interrupt_port);
  }

  if (debug_mcp_1){
    Serial.print(F("INTF:    "));
    Serial.println(MCP_INTF_read);
    Serial.print(F("INTCAP:  "));
    Serial.println(MCP_INTCAP_read);
    Serial.println();
  }
  if (debug){
    Serial.println();
  }

}



void button_timeout(){         // Button timing/timeout handling

  for (int x = 0; x <= 7; x++){ 

    if (mp_counter[x] == 1 && (unsigned long)(millis() - MCP_press_time[x]) >= mp_timeout){
      mp_counter[x] = 0;
      MCP_press_type = SINGLE;
      button_react(x);
      if(debug_led_mcp){
        digitalWrite(ledpin2, LOW);
      }

      if(debug){
        print_debug(x);
      }
    }

    else if (MCP_port_state[x] == 0 && (unsigned long)(millis() - MCP_press_time[x]) >= press_timeout && hold_triggered[x] == 0){
      hold_triggered[x] = 1;
      MCP_press_type = HOLD;
      button_react(x);
      if(debug){
        print_debug(x);
      }

      if (debug_led){
        if (interval < 1000){
          interval = 1000;
        }
        else {
          interval = 250;
        }
        ledstate = HIGH;
      }
    }
  }// Double press timeout End
}

void print_debug(unsigned char MCP_interrupt_port){

  if (MCP_prev_int == -1){
    Serial.print(F("No previous press"));
  }
  else {
    Serial.print(F("Prev GP"));
    Serial.print(MCP_prev_int, DEC);
  }
  Serial.print(F(" | Cnts: "));
  Serial.print(mp_counter[MCP_interrupt_port]);
  Serial.print(F(" | GP: "));
  Serial.print(MCP_interrupt_port, DEC);
  Serial.print(F(" | Press type: "));
  Serial.print(MCP_press_type);
  Serial.println(); 
}


//  For proper Rotary encoder operation, use external 4.7k pullups, 
//  0.1uf caps to ground on each encoder pin (as close as possible to the pin),
//  and 10k resistors between the encoder and it's pullups and the arduino
//
//                0.1uf ---- GND
//                  |
//    encoder A/B --|-- 10K ---- arduino pin
//                  |
//                 Vcc



void encoder_interrupt(){

  //    encoder_time = micros();
  encoder_time = millis();

  unsigned char port_read = PIND;

  char reading_A = (port_read >> 3) & 1;
  char reading_B = (port_read >> 4) & 1;

  if (reading_A == reading_B){ // working
    encoder_direction = 1;  
  }
  //  else if (reading_A != reading_B){  // working
  else { // working
    encoder_direction = -1;
  }
  if (debug_enc){
    Serial.print(F("B: "));
    Serial.println(reading_B, HEX);
    Serial.print(F("A: "));
    Serial.println(reading_A, HEX);
    Serial.println(encoder_direction);
    Serial.println();
  }
}































































