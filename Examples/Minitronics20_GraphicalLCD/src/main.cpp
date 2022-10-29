#include <Arduino.h>

#include <SPI.h>
#include <SD.h>


    #define U8G_COM_HW_SPI u8g_com_samd21_st7920_hw_spi_fn
    #define U8G_COM_ST7920_HW_SPI u8g_com_samd21_st7920_hw_spi_fn

#include <U8glib-HAL.h>

   #include <LCD_defines.h>
#define SERIALTYPE SerialUSB
#define SDSC 2
#define LCD_PINS_RS            18 // CS chip select /SS chip slave select
#define LCD_PIXEL_WIDTH 128
#define LCD_PIXEL_HEIGHT 64



void u8g_SetPIOutput(u8g_t *u8g, uint8_t pin_index) {
  pinMode(u8g->pin_list[pin_index],OUTPUT);
}

void u8g_SetPILevel(u8g_t *u8g, uint8_t pin_index, uint8_t level) {
  digitalWrite(u8g->pin_list[pin_index],level);
}




typedef int8_t pin_t;


#define U8G_CLASS U8GLIB_ST7920_128X64_4X
  //#define U8G_PARAM 2
//#define U8G_PARAM SCK, MOSI,  LCD_PINS_RS
 #define U8G_PARAM LCD_PINS_RS

    U8G_CLASS u8g;

void setup() {
  // put your setup code here, to run once:

  pinMode(MISO, INPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SS, OUTPUT);
  pinMode(LCD_PINS_RS, OUTPUT);
  digitalWrite(LCD_PINS_RS,LOW);
  SPI.begin(); 
  SERIALTYPE.begin(115200);
  u8g = U8G_CLASS(U8G_PARAM);
 //SPI.begin();
}

void u8g_prepare(void) {
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}


Sd2Card card;
SdVolume volume;
SdFile root;

uint8_t draw_state = 0;
bool SDOK = false;

void draw(void) {
  u8g_prepare();

  char s[2] = " ";
  uint8_t x, y;


  if (SDOK) {
    u8g.drawStr( 0, 0, "SD OK!");
  }
  else
    u8g.drawStr( 0, 0, "SD NOT READY!");

  /*u8g.drawStr( 0, 10, "Back:");
  if (digitalRead(SW_BACK) == 1)
    u8g.drawStr( 100, 10, "Down");
  else
    u8g.drawStr( 100, 10, "Up");

  u8g.drawStr( 0, 20, "Enc sw:");
  if (digitalRead(SW_ENC) == 1)
    u8g.drawStr( 100, 20, "Down");
  else
    u8g.drawStr( 100, 20, "Up");

  u8g.drawStr( 0, 30, "Enc A:");
  if (digitalRead(ENCA) == 1)
    u8g.drawStr( 100, 30, "Down");
  else
    u8g.drawStr( 100, 30, "Up");

  u8g.drawStr( 0, 40, "Enc B:");
  if (digitalRead(ENCB) == 1)
    u8g.drawStr( 100, 40, "Down");
  else
    u8g.drawStr( 100, 40, "Up");*/

}

unsigned long nextMillis = 0;

void loop() {


  if (nextMillis < millis()) {

    nextMillis = millis() + 5000;
    if (!card.init(SPI_FULL_SPEED, SDSC)) {
      SERIALTYPE.println("initialization failed. Things to check:");
      SERIALTYPE.println("* is a card is inserted?");
      SERIALTYPE.println("* Is your wiring correct?");
      SERIALTYPE.println("* did you change the chipSelect pin to match your shield or module?");
      SDOK = false;
    } else {
      SDOK = true;
      SERIALTYPE.println("Wiring is correct and a card is present.");

      if (!volume.init(card)) {
        SERIALTYPE.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
        return;
      }

      uint32_t volumesize;
      SERIALTYPE.print("\nVolume type is FAT");
      SERIALTYPE.println(volume.fatType(), DEC);
      SERIALTYPE.println();

      volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
      volumesize *= volume.clusterCount();       // we'll have a lot of clusters
      volumesize *= 512;                            // SD card blocks are always 512 bytes
      SERIALTYPE.print("Volume size (bytes): ");
      SERIALTYPE.println(volumesize);
    }

  }
 // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(1000);
  SerialUSB.println("assa");
}