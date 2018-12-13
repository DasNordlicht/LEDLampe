/****************************************************************
 * LED Lampe mit 4*17 LEDs
 * 2 Schaltern
 * Es sollen unterschiedliche Annimationen möglich werden
 * Es sollen feste Farben ermöglicht werden
 * -------------------------------------------------------------
 * Verwendetwerden die Lib Adafruit_NeoPixel 
 * 
 * -------------------------------------------------------------
 * Lizenzierung 
 * GNU Lesser General Public License v3.0 
 * -------------------------------------------------------------
 * Entwickelt für eine Lampe von Barbara Lamminger
 * Geschreiben von Frank Radzio
 * Web https://nucleon-ev.eu
 * Mail Frank.Radzio@nucleon-ev.eu
 * -------------------------------------------------------------
 * Stand 11.12.2018
 * Version 0.2.0
 *
 *****************************************************************/

/*****************************************************************
 * Includes
 *****************************************************************/
#include <Adafruit_NeoPixel.h>



/*****************************************************************
 * Defines
 *****************************************************************/
const uint16_t PixelCount = 17; // make sure to set this to the number of pixels in your strip
const uint16_t PixelPinA = 10;  // make sure to set this to the correct pin, ignored for Esp8266
const uint16_t PixelPinB = 11;  // make sure to set this to the correct pin, ignored for Esp8266
const uint16_t PixelPinC = 12;  // make sure to set this to the correct pin, ignored for Esp8266
const uint16_t PixelPinD = 13;  // make sure to set this to the correct pin, ignored for Esp8266

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel stripA = Adafruit_NeoPixel(PixelCount, PixelPinA, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB = Adafruit_NeoPixel(PixelCount, PixelPinB, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripC = Adafruit_NeoPixel(PixelCount, PixelPinC, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripD = Adafruit_NeoPixel(PixelCount, PixelPinD, NEO_GRB + NEO_KHZ800);

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

/*****************************************************************
 *  Definitionen rund um die Schalter
 *****************************************************************/
int TasterA=2;
int TasterB=3;
int Schalter=4;
int TasterStatusA=0;
int TasterStatusB=0;
int SchalterStatus=0;
int TasterCounterA=0;
int TasterCounterB=0;
int TasterModus=0;
int setA=0; //Entprellen der Taster
int setB=0; //Entprellen der Taster

#define BRIGHTNESS 30


/*****************************************************************
 * Der Bereich der Funktionen
 *****************************************************************/

/*****************************************************************
 * Schalter Abfragen
 *****************************************************************/
void Taster_abfragen()
{
// Schalter Abfragen
//Serial.print("TasterModus ");
//Serial.println(TasterModus);
SchalterStatus=digitalRead(Schalter);
TasterStatusA=digitalRead(TasterA);
TasterStatusB=digitalRead(TasterB);
//Serial.print("Schalter ");
if (SchalterStatus == HIGH)
{
  if (TasterStatusA == HIGH  && setA == 0)
  {
    TasterModus = 1;
    TasterCounterB = 0;
    if (TasterCounterA <= 5)
    {
      TasterCounterA++;
      setA = 1;
    } else {
      TasterCounterA = 0;
      setA = 1;
    }
  }
  
  if (TasterStatusA == LOW && setA == 1)
  {
    setA = 0;
  }

  if (TasterStatusB == HIGH && setB == 0)
  {
    TasterModus = 2;
    TasterCounterA = 0;
    if (TasterCounterB <= 6)
    {
      TasterCounterB++;
      setB = 1;
    } else {
      TasterCounterB = 0;
      setB = 1;
    }
  } 

  if (TasterStatusB == LOW && setB == 1)
  {
    setB = 0;
  }
   
} else {

  if (TasterStatusA == HIGH && setA == 0)
  {
    TasterModus = 3;
    TasterCounterB = 0;
    if (TasterCounterA <= 11)
    {
      TasterCounterA++;
      setA = 1;
    } else {
      TasterCounterA = 0;
      setA = 1;
    }
 
  } 
  if (TasterStatusA == LOW && setA == 1)
  {
    setA = 0;
  }
  
  if (TasterStatusB == HIGH && setB == 0)
  {
    TasterCounterA = 0;
    TasterModus = 4;
    if (TasterCounterB <= 6)
    {
      TasterCounterB++;
      setB = 1;      
    } else {
      TasterCounterB = 0;
    }
 
  } 
  if (TasterStatusB == LOW && setB == 1)
  {
    setB = 0;  
  }
}
  
}
/*****************************************************************
 * Start Animation als Gadget
 *****************************************************************/

/*****************************************************************
 * Temperatur erfassen um eine Start Farbe zu setzen
 *****************************************************************/
/****************************************************************
 * Meteor
 *****************************************************************/
 void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
 
  for(int i = 0; i < PixelCount+PixelCount; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<PixelCount; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <PixelCount) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      }
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = stripA.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    stripA.setPixelColor(ledNo, r,g,b);
    stripB.setPixelColor(ledNo, r,g,b);
    stripC.setPixelColor(ledNo, r,g,b);
    stripD.setPixelColor(ledNo, r,g,b);
 
}
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < PixelCount; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}
/***************************************************************** 
 *  Fire
 *****************************************************************/
