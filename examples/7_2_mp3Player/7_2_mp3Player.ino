/*
 Programm:			MP3-Player
 Beschreibung:		MP3-Player Abspielsteuerung

 Autor:				Rahm
 Datum:			    24.04.2017
 letzte �nderung:	08.05.2023
 Anmerkungen:		getestet mit Arduino-Carrier-Board (www.ase-schlierbach.de)
					und MP3-Player-Shield (www.ase-schlierbach.de)
					Siehe 7_2_MP3_Player_Steuerung.pdf
*/

#include "controller.h"
#include "mp3player.h"

// Abspielstatus Busy = 1 Song wird abgespielt
#define PLAY_STATE_BUSY _PORTD_,2

// Taster f�r Abspielsteuerung
#define Playback    _PORTB_,2
#define Rwd         _PORTB_,3
#define Play        _PORTB_,4
#define Fwd         _PORTB_,5

uint16_t current_track_number = 1;
uint8_t  current_folder = 1;
uint16_t total_tracks_number = 0;

volatile uint16_t milli = 0;
volatile uint8_t sek = 0;
volatile uint8_t min = 0;


void setup (void)   // Initialisierungen
{
  rs232_init(); 
  adc_init();
  lcd_init();
  lcd_clear();  
  mp3_init();
  
  bit_init(PLAY_STATE_BUSY,IN);
  bit_init(Playback,IN);
  bit_init(Rwd,IN);
  bit_init(Play,IN);
  bit_init(Fwd,IN);
  
  lcd_setcursor(1,1);
  lcd_print("MP3-Player");
  lcd_setcursor(2,13);
  lcd_char(_C_STOP_);lcd_char(_C_RWD_);lcd_char(_C_PLAY_);lcd_char(_C_FWD_);  
  
  total_tracks_number = mp3_getTotalTrackCount();
  lcd_setcursor(2,1);
  lcd_print("Total:"); lcd_int(total_tracks_number);
  delay_ms(1000);
  mp3_setPlaybackSource(1);
  timer1ms_init(timer1ms_isr);  // F�r genaue Track-Time Angabe
  mp3_setVolume(2);             // Silence !!
  //mp3_playRandomTrackFromAll();   // Alle Tracks zuf�llig spielen
  //mp3_loopGlobalTrack(24);        // Track 24 endlos spielen
  //mp3_setRepeatPlay(1);           // Aktuellen Track endlos spielen
  mp3_stop();                     // Wiedergabe anhalten

  playback_mode = _PLAYBACK_REPEAT_;
  lcd_setcursor(1,13); lcd_char(_C_REPEAT_);
  lcd_setcursor(2,13); lcd_char(_C_STOP_);
  play_mode = _PLAYBACK_STOP_;
  mp3_setEq(_EQ_ROCK_);           // Klang = Rock
}


uint16_t volume;

