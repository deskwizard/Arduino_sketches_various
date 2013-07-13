#include <avr/sleep.h>
// ADCSRA = 0;     // DISABLE ADC MODULE

/********************** Sleep routine *************************/
void go_to_sleep(){
  if(debug){
    Serial.println(F("go to sleep"));
    Serial.flush();
  }

  //  sleep_requested = 0;

  if(debug_led){
    digitalWrite(ledpin, HIGH);
    digitalWrite(ledpin2, LOW);  
    digitalWrite(ledpin3, HIGH);
  }
  
  pca_toggle(0);
  lightring_state(0); // Turn off lightring leds
  pca_sleep(); // ~8mA less power draw when in sleep 
  led_mode = -1;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);      // Configure sleep mode
  sleep_enable();                           // Enable sleep

  // DISABLE BROWN OUT DETECTION (has to be done just before sleep_cpu() else it gets cancelled)
  MCUCR = _BV(BODS) | _BV(BODSE);  // turn on brown-out enable select
  MCUCR = _BV(BODS);        // this must be done within 4 clock cycles of above
  sleep_cpu(); // PUT THE CPU TO SLEEP

  // THE NEXT INSTRUCTION IS THE FIRST TO RUN WHEN COMING BACK FROM SLEEP MODE
  sleep_disable();
  //  was_asleep = 1;                           // Sets was_asleep Flag
  pca_wake();
  if(debug){
    Serial.println(F("go_to_sleep end"));
    Serial.println();
  }

  if(debug_led && !ledstate3){
    digitalWrite(ledpin3, !ledstate3);
  }

}