void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[PixelCount];
  int cooldown;
 
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < PixelCount; i++) {
    cooldown = random(0, ((Cooling * 10) / PixelCount) + 2);
   
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
 
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= PixelCount - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
   
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < PixelCount; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  showStrip();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) {

   // NeoPixel
   stripA.setPixelColor(Pixel, stripA.Color(red, green, blue));
   stripB.setPixelColor(Pixel, stripB.Color(red, green, blue));
   stripC.setPixelColor(Pixel, stripC.Color(red, green, blue));
   stripD.setPixelColor(Pixel, stripD.Color(red, green, blue));
   
} 

void showStrip() {

   stripA.show();
   stripB.show();
   stripC.show();
   stripD.show();

}

/*****************************************************************
 * Kaminfeuer
 *****************************************************************/
void kaminfeuer(int r,int g,int b,int newbright,int mflic) {

//int r = 255;
//int g = 80; //96
//int b = 11; //12
//int newbright = 40;
  stripA.setBrightness(newbright);
  stripB.setBrightness(newbright);
  stripC.setBrightness(newbright);
  stripD.setBrightness(newbright);

for(int x = 0; x <stripA.numPixels(); x++)
{
int flicker = random(0,mflic);
int r1 = r-flicker;
int g1 = g-flicker;
int b1 = b-flicker;
if(g1<0) g1=0;
if(r1<0) r1=0;
if(b1<0) b1=0;
stripA.setPixelColor(x,r1,g1, b1);
stripB.setPixelColor(x,r1,g1, b1);
stripC.setPixelColor(x,r1,g1, b1);
stripD.setPixelColor(x,r1,g1, b1);
}
stripA.show();
stripB.show();
stripC.show();
stripD.show();
delay(random(50,150));
}
/*****************************************************************
 * pulseWhite
 *****************************************************************/
void pulseWhite(uint8_t wait) {
  for(int j = 0; j < 256 ; j++){
      for(uint16_t i=0; i<stripA.numPixels(); i++) {
         stripA.setPixelColor(i, stripA.Color(0,0,0, neopix_gamma[j] ) );
         stripB.setPixelColor(i, stripB.Color(0,0,0, neopix_gamma[j] ) );
         stripC.setPixelColor(i, stripC.Color(0,0,0, neopix_gamma[j] ) );
         stripD.setPixelColor(i, stripD.Color(0,0,0, neopix_gamma[j] ) );
        }
        delay(wait);
        stripA.show();
        stripB.show();
        stripC.show();
        stripD.show();
      }

  for(int j = 255; j >= 0 ; j--){
      for(uint16_t i=0; i<stripA.numPixels(); i++) {
         stripA.setPixelColor(i, stripA.Color(255,255,255, neopix_gamma[j] ) );
         stripB.setPixelColor(i, stripB.Color(255,255,255, neopix_gamma[j] ) );
         stripC.setPixelColor(i, stripC.Color(255,255,255, neopix_gamma[j] ) );
         stripD.setPixelColor(i, stripD.Color(255,255,255, neopix_gamma[j] ) );
        }
        delay(wait);
        stripA.show();
        stripB.show();
        stripC.show();
        stripD.show();
      }
}

/*****************************************************************
 * rainbowCycle
 *****************************************************************/
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 1 cycles of all colors on wheel
    for(i=0; i< stripA.numPixels(); i++) {
      stripA.setPixelColor(i, Wheel(((i * 256 / stripA.numPixels()) + j) & 255));
      stripB.setPixelColor(i, Wheel(((i * 256 / stripB.numPixels()) + j) & 255));
      stripC.setPixelColor(i, Wheel(((i * 256 / stripC.numPixels()) + j) & 255));
      stripD.setPixelColor(i, Wheel(((i * 256 / stripD.numPixels()) + j) & 255));
    }
    stripA.show();
    stripB.show();
    stripC.show();
    stripD.show();
    delay(wait);
  }
}

/*****************************************************************
 * Wheel
 *****************************************************************/
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return stripA.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return stripA.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return stripA.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
/*****************************************************************
 * Color Wipe
 * Fülle die Pixel mit einer Farbe 
 *****************************************************************/
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<stripA.numPixels(); i++) {
    stripA.setPixelColor(i, c);
    stripB.setPixelColor(i, c);
    stripC.setPixelColor(i, c);
    stripD.setPixelColor(i, c);
    stripA.show();
    stripB.show();
    stripC.show();
    stripD.show();
    delay(wait);
  }
}
/*****************************************************************
 * Rainbow Animation
 *****************************************************************/
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<stripA.numPixels(); i++) {
      stripA.setPixelColor(i, Wheel((i+j) & 255));
      stripB.setPixelColor(i, Wheel((i+j) & 255));
      stripC.setPixelColor(i, Wheel((i+j) & 255));
      stripD.setPixelColor(i, Wheel((i+j) & 255));
    }
    stripA.show();
    stripB.show();
    stripC.show();
    stripD.show();
    delay(wait);
  }
}

