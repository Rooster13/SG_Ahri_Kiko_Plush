#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"
 
 
SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
FatReader f;      // This holds the information for the file we're play
 
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

int soundNum =0;
 
#define DEBOUNCE 100  // button debouncer
 
// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 
 
void sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}
 
void setup() {
  // set up serial port
  Serial.begin(9600);
  putstring_nl("WaveHC with 6 buttons");
  
   putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(freeRam());      // if this is under 150 bytes it may spell trouble!
  
  // Set the output pins for the DAC control. This pins are defined in the library
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
 
  // pin13 LED
  pinMode(13, OUTPUT);
 
  // enable pull-up resistors on switch pins (analog inputs)
  digitalWrite(14, HIGH);
  digitalWrite(15, HIGH);
  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
 
  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
 
// Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    putstring_nl("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }
  
  // Whew! We got past the tough parts.
  putstring_nl("Ready!");
}
 
void loop() {
  //putstring(".");            // uncomment this to see if the loop isnt running
  // insert if function here for playing sound upon button press
    soundNum = random(1, 26);
    switch (soundNum) {
      case 1:   playComplete("1.wav"); break;
      case 2:   playComplete("2.wav"); break;
      case 3:   playComplete("3.wav"); break;
      case 4:   playComplete("4.wav"); break;
      case 5:   playComplete("5.wav"); break;
      case 6:   playComplete("6.wav"); break;
      case 7:   playComplete("7.wav"); break;
      case 8:   playComplete("8.wav"); break;
      case 9:   playComplete("9.wav"); break;
      case 10:  playComplete("10.wav"); break;
      case 11:  playComplete("11.wav"); break;
      case 12:  playComplete("12.wav"); break;
      case 13:  playComplete("13.wav"); break;
      case 14:  playComplete("14.wav"); break;
      case 15:  playComplete("15.wav"); break;
      case 16:  playComplete("16.wav"); break;
      case 17:  playComplete("17.wav"); break;
      case 18:  playComplete("18.wav"); break;
      case 19:  playComplete("19.wav"); break;
      case 20:  playComplete("20.wav"); break;
      case 21:  playComplete("21.wav"); break;
      case 22:  playComplete("22.wav"); break;
      case 23:  playComplete("23.wav"); break;
      case 24:  playComplete("24.wav"); break;
      case 25:  playComplete("25.wav"); break;
      case 26:  playComplete("26.wav"); break;
    }
  // ending brace for if statement goes here 
} 
 
// Plays a full file from beginning to end with no pause.
void playComplete(char *name) {
  // call our helper to find and play this name
  playfile(name);
  while (wave.isplaying) {
  // do nothing while its playing
  }
  // now its done playing
}
 
void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); Serial.print(name); return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();
}
