// MCP23008 / MCP23S08 Address, Registers, etc...
#define MCP23x08 0x40       // Address with R/W bit cleared
#define MCP_IODIR 0x00      // Port direction, 1 = INPUT
#define MCP_IPOL 0x01       // Port polarity inversion, 1 = INVERTED
#define MCP_GPINTEN 0x02    // Port Interrupt-On-Change, 1 = ENABLED
#define MCP_DEFVAL 0x03     // Interrupt-On-Change compare 
#define MCP_INTCON 0x04     // Interrupt-On-Change control 1 = DEFVAL, 0 = Previous value
#define MCP_IOCON 0x05      // Configuration Register
#define MCP_GPPU 0x06       // Port pull-up resistor (If Port = Input) 1 = Enabled
#define MCP_INTF 0x07       // Interrupt Flag 1 = Port caused the interrupt
#define MCP_INTCAP 0x08     // Interrupt capture ( See note in datasheet in INTF)
#define MCP_GPIO 0x09       // General Input/Output   Read port state, Write port latches 
#define MCP_OLAT 0x0A       // Output port latch, 1 = HIGH
#define MCP_INT_pin 2
#define MCP_SS_pin 10
#define MCP_short_time 200

// "backend" button handling
#define NONE 0
#define SINGLE 2
#define LONG 3
#define DOUBLE 42
#define HOLD 8
#define PRESSED 1
volatile unsigned char MCP_INTF_read = 0;
volatile unsigned char MCP_INTCAP_read = 0;
volatile bool MCP_int_triggered = 0;
char MCP_prev_int = -1;
bool MCP_port_state[8];
unsigned char mp_counter[8] = {
  0};


// Buttons timing/type handling
#define mp_timeout 200        // Timeout value for double click detection
#define press_timeout 1500       // Timeout value for button hold
unsigned char press_type = 0;    // Detected button press type 
unsigned long MCP_press_time[8] = {
  0};
unsigned char hold_triggered[8] = {
  0};
unsigned char MCP_press_type = 0;

// Rotary encoder
#define SLOW 1
#define MED 2
#define FAST 3

// PCA96x5 led driver Address, Registers, etc...
#define PCA96x5 0x20          // PCA96x5 chip I2C address
#define PCA_RESET 0x03        // PCA96x5 SOFTWARE_RESET I2C address
#define PCA_ALLCALL 0x70      // PCA96x5 LED_ALL_CALL I2C address
#define PCA_MODE1 0x00        // PCA96x5 MODE1 Register address
#define PCA_MODE2 0x01        // PCA96x5 MODE2 Register address
#define PCA_GRPPMW 0x12       // PCA9635 GRPPMW Register address
#define PCA_NFET_OUT 1        // Sets operational mode for N-Channel output drivers


// LED Controls
#define total_channels 5
int led_mode = -1; // I give up, using modes again... 1 =  white, 2 = manual cycle, 3 = autocycle,
#define last_mode 2
#define WHITE 0
#define COLORS 1
#define COLORCYCLE 2
unsigned char COLORLINK = last_mode + 1;

bool strips_pwr_state = 0;    // Power state of RGB strips channels // add 0 - 1 - 2 for white/color ?
bool lightring_pwr_state = 0;  // lightring leds power state
bool output4_enabled = 0;
int RGB_brightness = 50;
int rgb_channel[total_channels][3] = { // Channels 0 and 1 reversed for testin
  {
     0x02,0x03,0x04     } // lightring dont change
  ,
  {
     0x0C,0x0A,0x0E     }  // test board purple
  ,
  {
    0x08, 0x10, 0x06      }  // test board red
  ,
  {
    0xFF,0xFF,0xFF      }   // not to mess with the figuring out process...
  ,
  {
    0xFF,0xFF,0xFF      }
};

int rgb_colors[5][3] = {  // [channel][value] adjust according to wiring
  {
    0xFF,0x00,0x00      }
  ,
  {
    0x00,0x00,0xFF      }
  ,
  {
    0xFF,0x00,0x00      }
  ,
  {
    0xFF,0xFF,0x00      }
  ,
  {
    0xFF,0x00,0xFF      }
};
int white[4] = { // Red/Green/Blue/Brightness
  255, 169, 93, 100};

// temporary color cycling additions
#define off 0
#define on 255

#define OFF 0
#define ON 1
int color_cycle_speed = 0;           // RGB color cycle speed
unsigned long color_cycle_timeout = 0;
int color_cycle_step = 1;           // RGB color cycle step
//int cycle_count[total_channels] = {
//  3};
int cycle_count[total_channels] = { 3 };
int cycle_counter[total_channels] = { 0 };
int cycle_counter_top = on; // Count up to what for the fade
unsigned char incColor;                   // RGB Color to increase in RGB cycle mode
unsigned char decColor;                   // RGB Color to decrease in RGB cycle mode
unsigned char noColor;                    // RGB Color to turn off in RGB cycle mode


