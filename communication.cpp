// Technische Richtlinie FA205
// Bibliothek:       communication.c
// Controller:       ATMEL ATMega8 / ATmega328
//     I2C und RS232 Routinen
// Version:          1.0
// erstellt am:      5.5.2015
// letzte �nderung:  17.3.2018
// Autor:            Rahm

#include "communication.h"
//#include "interrupt.h"

// *****************************************************************************************
// I2C- Routinen zur Ansteuerung eins I2C-Slaves
// I2C-Bus-Funktionen i2c_init, i2c_start, i2c_stop, i2c_write, i2c_read, i2c_ack, i2c_nack
// *****************************************************************************************

// ****** Initialiserung I2C-Bus **********************************************
void i2c_init (void)
{
	#ifdef _ATMEGA328PB_
	  TWSR0 &= ~((1<<TWPS0)|(1<<TWPS1));	// Prescaler = 1
	  TWBR0 = ((F_CPU / _I2C_FREQUENZ_) - 16) / 2;
	#else
	  TWSR &= ~((1<<TWPS0)|(1<<TWPS1));	// Prescaler = 1
	  TWBR = ((F_CPU / _I2C_FREQUENZ_) - 16) / 2;
	#endif
}

// ****** Startbedingung I2C-Bus **********************************************
void i2c_start (void)
{
	#ifdef _ATMEGA328PB_
		/*writing a one to TWINT clears it, TWSTA=Start, TWEN=TWI-enable*/
		TWCR0 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);		// Control Register
		/*wait, until start condition has been sent --> ACK*/
		while (!(TWCR0 & (1<<TWINT)));
	#else
		/*writing a one to TWINT clears it, TWSTA=Start, TWEN=TWI-enable*/
		TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);		// Control Register
		/*wait, until start condition has been sent --> ACK*/
		while (!(TWCR & (1<<TWINT)));
	#endif
}

//****** Stoppbedingung I2C-Bus ***********************************************************
void i2c_stop (void)
{
	#ifdef _ATMEGA328PB_
    /*writing a one to TWINT clears it, TWSTO=Stop, TWEN=TWI-enable*/
    TWCR0 = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
  #else
	  /*writing a one to TWINT clears it, TWSTO=Stop, TWEN=TWI-enable*/
	  TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
  #endif
}

//*****************************************************************************************
// * Byte ausgeben an I2C-Bus , R�ckgabewert = ack = ACK/NACK
// ****************************************************************************************
uint8_t i2c_write (uint8_t byte)
{
	#ifdef _ATMEGA328PB_
    /*TWDR contains byte to send*/
    TWDR0 = byte;
    /*send content of TWDR*/
    TWCR0 = (1<<TWINT) | (1<<TWEN);
    /*wait, until byte has been sent --> ACK*/
    while (!(TWCR0 & (1<<TWINT)));
    // Abfrage der TWI-Statuscodes (ATmega128 Manual S.214)
    if (((TWSR0 & 0xf8)== 0x18) || ((TWSR0 & 0xf8)== 0x28)) return ACK;
    if (((TWSR0 & 0xf8)== 0x20) || ((TWSR0 & 0xf8)== 0x30)) return NACK;
    return 0;	//Status Register
  #else
	  /*TWDR contains byte to send*/
	  TWDR = byte;
	  /*send content of TWDR*/
	  TWCR = (1<<TWINT) | (1<<TWEN);
	  /*wait, until byte has been sent --> ACK*/
	  while (!(TWCR & (1<<TWINT)));
	  // Abfrage der TWI-Statuscodes (ATmega128 Manual S.214)
	  if (((TWSR & 0xf8)== 0x18) || ((TWSR & 0xf8)== 0x28)) return ACK;
	  if (((TWSR & 0xf8)== 0x20) || ((TWSR & 0xf8)== 0x30)) return NACK;
	  return 0;	//Status Register
  #endif
}

//*****************************************************************************************
// * Byte einlesen vom I2C-Bus.
// ****************************************************************************************
uint8_t i2c_read (uint8_t ack)
{
 	#ifdef _ATMEGA328PB_
  	if (ack == ACK)
  	  /*send content of TWDR; TWEA = enable ACK*/
 	    TWCR0 = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
    else
 	    /*send content of TWDR; NACK*/
 	    TWCR0 = (1<<TWINT) | (1<<TWEN);
 	
    /*wait, until byte has been received --> ACK*/
    while (!(TWCR0 & (1<<TWINT)));
    return TWDR0;
  #else
		if (ack == ACK)
		  /*send content of TWDR; TWEA = enable ACK*/
		  TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		else 
		  /*send content of TWDR; NACK*/
		  TWCR = (1<<TWINT) | (1<<TWEN);
			
		/*wait, until byte has been received --> ACK*/
		while (!(TWCR & (1<<TWINT)));
		return TWDR;
	#endif
}