void loop (void)
{
    volume = adc_in1();
    volume *= 10;
    volume /= 85;
    //volume = mp3_getVolume();   // Einstellung �ber Taster auf Shield
    mp3_setVolume(volume);

    lcd_setcursor(1,14);
    lcd_char(_C_LS_); lcd_dd(volume);
  
    if (bit_read(Play) == 0)
    {
      switch (play_mode)
      {
        case _PLAY_STOP_:
                play_mode = _PLAY_PLAY_;
                milli = 0; sek = 0; min = 0;
                timer1ms_enable();
                lcd_setcursor(2,15); lcd_char(_C_PAUSE_);
                switch (playback_mode)
                {
                  case _PLAYBACK_RANDOM_: 
                  case _PLAYBACK_REPEAT_: 
                            mp3_setPlaybackMode(playback_mode); break;
                  default: mp3_start(); break;
                }
                
                break;
        case _PLAY_PLAY_:
                play_mode = _PLAY_PAUSE_;
                timer1ms_disable();
                lcd_setcursor(2,15); lcd_char(_C_PLAY_); 
                mp3_pause();
                break;
        case _PLAY_PAUSE_:
                play_mode = _PLAY_PLAY_;
                timer1ms_enable();
                mp3_start();
                lcd_setcursor(2,15); lcd_char(_C_PAUSE_);
                break;
      }
    }
    
    if (bit_read(Fwd) == 0)
    {                
      mp3_nextTrack();
      timer1ms_enable();
      milli = 0; sek = 0; min = 0;
      play_mode = _PLAY_PLAY_;
      lcd_setcursor(2,15); lcd_char(_C_PAUSE_);
    }
    
    if(bit_read(Rwd) == 0)
    {
      mp3_prevTrack();
      timer1ms_enable();
      milli = 0; sek = 0; min = 0;
      play_mode = _PLAY_PLAY_;
      lcd_setcursor(2,15); lcd_char(_C_PAUSE_);
    }
    
    if(bit_read(Playback) == 0)
    {
      mp3_stop();
      timer1ms_disable();
      milli = 0; sek = 0; min = 0;
      
      if(play_mode == _PLAY_STOP_)
      {
        switch (playback_mode)
        {
			case _PLAYBACK_REPEAT_:
				playback_mode = _PLAYBACK_RANDOM_;
				lcd_setcursor(1,13); lcd_char(_C_RANDOM_);
				//mp3_setPlaybackMode(playback_mode);
				break;
			case _PLAYBACK_RANDOM_:
				playback_mode = _PLAYBACK_REPEAT_;
				lcd_setcursor(1,13); lcd_char(_C_REPEAT_);
				//mp3_setPlaybackMode(playback_mode);
				break;
        }
      }
      play_mode = _PLAYBACK_STOP_;
      lcd_setcursor(2,15); lcd_char(_C_PLAY_); 
    }
    
    if(play_mode == _PLAY_PLAY_)
    {
      current_track_number = mp3_getCurrentTrack();
      lcd_setcursor(2,7); lcd_int(current_track_number);
    }
    
	//Events werden �ber den seriellen-Interrupt vom MP3-Player-Modul getriggert
    switch (OnEvent)
    {
		case _TRACK_FINISHED_:
      
			if (playback_mode == _PLAYBACK_RANDOM_)
			{
				milli = 0; sek = 0; min = 0;
				mp3_nextTrack();
			}
			else
			{
				play_mode = _PLAY_STOP_;                   
				timer1ms_disable();
				lcd_setcursor(2,15); lcd_char(_C_PLAY_);
			}
			current_track_number = mp3_getCurrentTrack();
			lcd_setcursor(2,7); lcd_int(current_track_number);
			break;
			
		case _CARD_REMOVED_:
			play_mode = _PLAY_STOP_;
			lcd_setcursor(2,1); lcd_print("SD removed  ");
			lcd_setcursor(2,15); lcd_char(_C_PLAY_); break;
			
		case _CARD_INSERTED_:
			play_mode = _PLAY_STOP_;
			lcd_setcursor(2,1); lcd_print("SD inserted ");                    
			lcd_setcursor(2,15); lcd_char(_C_PLAY_); break;      
			
		case _INITIALIZED_:
			play_mode = _PLAY_STOP_;
			lcd_setcursor(2,1); lcd_print("initialized ");
			lcd_setcursor(2,15); lcd_char(_C_PLAY_); break;
			
		case _ERROR_:
			play_mode = _PLAY_STOP_;
			lcd_setcursor(2,1); lcd_print("error       ");
			lcd_setcursor(2,15); lcd_char(_C_PLAY_); break;     
		
		case _NONE_:
		default:      break;
    }
    OnEvent = _NONE_;
    
    lcd_setcursor(2,1);
    lcd_dd(min);lcd_char(':');lcd_dd(sek);lcd_char(' ');
}

void timer1ms_isr(void)
{
  milli++;
  if (milli > 999)
  {
    sek++; milli = 0;
    if (sek > 59)
    {
      min++; sek = 0;
      if (min > 59)
      min = 0;
    }
  }
}

