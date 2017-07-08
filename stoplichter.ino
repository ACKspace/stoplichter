void setup()
{
  pinMode( 5, OUTPUT );
  pinMode( 6, OUTPUT );
  pinMode( 7, OUTPUT );

  pinMode( 8, OUTPUT );
  pinMode( 9, OUTPUT );
  pinMode( 10, OUTPUT );

  pinMode( 11, OUTPUT );
  pinMode( 12, OUTPUT );
  pinMode( 13, OUTPUT );
}

#define BLINKDELAY 750
#define ORANGEGREEN 160

#define NORMALBLINK 5
#define NORMALLIGHT 3
#define SPECIALBLINK 8

void off()
{
    for ( uint8_t n = 5; n < 13; ++n )
    digitalWrite( 5, LOW );
}

void loop()
{
  // All off
  off();

  #ifdef NORMALBLINK
    for ( uint8_t n = 0; n < NORMALBLINK; ++n )
    {
      analogWrite( 8, 255 );
      analogWrite( 9, ORANGEGREEN );
      delay( BLINKDELAY );
      analogWrite( 8, 0 );
      analogWrite( 9, 0 );
      delay( BLINKDELAY );
    }
  #endif

  // All off
  off();

  #ifdef NORMALLIGHT
    for ( uint8_t n = 0; n < NORMALLIGHT; ++n )
    {
      analogWrite( 5, 255 );
      delay( 4000 );
      analogWrite( 7, 0 );
      analogWrite( 5, 0 );
      analogWrite( 12, 255 );
      delay( 8000 );
      analogWrite( 12, 0 );
      analogWrite( 8, 255 );
      analogWrite( 9, ORANGEGREEN );
  
      delay( 3000 );
      analogWrite( 8, 0 );
      analogWrite( 9, 0 );
  
      analogWrite( 5, 255 );
      delay( 3000 );
      for ( uint8_t p = 0; p < 100; ++p )
      {
        // Pink!
        analogWrite( 7, p );
        delay( 20 );
      }
    }
    analogWrite( 7, 0 );
  #endif

  // All off
  off();

  #ifdef SPECIALBLINK
    uint8_t r = 255;
    uint8_t g = ORANGEGREEN;
    uint8_t b = 0;
  
    for ( uint8_t n = 0; n < SPECIALBLINK; ++n )
    {
      analogWrite( 8, r );
      analogWrite( 9, g );
      analogWrite( 10, b );
      delay( BLINKDELAY );
      analogWrite( 8, 0 );
      analogWrite( 9, 0 );
      analogWrite( 10, 0 );
      delay( BLINKDELAY );
  
      r >>= 1;
      g >>= 1;
      b <<= 1;
      if ( !b )
        ++b;
    }
  #endif
}
