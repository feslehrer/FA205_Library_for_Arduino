/*
  Beschreibung:     Testprogramm zur Umsetzung der Technischen Richtlinie FA205
                    Beispiel für externen Interrupt (Interrupt 0 )
								
  Autor:            Rolf Rahm
  Datum:            18.02.2015
  Letzte Änderung:  08.05.2023
  Anmerkungen:      getestet mit Arduino-Carrier-Board (www.ase-schlierbach.de)
*/

#include "controller.h"

#define LED _PORTD_,0

// Globale Variablen
uint8_t i = 0;

// Funktionen
void setup (void)              /* Initialisierungen */
{
  lcd_init();
  ext_interrupt_init( ext_interrupt_isr );     // Interrupt initialisieren
  bit_init(LED,OUT);

  lcd_clear();
  lcd_setcursor(1,1);
  lcd_print("Int Test");
  
  ext_interrupt_enable();                      // Interrupt freigeben
}

// Interrupt-Serviceroutine
// Die Funktion ext_interrupt_isr() wird bei fallender Flanke an Int0 aufgerufen!
// Funktionskopf (Name) sollte nicht geändert werden!!!
void ext_interrupt_isr(void)
{
  delay_ms(40);                              // Taster-Entprellung
  i++;
}

// Funktion loop()
void loop (void)
{
  lcd_setcursor(2,4);                        // Anzahl der Interrupts anzeigen
  lcd_int(i);
  bit_write(LED,~bit_read(LED));
  lcd_setcursor(2,1);                        // Aktivitätsanzeige
  lcd_char('*');
  delay_ms(500);
  lcd_setcursor(2,1);
  lcd_char(' ');
  delay_ms(500);
}
