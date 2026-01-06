// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <SdFat.h>
#include <SdFatConfig.h>
#include <MinimumSerial.h>
#include <FreeStack.h>
#include <BlockDriver.h>
#include <SysCall.h>
#include <string>
#include <map>
#define CONF_FILE "leds.ini"    //configuration file
#define CHIP_SD D6    //pin sd


// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            D4

// How many NeoPixels are attached to the Arduino?
//#define NUMPIXELS      14

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1000, PIN, NEO_GRB + NEO_KHZ800);


//-------------------------define rgb struct----------------------------------->

typedef struct rgb {
    int r;
    int g;
    int b;
} RGB;
//-------------------------define rgb struct-----------------------------------<

//-------------------------------------globals--------------------------------->
int delayval = 100; // delay for half a second
int interval = 500;
int numleds = 1;
int numsteps = 2;
//std::list<RGB> ledlist;
std::map<String, rgb> ledlist;

SdFat SD;
static File settingFile;
static bool sdPresent = false;
char arraykey[32] = "0000000000000000000000000000000";

//-------------------------------------globals---------------------------------<

/**
 * 
 */
static bool CONF_is_valid_char(char character) {
    if (isalnum(character) || character == '_' || character == ',') {
        return true;
    }
    return false;
}

/**
 * 
 */
static int CONF_getConfValueInt(char* filename, char* key, int defaultValue = 0) {
    File myFile;
    char character;
    String description = "";
    String value = "";
    boolean valid = true;
    int ret = defaultValue;
    if (!SD.exists(filename)) {
        //Serial.println("file not exists");
        return defaultValue;
    }
    myFile = SD.open(filename);

    while (myFile.available()) {
        description = "";
        character = myFile.read();

        //cerca una linea valida----->
        if (!CONF_is_valid_char(character)) {
            // Comment - ignore this line
            while (character != '\n' && myFile.available()) {
                character = myFile.read();
            };
            continue;
        }
        //cerca una linea valida-----<
        //----riempo la descrizione---->
        do {
            description.concat(character);
            character = myFile.read();
            if (!myFile.available()) {
                myFile.close();
                return defaultValue;
            }
        } while (CONF_is_valid_char(character));
        //----riempo la descrizione----<
        //-------elimino gli spazi------->
        if (character == ' ') {
            do {
                character = myFile.read();
                if (!myFile.available()) {
                    myFile.close();
                    return defaultValue;
                }
            } while (character == ' ');
        }
        //-------elimino gli spazi-------<
        if (character == '=') {
            if (description == key) {
                //riempo il valore
                //Serial.println("trovato... scorro:");
                //-------elimino gli spazi------->
                do {
                    character = myFile.read();
                    if (!myFile.available()) {
                        myFile.close();
                        return defaultValue;
                    }
                } while (character == ' ');
                //-------elimino gli spazi-------<
                value = "";
                valid = true;
                while (character != '\n' && character != '\r') {
                    //Serial.println(character);
                    if (isdigit(character)) {
                        value.concat(character);
                    } else if (character != '\n' && character != '\r') {
                        // Use of invalid values
                        valid = false;
                    }
                    character = myFile.read();
                };
                if (valid) {
                    // Convert chars to integer
                    ret = std::atoi(value.c_str());
                    myFile.close();
                    return ret;
                }
            }
        } else {
            while (character != '\n' && myFile.available()) {
                character = myFile.read();
            };
            continue;
        }
    }

    return ret;
}

/**
 * 
 */
static rgb CONF_getConfValueRgb(char* filename, char* key) {
    File myFile;
    char character;
    String description = "";
    String value = "";
    boolean valid = true;
    rgb defaultValue;
    rgb ret;
    defaultValue.r = 1;
    defaultValue.g = 2;
    defaultValue.b = 3;

    if (!SD.exists(filename)) {
        Serial.println("file not exists");
        return defaultValue;
    }
    myFile = SD.open(filename);
    while (myFile.available()) {
        description = "";
        character = myFile.read();
        //cerca una linea valida----->
        if (!CONF_is_valid_char(character)) {
            // Comment - ignore this line
            while (character != '\n' && myFile.available()) {
                character = myFile.read();
            };
            continue;
        }
        //cerca una linea valida-----<
        //----riempo la descrizione---->
        do {
            description.concat(character);
            character = myFile.read();
            if (!myFile.available()) {
                myFile.close();
                Serial.println("key not exists");
                return defaultValue;
            }
        } while (character != '=');
        //----riempo la descrizione----<
        if (character == '=') {
            if (description == key) {
                //riempo il valore
                /*Serial.print(F("trovato..."));
                Serial.print(F(description));
                Serial.println(" scorro");*/
                //-------elimino gli spazi------->
                do {
                    character = myFile.read();
                    if (!myFile.available()) {

                        Serial.println(F("errore valore"));
                        myFile.close();
                        return defaultValue;
                    }
                } while (character == ' ');
                //-------elimino gli spazi-------<
                valid = true;
                //red----------------------------->
                value = "";
                while (character != ',') {
                    //Serial.println(character);
                    if (isdigit(character)) {
                        value.concat(character);
                    } else if (character != '\n' && character != '\r' && character != ',') {
                        // Use of invalid values
                        valid = false;
                    }
                    character = myFile.read();
                };
                if (valid) {
                    // Convert chars to integer
                    ret.r = std::atoi(value.c_str());
                } else {
                    Serial.println(F("valore r non valido"));
                    Serial.flush();
                    myFile.close();
                    return defaultValue;
                }
                //red-----------------------------<
                //green--------------------------->
                value = "";
                character = ' ';
                while (character != ',') {
                    //Serial.println(character);
                    if (isdigit(character)) {
                        value.concat(character);
                    } else if (character != '\n' && character != '\r' && character != ' ') {
                        // Use of invalid values
                        valid = false;
                    }
                    character = myFile.read();
                };
                if (valid) {
                    // Convert chars to integer
                    ret.g = std::atoi(value.c_str());
                } else {
                    Serial.println(F("valore g non valido"));
                    Serial.flush();
                    myFile.close();
                    return defaultValue;
                }

                //blue
                value = "";
                character = ' ';
                //Serial.println(character);
                while (character != '\n' && character != '\r') {
                    //Serial.println(character);
                    if (isdigit(character)) {
                        value.concat(character);
                    } else if (character != '\n' && character != '\r' && character != ' ') {
                        // Use of invalid values
                        valid = false;
                    }
                    character = myFile.read();
                    //Serial.println(character);
                };
                if (valid) {
                    // Convert chars to integer
                    ret.b = std::atoi(value.c_str());
                    Serial.println(F("valore rgb valido"));
                    Serial.flush();
                    myFile.close();
                    return ret;
                } else {
                    Serial.println(F("valore b non valido"));
                    Serial.flush();
                    myFile.close();
                    return defaultValue;
                }

            } else {
                while (character != '\n' && myFile.available()) {
                    character = myFile.read();
                };
                continue;
            }
        }
    }
    return ret;
}

