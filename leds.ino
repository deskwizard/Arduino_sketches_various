//******************* Global LED turn off routine ******************************************
void global_led_state(boolean state){

  if (!state){ // Turn off LEDS
    for (int x = 0x14; x <= 0x17; x++){
      I2c.write(PCA_ALLCALL, x, 0x00);            // LDRx = 00 — LED driver x is turned off
    }
    strips_pwr_state = 0;
    sleep_requested = 1;
  }
  else if (state){ // Turn on LEDS
    for (int x = 0x14; x <= 0x17; x++){
      I2c.write(PCA_ALLCALL, x, 0b01010101);            // LDRx = 01 — LED driver x is turned fully On (No PWM)
    }
    //    for (int x = 0x03; x <= 0x11; x++){
    //      //      Serial.println(x, HEX);
    //      I2c.write(PCA96x5, x, 0xFF);
    //    }
    strips_pwr_state = 1;
    //    sleep_requested = 0;
  }
}


void test_colors(){
  set_strips_same(COLORS);    // Set RGB strips values to Colors 
  set_lightring(COLORS);    // Set RGB strips values to Colors 
  lightring_state(1);
  strips_state(1);
  rgb_colors[0][0] = 255;
  rgb_colors[0][1] = 0;
  rgb_colors[0][2] = 0;
  set_strips_same(COLORS);    // Set RGB strips values to Colors 
  set_lightring(COLORS);    // Set RGB strips values to Colors 
  delay(1000);
  rgb_colors[0][0] = 0;
  rgb_colors[0][1] = 255;
  rgb_colors[0][2] = 0;
  set_strips_same(COLORS);    // Set RGB strips values to Colors 
  set_lightring(COLORS);    // Set RGB strips values to Colors 
  delay(1000);
  rgb_colors[0][0] = 0;
  rgb_colors[0][1] = 0;
  rgb_colors[0][2] = 255;
  set_strips_same(COLORS);    // Set RGB strips values to Colors 
  set_lightring(COLORS);    // Set RGB strips values to Colors 
  delay(1000);
  rgb_colors[0][0] = 255;
  rgb_colors[0][1] = 0;
  rgb_colors[0][2] = 0;
  strips_state(0);
  lightring_state(0);
}

void pca_toggle(bool _state){

  I2c.read(PCA96x5, 0x14, 8);                    
  int temp_write = I2c.receive();

  if (_state){
    temp_write = temp_write | 0b01000000; 
    output4_enabled = 1;
  }
  else if (!_state){
    temp_write = temp_write & 0b00111111; 
    output4_enabled = 0;
  }

  sleep_requested = !_state;

  I2c.write(PCA96x5, 0x14, temp_write);  // LDR1 Register
}


void cycle_mode(){
  led_mode++;
  //  Serial.print(F("Led mode bf: "));
  //  Serial.print(led_mode);
  if (led_mode > last_mode ){
    led_mode = 1;
  }
  if (debug){
    Serial.print(F("Led mode: "));
    Serial.print(led_mode);
    Serial.println();
  }
}


void strips_state(bool _state){

  for (int x = 0x15; x <= 0x17; x++){
    // 1: Read LDR1-2-3
    // 2: Toggle bits
    // 3 Write LDR1-2-3
    I2c.read(PCA96x5, x, 8);                    
    int temp_write = I2c.receive();
    if (_state){ 
      temp_write = temp_write | 0b11111111; // Set LDR1-3 bits 0-7 (11 = PWM + GPWM)
      set_brightness(0);
    }
    else if (!_state){
      temp_write = temp_write & 0b00000000;  // Clear LDR1-3 bits 0-7 (00 = OFF)
      led_mode = -1;
    }
    I2c.write(PCA96x5, x, temp_write);  // Write LDR1-3 Register  
  }
  //  set_brightness(0);

  sleep_requested = !_state;
  strips_pwr_state = _state;

}

void set_strips(bool _color){
  for (int y = 1; y <= 4; y++){
    if (_color){
      for (int x = 0; x <= 2; x++){
        I2c.write(PCA96x5, rgb_channel[y][x], rgb_colors[y][x]);
      }
    }
    else if (!_color){
      for (int x = 0; x <= 2; x++){
        I2c.write(PCA96x5, rgb_channel[y][x], white[x]);
      }
    }
  }
}

void set_strips_same(bool _color){
  for (int y = 0; y <= 4; y++){
    if (_color){
      for (int x = 0; x <= 2; x++){
        I2c.write(PCA96x5, rgb_channel[y][x], rgb_colors[0][x]);
      }
    }
    else if (!_color){
      for (int x = 0; x <= 2; x++){
        I2c.write(PCA96x5, rgb_channel[y][x], white[x]);
      }
    }
  }
}

void lightring_state(bool _state){

  I2c.read(PCA96x5, 0x14, 8);      // set pointer to LDR0 register              
  int temp_write = I2c.receive();  // receive LDR0 register

  if (_state){ // Turning on
    temp_write = temp_write | 0b00111111; // Set LDR0 bits 0-5 (11 = PWM + GPWM)
    set_brightness(0);
  }
  else if (!_state){
    temp_write = temp_write & 0b11000000; // Clear LDR0 bits 0-5 (00 = OFF)
    led_mode = -1;
  }
  //  sleep_requested = !_state;

  lightring_pwr_state = _state;          // Set lightring power state
  I2c.write(PCA96x5, 0x14, temp_write);  // LDR0 Register

}

void set_lightring(bool _color){
  if (_color){
    for (int x = 0; x <= 2; x++){
      I2c.write(PCA96x5, rgb_channel[0][x], rgb_colors[0][x]);
    }
  }
  else if (!_color){
    for (int x = 0; x <= 2; x++){
      I2c.write(PCA96x5, rgb_channel[0][x], white[x]);
    }
    //    set_brightness(0);
  }
  //  set_brightness(0);
}

// ************* Brightness adjustment (Group PWM on PCA9635 ONLY!) ***************
void set_brightness(int _adj){
  int brightness;

  if (led_mode == WHITE){
    brightness = white[3]; 
  }
  else {
    brightness = RGB_brightness;
  }

  int step = _adj;

  if (debug){
    Serial.print(F("Brightness direction:  "));
    Serial.println(_adj);
    Serial.print(F("Brightness:"));
    Serial.println(brightness);
  }

  if (brightness < 20 && brightness >= 1){
    step = step * 1;
  }
  else if (brightness >= 20 && brightness < 50){
    step = step * 2;
  }
  else if (brightness >= 50 && brightness < 90){
    step = step * 3;
  }    
  else if (brightness >= 90 && brightness < 150){
    step = step * 5;
  }    
  else if (brightness >= 150 && brightness <= 255){
    step = step * 7;
  }
  if (debug){
    Serial.print("Step: ");
    Serial.println(step);
  }

  brightness = brightness + step;

  if (brightness >= 255){
    brightness = 255;
  }
  else if (brightness <= 1){
    brightness = 1;
  }

  I2c.write(PCA96x5, PCA_GRPPMW, brightness);  // GRPPMW Register 

  if (led_mode == WHITE){
    white[3] = brightness; 
  }
  else {
    RGB_brightness = brightness;
  }
  if (debug){
    Serial.print(F("Brightness after:"));
    Serial.println(brightness);
  }
}




























