#include <Arduino.h>
#include <U8x8lib.h>
#include "rotary.h"

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

unsigned int timer100ms = 0;

unsigned char tsetpoint=10;

void on_release_action(unsigned char menuNb);
void on_rotary_cw_action(unsigned char menuNb);
void on_rotary_ccw_action(unsigned char menuNb);

unsigned char refreshMenuDisplay(unsigned char menuNb);

void display_menu_0();
void display_menu_1();
void display_menu_1_0();
void display_menu_1_1();
void display_menu_2();
void display_menu_2_0();

signed char switchEvent = -1;
unsigned char menu = 0, old_menu = 0;

void setup() {

  pinMode(2, INPUT);
  pinMode(0, INPUT);
  pinMode(16, INPUT);

  Serial.begin(9600);

  u8x8.begin();
  u8x8.setFont(u8x8_font_7x14B_1x2_f);
  u8x8.drawString(0, 0, "-ROTARY SWITCH-");
  u8x8.drawString(0, 2, "RTH 26.05.2020");

  delay(2000);
  u8x8.setFont(u8x8_font_7x14B_1x2_f);
  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  refreshMenuDisplay(0);
  delay(50);
}

void loop() {

  switchEvent = getRotarySwitchEvent(2,0,16);

  // Event detection on rotary switch
  if(switchEvent != ROTARY_NO_EVENT){
    switch(switchEvent){
      case ROTARY_INC : on_rotary_cw_action(menu); break;
      case ROTARY_DEC : on_rotary_ccw_action(menu); break;
      case ROTARY_SW_PUSH :  break;
      case ROTARY_SW_RELEASE : on_release_action(menu); break;
      default : break;
    }
  }

    // Refresh display only when menu change
    if((old_menu != menu)){
      Serial.print("---> MENU CHANGE\n");
      refreshMenuDisplay(menu);    
      old_menu = menu;
    }

  if(timer100ms>=10){
    timer100ms=0;
  }
  
  timer100ms++;
  delay(10);
}


//*******************************************
void on_rotary_cw_action(unsigned char menuNb){
  switch(menuNb){
    case 0: menu = 1; break;
    case 1: menu = 2; break;
    case 2: menu = 0; break;
    
    case 10: menu = 11; break;
    case 11: menu = 10; break;
    case 20: tsetpoint++; display_menu_2_0(); break;
    default :  break;
  }
}
//*******************************************
void on_rotary_ccw_action(unsigned char menuNb){
  switch(menuNb){
    case 0: menu = 2; break;
    case 1: menu = 1; break;
    case 2: menu = 0; break;
    
    case 10: menu = 11; break;
    case 11: menu = 10; break;
    case 20: tsetpoint--; display_menu_2_0(); break;    
    default :  break;
  } 
}
//*******************************************
void on_release_action(unsigned char menuNb){
  switch(menuNb){
    case 0: break;
    case 1: menu = 10; break;
    case 2: menu = 20; break;

    case 10: menu = 0; break;
    case 11: menu = 1; break;
    case 20: menu = 2; break;
    default :  break;
  } 
}

//******************************************

unsigned char refreshMenuDisplay(unsigned char menuNb){
  u8x8.clearDisplay();
  switch(menuNb){
    case 0 : display_menu_0(); break;
    case 1 : display_menu_1(); break;
    case 2 : display_menu_2(); break;

    case 10 : display_menu_1_0(); break;
    case 11 : display_menu_1_1(); break;
    case 20 : display_menu_2_0(); break;
    default : u8x8.drawString(0, 0, "ERROR MENU L0");
  }
}

//****************************************************

//-------------- MENU AND SUB MENU -------------------

/// MENU PRINCIPAL
void display_menu_0(){

  u8x8.drawString(0, 0, "Set:    C    %");
  u8x8.drawString(0, 2, "12.99C /   %Hr");
  u8x8.drawString(0, 4, "COOL:    /   C");
  u8x8.drawString(0, 6, "OUT:     /   C");
}


/// MENU 1
void display_menu_1(){

  u8x8.drawString(0, 0, "MENU 1");
  u8x8.drawString(0, 2, "LEVEL 0");
  u8x8.drawString(0, 4, " |");
  u8x8.drawString(0, 6, " |-> SUB");
}

        void display_menu_1_0(){

          u8x8.drawString(0, 0, "THIS IS LEVEL 1");
          u8x8.drawString(0, 2, "SUB MENU 0");
          u8x8.drawString(0, 4, "       |");
          u8x8.drawString(0, 6, "HOME <-|");
        }

        void display_menu_1_1(){

          u8x8.drawString(0, 0, "THIS IS LEVEL 1");
          u8x8.drawString(0, 2, "SUB MENU 1");
          u8x8.drawString(0, 4, "MOTOR SETTINGS");
          u8x8.drawString(0, 6, "BACK <-|");
        }

/// MENU BACK
void display_menu_2(){

  u8x8.drawString(0, 0, "     MENU 2     ");
  u8x8.drawString(0, 2, " TEMP. SETPOINT ");
  u8x8.setCursor(6, 4);
  u8x8.print(tsetpoint, DEC);
  u8x8.print(" C");
  u8x8.drawString(0, 6, "        -CHANGE-");
}
        void display_menu_2_0(){

          u8x8.drawString(0, 0, "     MENU 2     ");
          u8x8.drawString(0, 2, " TEMP. SETPOINT ");

          u8x8.setInverseFont(1);
          u8x8.setCursor(6, 4);
          u8x8.print(tsetpoint, DEC);
          u8x8.print(" C ");
          u8x8.setInverseFont(0);

          u8x8.drawString(0, 6, "            -OK-");
        }
