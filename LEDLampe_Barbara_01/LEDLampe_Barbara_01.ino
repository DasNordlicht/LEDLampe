/****************************************************************
 * LED Lampe mit 4*17 LEDs
 * 2 Schaltern
 * Es sollen unterschiedliche Annimationen möglich werden
 * Es sollen feste Farben ermöglicht werden
 * -------------------------------------------------------------
 * Verwendetwerden die Lib NeoPixelBus 
 * https://github.com/Makuna/NeoPixelBus/wiki
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
 * Version 0.1.0
 *
 *****************************************************************/

/*****************************************************************
 * Includes
 *****************************************************************/
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>



/*****************************************************************
 * Defines
 *****************************************************************/
const uint16_t PixelCount = 17; // make sure to set this to the number of pixels in your strip
const uint16_t PixelPinA = 10;  // make sure to set this to the correct pin, ignored for Esp8266
const uint16_t PixelPinB = 11;  // make sure to set this to the correct pin, ignored for Esp8266
const uint16_t PixelPinC = 12;  // make sure to set this to the correct pin, ignored for Esp8266
const uint16_t PixelPinD = 13;  // make sure to set this to the correct pin, ignored for Esp8266
const uint16_t AnimCount = PixelCount / 5 * 2 + 1; // we only need enough animations for the tail and one extra

const uint16_t PixelFadeDuration = 300; // third of a second
// one second divide by the number of pixels = loop once a second
const uint16_t NextPixelMoveDuration = 1000 / PixelCount; // how fast we move through the pixels

NeoGamma<NeoGammaTableMethod> colorGamma; // for any fade animations, best to correct gamma

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> stripA(PixelCount, PixelPinA);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> stripB(PixelCount, PixelPinB);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> stripC(PixelCount, PixelPinC);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> stripD(PixelCount, PixelPinD);

struct MyAnimationState
{
    RgbColor StartingColor;
    RgbColor EndingColor;
    uint16_t IndexPixel; // Das aktuelle Pixel
};

NeoPixelAnimator animations(AnimCount); // NeoPixel animation management object
MyAnimationState animationState[AnimCount];
uint16_t frontPixel = 0;  // Start Pixel beim Loop
RgbColor frontColor;  // Start Farbei beim Loop

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
    if (TasterCounterB <= 5)
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
    TasterCounterA = 0;
    TasterModus = 4;
    if (TasterCounterB <= 5)
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

/*****************************************************************
 * Start Seed festlegen (Random)
 *****************************************************************/
void SetRandomSeed()
{
//   Serial.println("SetRandomSeed");
    uint32_t seed;

    // random works best with a seed that can use 31 bits
    // analogRead on a unconnected pin tends toward less than four bits
    seed = analogRead(0);
    delay(1);

    for (int shifts = 3; shifts < 31; shifts += 3)
    {
        seed ^= analogRead(0) << shifts;
        delay(1);
    }

    // Serial.println(seed);
    randomSeed(seed);
} 
/*****************************************************************
 * Fade Out Animation
 *****************************************************************/
 void FadeOutAnimUpdate(const AnimationParam& param)
{
// Serial.println("FadeOutAnimUpdate");
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        param.progress);
    // apply the color to the strip
    stripA.SetPixelColor(animationState[param.index].IndexPixel, colorGamma.Correct(updatedColor));
    stripB.SetPixelColor(animationState[param.index].IndexPixel, colorGamma.Correct(updatedColor));
    stripC.SetPixelColor(animationState[param.index].IndexPixel, colorGamma.Correct(updatedColor));
    stripD.SetPixelColor(animationState[param.index].IndexPixel, colorGamma.Correct(updatedColor));
}

/*****************************************************************
 * Loop Animation Update
 *****************************************************************/
void LoopAnimUpdate(const AnimationParam& param)
{
// Serial.println("LoopAnimUpdate");
    // wait for this animation to complete,
    // we are using it as a timer of sorts
    if (param.state == AnimationState_Completed)
    {
        // done, time to restart this position tracking animation/timer
        animations.RestartAnimation(param.index);

        // pick the next pixel inline to start animating
        // 
        frontPixel = (frontPixel + 1) % PixelCount; // increment and wrap
        if (frontPixel == 0)
        {
            // we looped, lets pick a new front color
            frontColor = HslColor(random(360) / 360.0f, 1.0f, 0.25f);
        }

        uint16_t indexAnim;
        // do we have an animation available to use to animate the next front pixel?
        // if you see skipping, then either you are going to fast or need to increase
        // the number of animation channels
        if (animations.NextAvailableAnimation(&indexAnim, 1))
        {
            animationState[indexAnim].StartingColor = frontColor;
            animationState[indexAnim].EndingColor = RgbColor(0, 0, 0);
            animationState[indexAnim].IndexPixel = frontPixel;

            animations.StartAnimation(indexAnim, PixelFadeDuration, FadeOutAnimUpdate);
        }
    }
} 
/*****************************************************************
 * Color Wipe
 * Fülle die Pixel mit einer Farbe 
 *****************************************************************/
//// Fill the dots one after the other with a color
//void colorWipe(uint32_t c, uint8_t wait) {
//  for(uint16_t i=0; i<strip.numPixels(); i++) {
//    strip.setPixelColor(i, c);
//    strip.show();
//    delay(wait);
//  }
//}
/*****************************************************************
 * Rainbow Animation
 *****************************************************************/
//void rainbow(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256; j++) {
//    for(i=0; i<stripA.PixelCount(); i++) {
//      stripA.SetPixelColor(i, RgbColor(0, 0, 0));
//      stripB.SetPixelColor(i, RgbColor(0, 0, 0));
//      stripC.SetPixelColor(i, RgbColor(0, 0, 0));
//      stripD.SetPixelColor(i, RgbColor(0, 0, 0));
//    }
//    stripA.Show();
//    stripB.Show();
//    stripC.Show();
//    stripD.Show();
//    delay(wait);
//  }
//}

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
  stripA.Begin();
  stripA.Show();
  stripB.Begin();
  stripB.Show();
  stripC.Begin();
  stripC.Show();
  stripD.Begin();
  stripD.Show();

  SetRandomSeed();

  animations.StartAnimation(0, NextPixelMoveDuration, LoopAnimUpdate);

}
/*****************************************************************
 * Das Hauptprogramm, der Loop der die eigentliche Basis der 
 * Funktionen der Lampe bildet.
 * Hier werden die Annimationen an die Stripes angezeigt und die 
 * Schalter abgefragt.
 *****************************************************************/

void loop()
{
switch(TasterModus) {

  case 0: { 
          animations.UpdateAnimations();
          stripA.Show();
          stripB.Show();
          stripC.Show();
          stripD.Show();
          };
          break;
  case 1: colorWipe(AstripA.Color(255, 0, 0), 50);  // Red
          break;
  case 2: colorWipe(AstripA.Color(0, 255, 0), 50);  // Green
          break; 
  case 3: colorWipe(AstripA.Color(0, 0, 255), 50);  // Blue
          break;
  case 4: colorWipe(AstripA.Color(255, 255, 255), 50);  // Blue
          break;  
  }

    Taster_abfragen();

    Serial.print("SchalterStatus ");
    Serial.println(SchalterStatus);
    Serial.print("Modus ");
    Serial.println(TasterModus);
    Serial.print("TasterCounterA ");
    Serial.println(TasterCounterA);
    Serial.print("TasterCounterB ");
    Serial.println(TasterCounterB);




}

