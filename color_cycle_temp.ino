void color_cycle(bool color_cycle_direction){

  //bool color_cycle_direction = 1;  // ADD CHANNEL DIFF

  unsigned char current_channel = 0; // testing channel 0

    if(cycle_debug){
    Serial.print(F("CYCLE FUNC TLC Not grouped- color_cycle_step: "));
    Serial.println(color_cycle_step);
  }

  // NEW PART ** forces minor changes down the code (adds the for() loop line)
  byte temp_current_channel = current_channel + 1;
  ;


  //  temp_current_channel = current_channel + 1; // trick, do not change.... just add one to current channel in the check ffs... !

  for (byte y = current_channel; y < temp_current_channel; y++){// END NEW PART 
    boolean error = 1;

    switch (cycle_count[y]){ // Internal && !Drive
    case 1:
      incColor = 1;
      decColor = 0;
      noColor = 2;
      break;
    case 2:
      incColor = 2;
      decColor = 1;
      noColor = 0;
      break;
    case 3:
      incColor = 0;
      decColor = 2;
      noColor = 1;
      break;
    }

    if (color_cycle_direction == 1){
      if(cycle_debug){    
        Serial.print(F("Cycle direction: "));
        Serial.println(color_cycle_direction);
                  Serial.println(rgb_colors[y][0]);
          Serial.println(rgb_colors[y][1]);
          Serial.println(rgb_colors[y][2]);
      }
      if ( rgb_colors[y][decColor] + rgb_colors[y][incColor]  != cycle_counter_top){ 
        if(debug){    
          Serial.println(F("ERROR 1"));
        }
        error = 1;
      }
      else { 
        error = 0;
      }
      if (cycle_counter[y] > 0){
        cycle_counter[y] = cycle_counter[y] - color_cycle_step;
        rgb_colors[y][incColor] = rgb_colors[y][incColor] + color_cycle_step;
        rgb_colors[y][decColor] = rgb_colors[y][decColor] - color_cycle_step;
        rgb_colors[y][noColor] = off;
//        if(cycle_debug){
//          Serial.println(y);
//          Serial.println(rgb_colors[y][0]);
//          Serial.println(rgb_colors[y][1]);
//          Serial.println(rgb_colors[y][2]);
//        }
        //        update_leds();
        set_strips_same(1);
      }
      else if (cycle_counter[y] <= 0){
        if(cycle_debug){
          Serial.println(F("***************Change INC/DEC/NO DIR 1***************"));
        }
        cycle_counter[y] = cycle_counter_top;
        if (cycle_count[y] != 3){
          cycle_count[y]++;
        }
        else {
          cycle_count[y] = 1;
        }
      }

      if ( rgb_colors[y][decColor] + rgb_colors[y][incColor]  != cycle_counter_top){ 
        if(debug){        
          Serial.println(F("ERROR 2"));
        }
        error = 1;
      }
      else { 
        error = 0;
      }
    }
    else if (color_cycle_direction == 0){
      if(cycle_debug){
        Serial.print(F("Cycle direction: "));
        Serial.println(color_cycle_direction);
//      }
//              if(cycle_debug){
          Serial.println(rgb_colors[y][0]);
          Serial.println(rgb_colors[y][1]);
          Serial.println(rgb_colors[y][2]);
        }
      if ( rgb_colors[y][decColor] + rgb_colors[y][incColor]  != cycle_counter_top){ 
        if(debug){        
          Serial.println(F("ERROR 3"));
        }
        error = 1;
      }
      else { 
        error = 0;
      }

      if (cycle_counter[y] < cycle_counter_top){
        cycle_counter[y] = cycle_counter[y] + color_cycle_step;
        rgb_colors[y][incColor] = rgb_colors[y][incColor] - color_cycle_step;
        rgb_colors[y][decColor] = rgb_colors[y][decColor] + color_cycle_step;
        rgb_colors[y][noColor] = off;
        set_strips_same(1);
      }
      else if (cycle_counter[y] >= cycle_counter_top){
        if(cycle_debug){
          Serial.println(F("***************Change INC/DEC/NO DIR 0***************"));
        }
        cycle_counter[y] = 0;
        if (cycle_count[y] != 1){
          cycle_count[y]--;
        }
        else {
          cycle_count[y] = 3;
        }
      }

      if ( rgb_colors[y][decColor] + rgb_colors[y][incColor]  != cycle_counter_top){ 
        if(debug){        
          Serial.println(F("ERROR 4"));
        }
        error = 1;
      }
      else { 
        error = 0;
      }

    }
    if(cycle_debug){
      Serial.print(F("cycle_count: "));
      Serial.println(cycle_count[y]);
      Serial.print(F("cycle_counter: "));
      Serial.println(cycle_counter[y]);
    }
  }

}