/*****************************************************************
 * Das Setup
 * Hier werden die Einzelnen Stripes eingerichtet und die 
 * Start Parameter festgelegt
 *****************************************************************/

void setup()
{
      Serial.begin(115200);
      Serial.println("Start");

  pinMode(TasterA, INPUT);
  pinMode(TasterB, INPUT);
  pinMode(Schalter, INPUT);
  stripA.setBrightness(BRIGHTNESS);
  stripB.setBrightness(BRIGHTNESS);
  stripC.setBrightness(BRIGHTNESS);
  stripD.setBrightness(BRIGHTNESS);
  stripA.begin();
  stripA.show();
  stripB.begin();
  stripB.show();
  stripC.begin();
  stripC.show();
  stripD.begin();
  stripD.show();


}
/*****************************************************************
 * Das Hauptprogramm, der Loop der die eigentliche Basis der 
 * Funktionen der Lampe bildet.
 * Hier werden die Annimationen an die Stripes angezeigt und die 
 * Schalter abgefragt.
 *****************************************************************/

void loop()
{
      Taster_abfragen();
switch(TasterModus) {

  case 0: kaminfeuer(255,70,10,25,80);
          //Fire(65,100,25);
          //meteorRain(0xff,0xff,0xff,5, 64, true, 30);
          break;
  case 1: switch(TasterCounterA) {
          case 0:rainbow(5);
                 break;
          case 1:rainbow(5);
                 break;
          case 2:rainbowCycle(2);
                 break;
          case 3:rainbowCycle(5);
                 break;
          case 4:meteorRain(0xff,0xff,0xff,5, 64, true, 30);
                 break;
          case 5:meteorRain(0xff,0xff,0xff,3, 32, true, 30);
                 break;
          case 6:rainbow(5);
                 break;
          }
          colorWipe(stripA.Color(255, 0, 0), 10);
          break;
  case 2: switch(TasterCounterB) {
          case 0:rainbow(5);
                 break;
          case 1:rainbow(5);
                 break;
          case 2:rainbowCycle(2);
                 break;
          case 3:rainbowCycle(5);
                 break;
          case 4:rainbow(5);
                 break;
          case 5:rainbow(5);
                 break;
          case 6:rainbow(5);
                 break;
          case 7:rainbow(5);
                 break;
          }
          colorWipe(stripA.Color(0, 255, 0), 10);
          break;
  case 3: switch(TasterCounterA) {
          case 0: colorWipe(stripA.Color(0, 0, 255), 10);  // blau
          break;     
          case 1: colorWipe(stripA.Color(255, 255, 0), 10);  // gelb
          break;     
          case 2: colorWipe(stripA.Color(127, 0, 255), 10);  // lila
          break;     
          case 3: colorWipe(stripA.Color(128, 255, 0), 10);  // gelbgrün
          break;     
          case 4: colorWipe(stripA.Color(255, 0, 255), 10);  // pink
          break;     
          case 5: colorWipe(stripA.Color(0, 255, 0), 10);  // grün
          break;     
          case 6: colorWipe(stripA.Color(255, 0, 128), 10);  // magenta
          break;     
          case 7: colorWipe(stripA.Color(0, 255, 127), 10);  // blaugrün
          break;     
          case 8: colorWipe(stripA.Color(255, 0, 0), 10);  // rot
          break;     
          case 9: colorWipe(stripA.Color(0, 255, 0), 10);  // 
          break;     
          case 10: colorWipe(stripA.Color(255, 127, 0), 10);  // 
          break;     
          case 11: colorWipe(stripA.Color(0, 128, 255), 10);  // Black/off
          break;     
          case 12: colorWipe(stripA.Color(127, 255, 127), 10);  // Black/off
          break;     
          }
          colorWipe(stripA.Color(255, 255, 255), 10); 
          break;  

  case 4: switch(TasterCounterB) {
          case 0: kaminfeuer(255,80,11,40,70);
          break;     
          case 1: kaminfeuer(255,80,11,40,70);
          break;     
          case 2: Fire(55,120,15);
          break;     
          case 3: Fire(75,120,15);
          break;     
          case 4: Fire(35,120,15);
          break;     
          case 5: Fire(55,90,15);
          break;     
          case 6: Fire(45,180,5);
          break;     
          case 7: kaminfeuer(255,70,10,25,80);
          break;     
          }
          pulseWhite(2);
          break;  
  }



    Serial.print("SchalterStatus ");
    Serial.println(SchalterStatus);
    Serial.print("Modus ");
    Serial.println(TasterModus);
    Serial.print("TasterCounterA ");
    Serial.println(TasterCounterA);
    Serial.print("TasterCounterB ");
    Serial.println(TasterCounterB);




}

