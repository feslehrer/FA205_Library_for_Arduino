/* 
  Beschreibung:     Testprogramm zur Umsetzung der Technischen Richtlinie FA205
                    Beispiel für LCD
								
  Autor:            Rolf Rahm
  Datum:            17.02.2015
  Letzte Änderung:  07.05.2023
  Anmerkung:		Getestet mit Arduino-Carrier-Board (www.ase-schlierbach.de)
*/
#include "controller.h"


void setup (void)
{
  /* Initialisierungen */
  lcd_init();
  
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("LCD Test");
}

uint8_t a = 0;
  
void loop(void)
{
  lcd_setcursor(2,1);
  lcd_byte(a);
  lcd_print(" -> ");   
  lcd_char(a);
  delay_ms(1000);
  a++;
}

