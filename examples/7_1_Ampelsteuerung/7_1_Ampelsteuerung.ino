/*
 Beschreibung:			Ampelsteuerung. Ampelphasen per timer-Interrupt 
						Fussgängeranforderung per Polling
   
 Erstellt am:       
 Letzte Änderung:		08.05.2023
 
 Autor: 				Rahm
 Anmerkungen:			getestet mit Arduino-Carrier-Board (www.ase-schlierbach.de)
						und Ampel-Shield (www.ase-schlierbach.de)
*/

#include "controller.h"

#define Ampel   _PORTC_

#define Fuss_GRUEN   _PORTB_,2
#define Fuss_ROT     _PORTB_,3
#define Fuss_Anf     _PORTD_,2    // Auch als Interrupt (Int0) nutzbar 
#define On      1
#define Off     0

uint8_t Ampelwerte[] = {0x09, 0x0b, 0x0c, 0x0a, 0x09, 0x19, 0x21, 0x11};	//Ampelwerte
uint16_t zeit[]      = {2000,  400, 3000,  400, 1000,  400, 5000,  400};	//Zeitwerte
     
volatile uint8_t phase=0;
volatile uint16_t count;
volatile uint8_t status = 0;

void setup(void)
{
  byte_init(Ampel,OUT);
  bit_init(Fuss_GRUEN,OUT);
  bit_init(Fuss_ROT,OUT);
  bit_write(Fuss_GRUEN,Off);
  bit_write(Fuss_ROT,On);           // Fussgängerampel rot
  bit_init(Fuss_Anf,IN);			// Fussgängerampel Anforderungstaster
     
  timer1ms_init( timer1ms_isr );    // Timer initialisieren
  timer1ms_enable();                // Timer freigeben
}

void loop (void)                    // endlos
{
  bit_toggle(Fuss_Anf,&status);	// Fussgänger Anforderung (Polling)
}

//************************************************************************
void timer1ms_isr(void)
{   
  count++;            // Diese Variable wird jedesmal um 1 erhöht ->jede msec
  byte_write(Ampel,Ampelwerte[phase]);

  if(count>=zeit[phase])  // Dieser Programmteil wird ausgeführt: 1msec x arraywert von zeit
  {
    count=0; phase++;
    if( phase==8 )
    {
      if( status )            //Wenn Interrupt aufgetreten und Zyklus durchgelaufen!
      {
        byte_write(Ampel,0x09);        //Ampel auf rot
        //bit_write(Fuss_GRUEN,Off);
        //bit_write(Fuss_ROT,On);         // Fussgängerampel rot
        delay_ms(1000);
        bit_write(Fuss_GRUEN,On);
        bit_write(Fuss_ROT,Off);         // Fussgängerampel grün
        delay_ms(5000);
        bit_write(Fuss_GRUEN,Off);
        bit_write(Fuss_ROT,On);         // Fussgängerampel rot
        delay_ms(2000);
        status = 0;
      }
      phase = 0;      
    }
  }
}
