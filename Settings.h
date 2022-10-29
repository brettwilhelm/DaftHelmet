/* Definitions and Constants */
#define DEBUG true
#define MatrixLeft 0
#define MatrixRight 1

/* Hardware Buttons */
#define BUTTONS true
#define ButtonOnOff 15
#define ButtonOnOffClosed HIGH
#define ButtonMode 13
#define ButtonModeClosed LOW

#define SpeedSlow 100
#define SpeedMedium 50
#define SpeedFast 30

#define ModeText 0
#define ModeTrafficAdvisor 1
#define ModeScanner 2
#define ModeEQ 3
#define ModeEyes 4
#define ModeCount 5
const char *my_modes[]        = { "Text", "Traffic Advisor", "Scanner", "Fake Equalizer", "Eyes" };

#define TextCount 5
const char *TextName[]        = { "Daft Helmet", "Happy Halloween", "Trick or Treat", "Party At The Cabin", "Spooktacular" };
const uint8_t *TextData[]     = { tDaftHelmet, tHappyHalloween, tTrickOrTreat, tPartyAtTheCabin, tSpooktacular };
const uint8_t TextSize[]      = { sizeof(tDaftHelmet), sizeof(tHappyHalloween), sizeof(tTrickOrTreat), sizeof(tPartyAtTheCabin), sizeof(tSpooktacular) };

/* WiFi Configuration */
const char* ap_ssid     = "Daft_Helmet";
const char* ap_password = "Daft_Helmet";

/* Settings */
int my_brightness             = 5; /* 1 to 15 */
int my_speed                  = SpeedMedium; /* delay speed */
int my_mode                   = ModeScanner; /* current visor mode */
int my_buttonmode             = my_mode; /* current visor button mode */
int my_text                   = 0; /* default text (array index) */
bool isOn                     = true; /* is visor on */

/* Variables */
int lhY                       = 0; /* see MatrixInitCoordinates() */
int rhY                       = 0; /* see MatrixInitCoordinates() */
Adafruit_8x16matrix Matrix[2] = { Adafruit_8x16matrix(), Adafruit_8x16matrix() };
