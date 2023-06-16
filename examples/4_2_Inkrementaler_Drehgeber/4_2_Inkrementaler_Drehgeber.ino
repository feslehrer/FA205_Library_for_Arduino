/*
 Beschreibung:      Übung für externen Interrupt
   
 Erstellt am:       13.12.2016
 Letzte Änderung:   08.05.2023
 
 Autor:             Rahm
 Anmerkungen:		getestet mit Arduino-Carrier-Board und
                    Drehgeber-Shield (www.ase-schlierbach.de)
*/

#include "controller.h"

#define Sensor_A _PORTD_,2  // Wird eigentlicj nicht benötigt (Int0)!
#define Sensor_B _PORTD_,3
#define Reset_S1 _PORTB_,2

#define m       20 // Impulse/cm
#define MAXCOUNTS 2000

volatile uint16_t counts = 0;

void setup (void)
{  /* Initialisierungen */
  bit_init(Sensor_B,IN);
  bit_init(Reset_S1,IN);
  
  lcd_init();
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("counts =");
  lcd_setcursor(2,1);
  lcd_print("s =");
  ext_interrupt_init(ext_interrupt_isr);
  ext_interrupt_enable();
}

volatile uint16_t temp, wert;

// Funktion loop()
void loop(void)
{
    lcd_setcursor(1,9);
    lcd_int(counts);
    lcd_setcursor(2,4);
    wert = counts * 100 / m;
    temp = wert / 100;
    lcd_int(temp);
    lcd_char(',');

    wert = wert % 100;
    temp = wert / 10;    
    lcd_char(temp + '0');
    temp = wert % 10;
    lcd_char(temp + '0');
    
    lcd_print(" cm");

    if (!bit_read(Reset_S1)) counts = 0;
}

void ext_interrupt_isr(void)
{
  if (bit_read(Sensor_B))       // Wenn Sensor_A noch 0 ist...
  {
    if (counts != 0)  counts--;
	}
  else
  {
      if (counts < MAXCOUNTS ) counts++;
  }
}
