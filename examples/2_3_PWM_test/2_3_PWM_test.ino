/* 
  Beschreibung: Testprogramm zur Umsetzung der Technischen Richtlinie FA205
                Beispiel für Pulsweitenmodulation (PWM)
                Am PWM-Ausgang (PB.3) wird eine PWM erzeugt. Der Tastgrad wird mit dem
                Analogwert an CH1 eingestellt.

  Autor:            Rolf Rahm
  Datum:            18.02.2015
  Letzte Änderung:  07.05.2023
  Anmerkungen:  Erstellt mit Arduino-Carrier-Board (www.ase-schlierbach.de)
*/
#include "controller.h"

#define Taster_S4  _PORTB_,5

// Funktionen
void setup (void)   /* Initialisierungen */
{
  lcd_init();
  adc_init();
  pwm_init();
  bit_init(Taster_S4,IN);           // Taster !
  
  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("PWM Test");
}

void loop(void)
{
  uint16_t temp;

  if (bit_read(Taster_S4) != 1)
  {
    pwm_start();
    lcd_setcursor(2,1);
    lcd_print(" on ");
  }
  else
  {
    pwm_stop();
    lcd_setcursor(2,1);
    lcd_print(" off");
  }
  temp = adc_in1();
  pwm_duty_cycle((uint8_t)temp);      // PWM Tastgrad ändern
  lcd_setcursor(2,5);
  temp = temp*100/255;
  lcd_byte((uint8_t)temp);
  lcd_char('%');
  delay_ms(500);
}
