  #include <Arduino.h>
  #include "rotary.h"
  
  /*
  pinMode(rswitch_sw_pin, INPUT);
  pinMode(rswitch_clk_pin, INPUT);
  pinMode(rswitch_dat_pin, INPUT);
*/

  /**
 * @brief Get the Rotary Switch Event object
 * 
 * @return unsigned char Rotary switch event
 */
signed char getRotarySwitchEvent(unsigned char rswitch_clk_pin, unsigned char rswitch_dat_pin, unsigned char rswitch_sw_pin){

  unsigned char encoder_pin = 0;
  static unsigned char old_encoder_pin = 1;
  unsigned char rotation_direction = 0;
  unsigned char sw_event=0;
  unsigned char rotary_sw_event =0;
  unsigned char old_rotary_sw_event=0;
  signed char encoderAction=-1;

  //*****
  encoder_pin |= digitalRead(rswitch_clk_pin);
  encoder_pin |= digitalRead(rswitch_dat_pin)<<1;
  encoder_pin |= digitalRead(rswitch_sw_pin)<<2;
  //*****

  // Detect rising edge of CLK
  if(((old_encoder_pin & 0x01) == 0) && ((encoder_pin & 0x01) > 0)){
    if((encoder_pin & 0x02) > 0){
      // Get data level for CCW
      rotation_direction = 2;
    }else 
    // Get data level for CW
      rotation_direction = 1;
/*
    Serial.print("Rot: ");
    Serial.print(rotation_direction, DEC);
    Serial.print("  ");
    */
  }

      // Encoder rotation pin CLK and DAT must be to high level before get SW state
    if(((encoder_pin & 0x03) ==  0x03) && ((old_encoder_pin & 0x03) ==  0x03)) {
      // Detect falling edge of SW push
      if(((old_encoder_pin & 0x04) > 0) && ((encoder_pin & 0x04) == 0)){
        sw_event = 1;
        /*
        Serial.print("Sw: ");
        Serial.print(sw_event, DEC);
        Serial.print("   ");
        */
      }
      // Detect rising edge of SW push
      if(((old_encoder_pin & 0x04) == 0) && ((encoder_pin & 0x04) > 0)){
        sw_event = 2;
        /*
        Serial.print("Sw: ");
        Serial.print(sw_event, DEC);
        Serial.print("   ");
        */
      }
    }

  old_encoder_pin = encoder_pin;

  rotary_sw_event |= rotation_direction;
  rotary_sw_event |= sw_event <<2;

  if(rotary_sw_event != old_rotary_sw_event){
    rotary_sw_event |= 0x10;   
  }

  if(rotary_sw_event & 0x10){
    switch(rotary_sw_event & 0x03){
      case 0x01 : encoderAction = ROTARY_DEC; break;
      case 0x02 : encoderAction = ROTARY_INC; break;
      default : break;
    }

      switch(rotary_sw_event & 0x0C){
      case 0x04 : encoderAction = ROTARY_SW_PUSH; break;
      case 0x08 : encoderAction = ROTARY_SW_RELEASE; break;
      default : break;
    }
  }else encoderAction = ROTARY_NO_EVENT;

    old_rotary_sw_event = rotary_sw_event;
  return encoderAction;
}