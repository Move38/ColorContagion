/*
 * Blinks - Color Contagion
 * 
 * If alone, glow white
 * Otherwise transition to neighboring color
 * Holdoff after transitioning to neighboring color for length of transition
 * 
 */

#include "blinklib.h"
#include "blinkstate.h"

Color colors[] = { RED , CYAN , MAGENTA, YELLOW, BLUE, GREEN };
byte hues[] = { 0 , 126, 42, 168, 84, 210};
uint32_t timeLastChanged_ms = 0;
uint32_t changeHoldoff_ms = 1000;
uint32_t changeDelay_ms = 200;  // create a delay in the wave of color
uint32_t colorFadeDuration_ms = 2000; // fade to our new color over time
byte startHue, endHue;

void setup() {
  setState(1);
  setColor( makeColorHSB(hues[getState()-1], 255, 255) );
  startHue = hues[getState()-1];
  endHue = hues[getState()-1];
}

void loop() {
    
  int numNeighbors = 0;
  uint32_t curTime_ms = millis();

  if( buttonSingleClicked() ) {
        startHue = hues[getState()-1];
        setState( (getState() % 6) + 1 );
        endHue = hues[getState()-1];
        timeLastChanged_ms = curTime_ms;
  }

  for( int i = 0; i < 6; i++ ) {
    
    byte neighborState = getNeighborState(i);
    
    if(neighborState != 0) {

      numNeighbors++;
      
      if(neighborState != getState() && curTime_ms - timeLastChanged_ms > changeHoldoff_ms){
        startHue = hues[getState()-1];
        setState( neighborState );
        endHue = hues[getState()-1];
        timeLastChanged_ms = curTime_ms;
      }
    }
  }
  
  if(numNeighbors == 0) {
    // alone, so glow white
    setColor(WHITE);
  }
  else {
    // if is transitioning do so
    if(curTime_ms - timeLastChanged_ms < colorFadeDuration_ms) {
      // linearly interpolate over HSB to arrive at new color
      uint32_t delta_ms = curTime_ms - timeLastChanged_ms;
      byte hue = map(delta_ms, 0, colorFadeDuration_ms, uint32_t(startHue), uint32_t(endHue));
      setColor( makeColorHSB(hue,255,255) );
    }
    else {
      // else show current color
      setColor( makeColorHSB(hues[getState()-1], 255, 255) );
    }
  }

}

byte map( uint32_t var, uint32_t leftBound, uint32_t rightBound, uint32_t from, uint32_t to) {
  
  float progress, mappedValue;

  if(from < to) {
  
    progress = float(var - leftBound) / float(rightBound - leftBound);
  
    mappedValue = progress * float(to - from) + float(from);
  }
  else {
  
    progress = float(var - leftBound) / float(rightBound - leftBound);
  
    mappedValue = float(from) - progress * float(from - to);
  }
  
  
  return byte(mappedValue); 
}