// ****************************************************************************
// RS232-Routinen zur Kommunikation mit PC-Terminal
//
// ****************************************************************************
void rs232_init(void)
{ 
  rs232_baud(BAUD);
}

void rs232_baud(uint32_t baud)
{
  uint16_t ubrr_value;
  
  // Berechne den UBRR-Wert basierend auf der gewünschten Baudrate
  if (baud >= 57600) 
  {
    // Setze das U2X-Bit für höhere Baudraten
    UCSR0A |= (1 << U2X0);
    ubrr_value = (F_CPU / (8 * baud)) - 1;
  } else 
  {
    // Lösche das U2X-Bit für niedrigere Baudraten
    UCSR0A &= ~(1 << U2X0);
    ubrr_value = (F_CPU / (16 * baud)) - 1;
  }
  
  // Setze die UBRR-Register
  UBRR0H = (ubrr_value >> 8);
  UBRR0L = ubrr_value;

  UCSR0B |= (1<<TXEN0) | (1<<RXEN0);    // UART TX und RX einschalten
  UCSR0C |= (1<<USBS0) | (3<<UCSZ00);   // Asynchron 8N1
}
  

uint8_t rs232_get ( void )
{
  if (!(UCSR0A & (1<<RXC0))) return ('\0');    // Wenn kein Zeichen im Puffer, dann 0 zurückgeben!
  return UDR0;                                // Zeichen zurückliefern
 }

void rs232_put ( uint8_t value )
{
  while (!(UCSR0A & (1<<UDRE0)));              // Warten bis Sendebereit!
  UDR0 = value;
}

// Doppelte Definition notwendig, sonst Fehlermeldung "unerlaubte Typumwandlung"
void rs232_print(const char* text) 
{
  while (*text != '\0')
  rs232_put(*text++);
}

void rs232_print(uint8_t* text) 
{
  const char* temp_str = (const char*)text;
  rs232_print(temp_str);
}

// RS232-Erweiterungen sind nicht Teil der Technischen Richtlinie FA205!!
// Erzeugt ein Eingabeprompt am Terminal 00 .. 99
uint8_t rs232_inputdd(void )
{
  uint8_t buf1,buf2;
  do
  {
    while((buf1 = rs232_get()) == 0);
  } while ((buf1 < '0') || (buf1 > '9'));

  rs232_put(buf1);    // Echo

  do
  {
    while((buf2 = rs232_get()) == 0);
  } while ((buf1 < '0') || (buf1 > '9'));

  rs232_put(buf2);    // Echo

  buf1 -= '0';
  buf2 -= '0';

  return (buf1*10 + buf2);
}

// Ausgabe einer Dezimalzahl 00..99 auf RS232
void rs232_printdd(uint8_t value)
{
  uint8_t buf;

  buf = value / 10;
  rs232_put(buf+'0');      // 10er-Stelle anzeigen
  buf = value % 10;
  rs232_put(buf+'0');      // 1er-Stelle anzeigen
}

void rs232_int(uint16_t val)
{
  uint8_t buffer[5];
  uint8_t n = 0;
  
  do
  {
    buffer[n++] = val%10 + '0';
  } while ((val /= 10) > 0);
  
  while (n<5)                   // Rest von buffer mit blank füllen
  {
    buffer[n++] = ' ';
  }

  while (n > 0)                 // Ausgabe auf das Display (umgekehrt)
  {
    n--;
    rs232_put(buffer[n]);
  }
}

uint8_t rs232_binary_get ( void )
{
   while (!rs232_is_received());    // warten, bis Byte im Puffer!

   return UDR0;                     // wert zurückgeben
}

uint8_t rs232_readbytes(uint8_t *buf, int8_t _length)
{
   uint16_t timeout = 0;
   uint8_t  i = 0;

   //Puffer löschen
   for (uint8_t n=0; n<=_length;n++)  buf[n] = 0;

   while(_length > 0)
   {
     timeout = 0;
     while(!rs232_is_received())
     {
        if(timeout>=0xfffe) return 0;
        else                 timeout++;
     }
     
     *buf++ = rs232_binary_get();
     _length--;
     i++;
   }
   return i; 
}

uint8_t rs232_is_received( void )
{
  if (!(UCSR0A & (1<<RXC0))) return 0;    // 0, wenn kein Byte im Puffer!
  return 1;                               // 1, wenn ein Byte im Puffer
}