/**
 *
 */
static void DL_initConf() {
    if (sdPresent) {
        if (!SD.exists(CONF_FILE)) {
            Serial.print(CONF_FILE);
            Serial.println(F(" not exist"));
            settingFile = SD.open(CONF_FILE, FILE_WRITE);
            if (settingFile) {
                //create new file config.ini------>
                /*interval=1000
numleds=1
numsteps=2
step1_led1=255,0,0*/
                Serial.print(F("new file"));
                Serial.println(CONF_FILE);
                settingFile.println(F("numleds=1"));
                settingFile.println(F("numsteps=2"));
                settingFile.println(F("interval=500"));
                settingFile.println(F("step1_led1=255,0,0"));
                settingFile.println(F("step2_led1=0,255,0"));
                settingFile.flush();
                settingFile.close();
                //create new file config.ini------<
            } else {
                Serial.print(F("error to create "));
                Serial.println(CONF_FILE);
            }
        } else {
            Serial.print(CONF_FILE);
            Serial.println(F(" already exist"));
        }
        Serial.flush();
        //read from CONFIG.INI------->
        interval = CONF_getConfValueInt(CONF_FILE, F("interval"), 500);
        numsteps = CONF_getConfValueInt(CONF_FILE, F("numsteps"), 1);
        numleds = CONF_getConfValueInt(CONF_FILE, F("numleds"), 2);


        Serial.print(F("Interval(ms)="));
        Serial.println(interval);
        Serial.print(F("numsteps="));
        Serial.println(numsteps);
        Serial.print(F("numleds="));
        Serial.println(numleds);
        Serial.flush();

        for (int step = 1; step <= numsteps; step++) {
            for (int i = 1; i <= numleds; i++) {
                rgb val;
                //compongo la chiave es. step1_led1
                sprintf(arraykey, F("step%d_led%d"), step, i);
                val = CONF_getConfValueRgb(CONF_FILE, arraykey);
                Serial.print("key=");
                Serial.print(arraykey);
                Serial.print(" val=");
                Serial.print(val.r);
                Serial.print(",");
                Serial.print(val.g);
                Serial.print(",");
                Serial.println(val.b);
                Serial.print(",");
                ledlist[arraykey] = val;
                Serial.flush();
            }
        }
        Serial.println("fine lettura file");
        Serial.flush();
        //read from CONFIG.INI-------<
    }
}




/**
 * 
 */
void setup() {
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // End of trinket special code

    sdPresent = false;
    //--init serial------------------------------>
    Serial.begin(9600);
    //--init serial------------------------------<

    // initialize the SD card ------------------->
    //Serial.println("Initializing SD");    
    pinMode(CHIP_SD, OUTPUT);
    Serial.print(F("SD initialization "));
    if (!SD.begin(CHIP_SD)) {
        sdPresent = false;
        Serial.println(F("failed"));
    } else {
        Serial.println(F("successful"));
        sdPresent = true;
    }
    Serial.flush();
    DL_initConf();
//    pixels = Adafruit_NeoPixel(numleds, PIN, NEO_GRB + NEO_KHZ800);
    // initialize the SD card -------------------<
    pixels.begin(); // This initializes the NeoPixel library.
    Serial.println(F("init pixel"));
}

/**
 * 
 */
void loop() {
    int r, g, b;
    int idpx;
    // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
    rgb val;
    Serial.println(F("loop"));
    //ledlist
    for (int step = 1; step <= numsteps; step++) {
        
        for (int i = 1; i <= numleds; i++) {
            //step1_led1
            sprintf(arraykey, F("step%d_led%d"), step, i);
           // Serial.print(arraykey);
            val = ledlist[arraykey];
          //  Serial.print(":");
          //  Serial.print(val.r);
          //  Serial.print(" ");
          //  Serial.print(val.g);
          //  Serial.print(" ");
          //  Serial.println(val.b);
            r = val.r;
            g = val.g;
            b = val.b;
            // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
           // Serial.print(i);
            idpx = i - 1;
            //se setto un px minore di 2 si pianta
            /*if (idpx > 1)*/ {
                pixels.setPixelColor(idpx, pixels.Color(r, g, b)); // Moderately bright green color.
            }
        }
        pixels.show();
        delay(interval); // Delay for a period of time (in milliseconds).
        //          delay(25); 
    }
    pixels.show();

}
