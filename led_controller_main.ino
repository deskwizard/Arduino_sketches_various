/*******  Informations *****************
 *
 * FYI: MCP23x08 and encoder != MCP23x08 with encoder ... 
 * 
 * Reset pin to MCP Expander Reset Pin
 * MCP Expander Interrupt to PD2 (INT0)
 *
 *
 * Rotary encoder A/B to PD3-PD4 
 * Rotary encoder SW to MCP GP2
 * See encoder tab for details       ^^
 * 
 ****************************************/

//#include <avr/sleep.h>
#include "defines.h"     // Include external defines file
#include <SPI.h>         // include the SPI library for MCP23S08
#include <I2C.h>
#include <EEPROM.h>

// Debugging
#define debug 1
#define debug_led 1
#define debug_enc 0
#define debug_mcp_1 0
#define debug_led_mcp 1
#define cycle_debug 0
#define ledpin 5
#define ledpin2 9
#define ledpin3 14
bool debug_led_bts = !debug_led_mcp;
bool ledstate = LOW;
bool ledstate3 = HIGH;
unsigned int interval = 250;
unsigned long waitUntil = 0;
volatile unsigned long debug_millis = 0;


// Rotary encoder related
#define encoder_pin1 3
#define encoder_pin2 4
volatile char encoder_direction = 0; // 1 == CW, -1 == CCW, 0 == Invalid
unsigned long encoder_last_time = 0;
volatile unsigned long encoder_time = 0;

// Sleep related
boolean was_asleep = 1;
boolean sleep_requested = 1; // Normally 1
unsigned long sleep_timeout = 0;


void setup() {

  // Configures Pin modes
  pinMode(MCP_SS_pin, OUTPUT);
  pinMode(MCP_INT_pin, INPUT);
  pinMode(encoder_pin1, INPUT); // Use external pullups
  pinMode(encoder_pin2, INPUT); // Use external pullups

  if (debug_led){
    pinMode(ledpin, OUTPUT);
  }
  if (debug_led){
    pinMode(ledpin2, OUTPUT);
    pinMode(ledpin3, OUTPUT);
    digitalWrite(ledpin2, LOW);
    digitalWrite(ledpin3, ledstate3);
  }

  if (debug){
    // Initialize Serial communications
    Serial.begin(115200);
    Serial.println(F("Sketch start"));
    Serial.println();
  }
  // Initialize I2C  (Step 1)
  Configure_I2C();
  recall_settings();
  
  // Initialize SPI  (Step 2)
  Configure_MCP();

  // Attach MCP interrupt pin (Step 3)
  attachInterrupt(0, MCP_interrupt, LOW);

  // Attach Rotary encoder interrupt pin (Step 4)
  attachInterrupt(1, encoder_interrupt, CHANGE);


} // ***************** Setup End *********************


void loop() {

  if (encoder_direction != 0){    // React on Rotary Encoder rotation (-1/1)
    //    unsigned long temp_micros = micros();

    sleep_timeout = millis();  // Resets sleep timeout on rotary encoder change

    int temp_time = encoder_time - encoder_last_time;

    if (temp_time > 0 && temp_time <= 25){
      encoder_direction = encoder_direction * FAST;
    }
    else if (temp_time > 26 && temp_time < 250){
      encoder_direction = encoder_direction * MED;
    }
    else if (temp_time > 251 && temp_time < 900){
      encoder_direction = encoder_direction * SLOW;
    }
    encoder_react(encoder_direction, temp_time);
    encoder_last_time = encoder_time;
    encoder_direction = 0;
  }

  if (MCP_int_triggered){  // Check for button triggered on MCP Expander
    button_id_detect();
  }

  button_timeout();        // Check for button press timeout


    if (sleep_requested && (unsigned long)(millis() - sleep_timeout) >= 5000){    // Sleep timer trigger
    sleep_timeout = millis();   
    go_to_sleep();
  }// End sleep timer trigger

  // Auto-cycle timer
  if ((unsigned long)(millis() - color_cycle_timeout) >= color_cycle_speed && led_mode == COLORCYCLE) {  // check for rollover
    color_cycle(1);
    color_cycle_timeout = color_cycle_timeout + color_cycle_speed;
  }


  if(debug_led){
    if ((unsigned long)(millis() - waitUntil) >= interval) {  // check for rollover
      ledstate = !ledstate;
      digitalWrite(ledpin, ledstate);
      waitUntil = waitUntil + interval;
    }
  } // end debug led

}
// *************** END OF LOOP **************************






























