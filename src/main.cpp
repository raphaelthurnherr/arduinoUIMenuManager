#include <Arduino.h>
#include <U8x8lib.h>
#include "rotary.h"

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

char i=0, j=0;
unsigned int timer100ms = 0;

unsigned char tsetpoint=10;

void on_release_action(unsigned char level, unsigned char menu, unsigned char sub_menu);
void on_rotary_cw_action(unsigned char level, unsigned char smenu, unsigned char ssub_menu);
void on_rotary_ccw_action(unsigned char level, unsigned char smenu, unsigned char ssub_menu);

unsigned char refreshMenuDisplay(unsigned char level, unsigned char menuNb, unsigned char sub_menuNb);

void display_menu_0();
void display_menu_1();
void display_menu_1_sub_menu_0();
void display_menu_1_sub_menu_1();
void display_menu_2();
void display_menu_2_sub_menu_0();

signed char switchEvent = -1;
unsigned char menu = 0, old_menu = 0;
unsigned char menu_level=0, old_menu_level=0;
unsigned char sub_menu = 0, old_sub_menu = 0;


void setup() {

  Serial.begin(9600);

  u8x8.begin();
  u8x8.setFont(u8x8_font_7x14B_1x2_f);
  u8x8.drawString(0, 0, "-ROTARY SWITCH-");
  u8x8.drawString(0, 2, "RTH 24.05.2020");

  delay(2000);
  u8x8.setFont(u8x8_font_7x14B_1x2_f);
  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  refreshMenuDisplay(0, 0, 0);
  delay(50);
}

void loop() {
              //menu(0, displayFunc(), action());
  //myMenu = new menuManager(RotA, RotB, RotC);

  //myMenu.addMenu(menuNumber, DisplayFunc(), ActionOnPush(), ActionOnCW(), ActionOnCCW());
  //myMenu.addMenu(0, dispA());

  //myMenu.addMenu(1, dispB());
  //myMenu.addMenu(10, dispBa());
  //myMenu.addMenu(11, dispBb());

  //myMenu.addMenu(2, dispC());
  //myMenu.addOnCwAction(20, myAction());
  //myMenu.addOnCcwAction(20, myAction());
  //myMenu.addOnClickAction(20, myAction());

  switchEvent = getRotarySwitchEvent(2,0,16);

  // Event detection on rotary switch
  if(switchEvent != ROTARY_NO_EVENT){
   
    switch(switchEvent){
      case ROTARY_INC : on_rotary_cw_action(menu_level, menu, sub_menu); break;
      case ROTARY_DEC : on_rotary_ccw_action(menu_level, menu, sub_menu); break;
      case ROTARY_SW_PUSH :  break;
      case ROTARY_SW_RELEASE : on_release_action(menu_level, menu, sub_menu); break;
      default : break;
    }
    if(menu > 2) menu = 2;
    if(sub_menu > 1) sub_menu = 1;

  }

    // Refresh display only when menu change
    if((old_menu_level != menu_level) || (old_menu != menu) || (old_sub_menu != sub_menu)){
      refreshMenuDisplay(menu_level, menu, sub_menu);
      old_menu_level = menu_level;      
      old_menu = menu;
      old_sub_menu = sub_menu;
    }


  if(timer100ms>=10){

    timer100ms=0;
  }
  
  timer100ms++;
  delay(10);
}


//*******************************************
void on_rotary_cw_action(unsigned char level, unsigned char smenu, unsigned char ssub_menu){
  if(level == 0){
    switch(smenu){
      default :  menu++; break;
    }
  }

  if(level == 1){
    ssub_menu = (smenu*10)+ssub_menu;
    switch(ssub_menu){
      case 20 : tsetpoint++; display_menu_2_sub_menu_0(); break;
      default :  sub_menu++; break;
    }
  }
}
//*******************************************
void on_rotary_ccw_action(unsigned char level, unsigned char smenu, unsigned char ssub_menu){
  if(level == 0){
    switch(smenu){
      default :  menu--; break;
    }
  }

  if(level == 1){
      ssub_menu = (smenu*10)+ssub_menu;
    switch(ssub_menu){
      case 20 : tsetpoint--; display_menu_2_sub_menu_0(); break;
      default :  sub_menu--; break;
    }
  }  
}
//*******************************************
void on_release_action(unsigned char level, unsigned char smenu, unsigned char ssub_menu){
  if(level == 0){
    switch(smenu){
      case 0 : break;
      case 1 : menu_level = 1; sub_menu = 0; break; // Go to sub menu
      case 2 : menu_level = 1; sub_menu = 0; break; // Go to sub menu
      default : break; 
    }
  }

  if(level == 1){
    ssub_menu = (smenu*10)+ssub_menu;
    switch(ssub_menu){
      case 10 : menu_level = 0; menu = 0; sub_menu = 0; break;
      case 11 : menu_level = 0; menu = 1; sub_menu = 0; break;
      case 20 : menu_level = 0; menu = 2; sub_menu = 0; break;
      default : break; 
    }
  }
}

//******************************************

unsigned char refreshMenuDisplay(unsigned char level, unsigned char menuNb, unsigned char sub_menuNb){
  u8x8.clearDisplay();

  if(level == 0){
    switch(menuNb){
      case 0 : display_menu_0(); break;
      case 1 : display_menu_1(); break;
      case 2 : display_menu_2(); break;
      default : u8x8.drawString(0, 0, "ERROR MENU L0");
    }
  }

    if(level == 1){
      sub_menuNb = (menuNb*10)+sub_menuNb;
    switch(sub_menuNb){
      case 10 : display_menu_1_sub_menu_0(); break;
      case 11 : display_menu_1_sub_menu_1(); break;
      case 20 : display_menu_2_sub_menu_0(); break;
      default : u8x8.drawString(0, 0, "ERROR MENU L1");
    }
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

        void display_menu_1_sub_menu_0(){

          u8x8.drawString(0, 0, "THIS IS LEVEL 1");
          u8x8.drawString(0, 2, "SUB MENU 0");
          u8x8.drawString(0, 4, "       |");
          u8x8.drawString(0, 6, "HOME <-|");
        }

        void display_menu_1_sub_menu_1(){

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
        void display_menu_2_sub_menu_0(){

          u8x8.drawString(0, 0, "     MENU 2     ");
          u8x8.drawString(0, 2, " TEMP. SETPOINT ");

          u8x8.setInverseFont(1);
          u8x8.setCursor(6, 4);
          u8x8.print(tsetpoint, DEC);
          u8x8.print(" C");
          u8x8.setInverseFont(0);

          u8x8.drawString(0, 6, "            -OK-");
        }
