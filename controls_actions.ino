
// Using variable lightring_pwr_state to track global power state as its always enabled ...

void button_react(unsigned char button_id){

  if (button_id == 0){
    switch (MCP_press_type){
    case SINGLE:
      Serial.println("1");
      if (led_mode == -1){ // If leds are off, turn on domelight on white
        led_mode = WHITE;
        set_lightring(WHITE); // Sets PWM values for channels 0-2 to white values
        pca_toggle(1);      // Toggle ON Output 4 (domelight enable?)
        lightring_state(1); // Enable outputs on channels 0-2
      }
      else if (led_mode == WHITE){ // If in white mode, turn off everything
        pca_toggle(0);      // Toggle ON Output 4 (domelight enable?)
        lightring_state(0); // Disable outputs on channels 0-2
        strips_state(0);    // Turn off RGB strips
      }
      else if (led_mode >= COLORS && strips_pwr_state){ // If in another mode than white or off, scroll through modes
        //        Serial.println(strips_pwr_state);
        Serial.println("cycle mode");
        cycle_mode();
      }
      else if (led_mode >= COLORS && !strips_pwr_state){ // If in another mode than white or off, scroll through modes
        strips_state(1);
      }
      break;
    case HOLD:
      Serial.println("1");
      if (led_mode == -1){
        led_mode = COLORS;
        set_lightring(COLORS); // Sets PWM values for channels 0-2 to RGB values.
        //        set_strips(COLORS);    // Set RGB strips values to Colors 
        set_strips_same(COLORS);    // Set RGB strips values to Colors 
        lightring_state(ON); // Enable outputs on channels 0-2
        strips_state(ON);      // Turn on RGB strips
      }
      else if (led_mode == WHITE){
        if (output4_enabled && strips_pwr_state){
          Serial.println("11");
          pca_toggle(OFF);      // Toggle PCA output 4
        }
        else if (output4_enabled && !strips_pwr_state){
          strips_state(1);      // Turn on RGB strips
          set_strips_same(WHITE);
        }
        else if (!output4_enabled && strips_pwr_state){
          pca_toggle(ON);      // Toggle PCA output 4
        } 
      }

      break;
    case DOUBLE:
      Serial.println("1");
      //      if (!output4_enabled && !lightring_pwr_state){
      if (led_mode == -1){
        led_mode = WHITE;
        set_lightring(WHITE); // Sets PWM values for channels 0-2 to white values
        set_strips(WHITE);    // Set RGB strips values to Colors
        lightring_state(ON); // Enable outputs on channels 0-2
        strips_state(ON);    // Turn on RGB strips
      }
      else {
        pca_toggle(OFF);      // Toggle ON Output 4 (domelight enable?)
        lightring_state(OFF); // Disable outputs on channels 0-2
        strips_state(OFF);    // Turn off RGB strips
      }
      break;
    }
  }

  else if (button_id == 1){
    switch (MCP_press_type){
    case 1:
      Serial.println("1");
      if (strips_pwr_state){
      }
      else if (!strips_pwr_state){
      }
      break;
    case 8:
      Serial.println("8");
      if (strips_pwr_state){
        global_led_state(0);
      }
      else if (!strips_pwr_state){
        global_led_state(1);
      }
      break;
    case 42:
      Serial.println("42");
      if (strips_pwr_state){
      }
      else if (!strips_pwr_state){
      }
      break;
    }
  }
  else if (button_id == 2){
    Serial.println("2");
    pca_toggle(0);
  }
  else if (button_id == 3){
    Serial.println("3");
    pca_toggle(1);
  }
  else if (button_id == 4){
    Serial.println("4");
    lightring_state(0);
    strips_state(0);
  }
  else if (button_id == 5){
    Serial.println("5");
    test_colors();
  }
  else if (button_id == 6){
    Serial.println("6");
  }
  else if (button_id == 7){
    Serial.println("7");
  }
}

// Encoder Reactions 
void encoder_react(char _value, int temp_time){
  if (lightring_pwr_state == 0){
    led_mode = COLORS;
    set_lightring(COLORS);
    lightring_state(1);
  }

  if (led_mode == WHITE || (led_mode >= COLORS && MCP_port_state[0] == 0)){
    set_brightness(_value);
  }
  else if (led_mode == COLORS && MCP_port_state[0] == 1){
    if (debug){
      Serial.println(MCP_port_state[0]);
    }
    if (_value < 0){
      color_cycle(0);
    }
    else if (_value > 0){
      color_cycle(1);
    }
  }
  else if (led_mode == COLORCYCLE && MCP_port_state[0] == 1){

    // ** NOTE 2553 in this case = ~10 minutes...
    // fix steps for defined steps (ie. 1 second, 5 sec, ..., 1 min, etc)

    if (color_cycle_speed >= 0 && color_cycle_speed < 100){
      _value = _value * 1;
    }
    else if (color_cycle_speed >= 100 && color_cycle_speed < 500){
      _value = _value * 3;
    }
    else if (color_cycle_speed >= 500 && color_cycle_speed < 900){
      _value = _value * 5;
    }    
    else if (color_cycle_speed >= 900 && color_cycle_speed < 1500){
      _value = _value * 10;
    }    
    else if (color_cycle_speed >= 1500 && color_cycle_speed <= 2353){
      _value = _value * 20;
    }

    color_cycle_speed = color_cycle_speed + _value;

    if (color_cycle_speed <= 0){
      color_cycle_speed = 2353;
    }
    else if (color_cycle_speed >= 2353){
      color_cycle_speed = 0;
    }

    if (debug){
      Serial.println(color_cycle_speed);
    }
  }
  if (debug){
    Serial.print("Rotated!: ");
    Serial.print(encoder_direction, DEC); // Print in DEC since char 0 and -1 wont print out properly otherwise or cast as int, whatever
    Serial.print(F(" | _value: "));
    Serial.print(_value, DEC);
    Serial.print(F(" | Time: "));
    Serial.println(temp_time);
    Serial.println();
  }

}



