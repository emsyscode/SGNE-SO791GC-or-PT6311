/****************************************************/
/* This is only one example of code structure       */
/* OFFCOURSE this code can be optimized, but        */
/* the idea is let it so simple to be easy catch    */
/* where we can do changes and look to the results  */
/****************************************************/
//set your clock speed
#define F_CPU 16000000UL
//these are the include files. They are outside the project folder
#include <avr/io.h>
//#include <iom1284p.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define VFD_data 7// If 0 write LCD, if 1 read of LCD
#define VFD_clk 8 // if 0 is a command, if 1 is a data0
#define VFD_stb 9 // Must be pulsed to LCD fetch data of bus
/*Global Variables Declarations*/
unsigned char grid;
/*
unsigned int segPT6311[] ={
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//bar in middle(center)
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//bar
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//bar
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//left grid
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//DSP 1
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//DSP 2
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//DSP 3
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//DSP 4
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//DSP 5
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//DSP 6
            (0b00000000), (0b00100000), (0b00000000), // The third byte is only half byte of third byte.//DSP 7  // this have the point active in front of display 7
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//DSP 8
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//dsp small
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.//right grid
            (0b00000000), (0b00000000), (0b00000000), // The third byte is only half byte of third byte.
            (0b00000000), (0b00000000), (0b00000000)  // The third byte is only half byte of third byte.
};
*/
//    Numbers desing        01234567     abcdefgh
unsigned char number0[]={(0b00100011),(0b00011001),(0b00000000)}; // 0
unsigned char number1[]={(0b00000010),(0b00000001),(0b00000000)}; // 1
unsigned char number2[]={(0b11000011),(0b00011000),(0b00000000)}; // 2
unsigned char number3[]={(0b11000011),(0b00010001),(0b00000000)}; // 3
unsigned char number4[]={(0b11100010),(0b00000001),(0b00000000)}; // 4
unsigned char number5[]={(0b11100001),(0b00010001),(0b00000000)}; // 5
unsigned char number6[]={(0b11100001),(0b00011001),(0b00000000)}; // 6
unsigned char number7[]={(0b00000011),(0b00000001),(0b00000000)}; // 7
unsigned char number8[]={(0b11100011),(0b00011001),(0b00000000)}; // 8
unsigned char number9[]={(0b11100011),(0b00000001),(0b00000000)}; // 9
//    Letters desing       01234567     abcdefgh
unsigned char letterA[]={(0b11100011),(0b00001001),(0b00000000)}; // A
unsigned char letterB[]={(0b10110001),(0b00011010),(0b00000000)}; // B
unsigned char letterC[]={(0b00100001),(0b00011000),(0b00000000)}; // C
unsigned char letterD[]={(0b00100100),(0b00001100),(0b00000000)}; // D
unsigned char letterE[]={(0b11100001),(0b00011000),(0b00000000)}; // E
unsigned char letterF[]={(0b11100001),(0b00001000),(0b00000000)}; // F
unsigned char letterG[]={(0b01100001),(0b00011001),(0b00000000)}; // G
unsigned char letterH[]={(0b11100010),(0b00001001),(0b00000000)}; // H
unsigned char letterI[]={(0b00001001),(0b00010000),(0b00000000)}; // I
unsigned char letterJ[]={(0b00000010),(0b00011001),(0b00000000)}; // J
unsigned char letterK[]={(0b10110000),(0b00001010),(0b00000000)}; // k
unsigned char letterL[]={(0b00100000),(0b00011000),(0b00000000)}; // L
unsigned char letterM[]={(0b00110110),(0b00001001),(0b00000000)}; // M
unsigned char letterN[]={(0b00100110),(0b00001011),(0b00000000)}; // N
unsigned char letterO[]={(0b00100011),(0b00011001),(0b00000000)}; // O
unsigned char letterP[]={(0b11100011),(0b00001000),(0b00000000)}; // P
unsigned char letterQ[]={(0b00100011),(0b00011011),(0b00000000)}; // A
unsigned char letterR[]={(0b11100011),(0b00001010),(0b00000000)}; // R
unsigned char letterS[]={(0b11100001),(0b00010001),(0b00000000)}; // S
unsigned char letterT[]={(0b00001001),(0b00000000),(0b00000000)}; // T
unsigned char letterU[]={(0b00100010),(0b00011001),(0b00000000)}; // U
unsigned char letterV[]={(0b00110000),(0b00001100),(0b00000000)}; // V
unsigned char letterX[]={(0b00010100),(0b00000110),(0b00000000)}; // X
unsigned char letterZ[]={(0b00010001),(0b00010100),(0b00000000)}; // Z
unsigned char letterW[]={(0b00100010),(0b00001111),(0b00000000)}; // W
unsigned int segments[] ={ // Draw numbers to the display of 7 segments
  //This not respect the normal table for 7segm like "abcdefgh"  // 
      0b01110111, //0  // 
      0b00010010, //1  // 
      0b01101011, //2  // 
      0b01011011, //3  // 
      0b00011110, //4  // 
      0b01011101, //5  // 
      0b01111101, //6  // 
      0b00010011, //7  // 
      0b01111111, //8  // 
      0b00011111, //9  // 
      0b00000000, //10 // empty display
  };
/*****************************************************************/
void pt6311_init(void)
{
  delayMicroseconds(200); //power_up delay
  // Note: Allways the first byte in the input data after the STB go to LOW is interpret as command!!!
  // Configure VFD display (grids)
  cmd_with_stb(0b00000000);//  (0b01000000)    cmd1 14 Grids and 14 segments??? (IC da SGNEC ref: S0791 que é igual 6311 pinout)
  delayMicroseconds(1);
  cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
  delayMicroseconds(1);
  // turn vfd on, stop key scannig
   cmd_with_stb(0b10001000);//(BIN(01100110)); 
  delayMicroseconds(1);
  // Write to memory display, increment address, normal operation
  cmd_with_stb(0b01000000);//(BIN(01000000));
  delayMicroseconds(1);
  // Address 00H - 15H ( total of 11*2Bytes=176 Bits)
  cmd_with_stb(0b11000000);//(BIN(01100110)); 
  delayMicroseconds(1);
  // set DIMM/PWM to value
  cmd_with_stb((0b10001000) | 7);//0 min - 7 max  )(0b01010000)
  delayMicroseconds(1);
}
/*****************************************************************/
void send_char(unsigned char grid, unsigned char byte0, unsigned char byte1,unsigned char byte2){
 
        cmd_with_stb(0b00001101); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(2);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        delayMicroseconds(2);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Auto increment
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(2);
            cmd_without_stb((0b11000000) | grid); // cmd 3 //wich define the start address (00H to 15H)
            cmd_without_stb(byte0); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(byte1); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_4bitsWithout_stb(byte2); // only half byte of third byte.
            //
            digitalWrite(VFD_stb, HIGH);
            delayMicroseconds(2);
             /*
            Serial.println("******************");  // only to debug on the console print
            Serial.println(((0b11000000) | grid), BIN);
            Serial.println("------------------");
            Serial.println(grid, BIN);//
            Serial.println(byte0, BIN);//
            Serial.println(byte1, BIN);//
            Serial.println(byte2, BIN);
            Serial.println("++++++++++++++++++");
            */
            delay(400);
}
void cmd_4bitsWithout_stb(unsigned char a)
{
  // send without stb
  unsigned char transmit = 3; //define our transmit pin
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  //This don't send the strobe signal, to be used in burst data send
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data, LOW);
     }
    delayMicroseconds(5);
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(5);
   }
   //digitalWrite(VFD_clk, LOW);
}
/*****************************************************************/
void cmd_without_stb(unsigned char a)
{
  // send without stb
  unsigned char transmit = 7; //define our transmit pin
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  //This don't send the strobe signal, to be used in burst data send
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data, LOW);
     }
    delayMicroseconds(5);
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(5);
   }
   //digitalWrite(VFD_clk, LOW);
}
/*****************************************************************/
void cmd_4bitsWith_stb(unsigned char a)
{
  // send with stb
  unsigned char transmit = 3; //define our transmit pin
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  
  //This send the strobe signal
  //Note: The first byte input at in after the STB go LOW is interpreted as a command!!!
  digitalWrite(VFD_stb, LOW);
  delayMicroseconds(1);
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     delayMicroseconds(1);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data, LOW);
     }
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(1);
   }
   digitalWrite(VFD_stb, HIGH);
   delayMicroseconds(1);
}
/*****************************************************************/
void cmd_with_stb(unsigned char a)
{
  // send with stb
  unsigned char transmit = 7; //define our transmit pin
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  
  //This send the strobe signal
  //Note: The first byte input at in after the STB go LOW is interpreted as a command!!!
  digitalWrite(VFD_stb, LOW);
  delayMicroseconds(1);
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     delayMicroseconds(1);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data, LOW);
     }
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(1);
   }
   digitalWrite(VFD_stb, HIGH);
   delayMicroseconds(1);
}
/*****************************************************************/
void test_VFD(void)
{
  clear_VFD();
      
      digitalWrite(VFD_stb, LOW);
      delayMicroseconds(1);
      cmd_with_stb(0b00001000); // cmd 1 // 5 Grids & 16 Segments
      cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
      cmd_with_stb(0b01000000); // cmd 2 //Write VFD, Normal operation; Set pulse as 1/16, Auto increment
      cmd_with_stb(0b10001000 | 0x07); // cmd 2 //set on, dimmer to max
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
        cmd_without_stb((0b11000000)); //cmd 3 wich define the start address (00H to 15H)
        // Only here must change the bit to test, first 2 bytes and 1/2 byte of third.
         for (int i = 0; i < 8 ; i++){ // test base to 16 segm and 5 grids
          // Zone of test, if write 1 on any position of 3 bytes below position, will bright segment corresponding it.
         cmd_without_stb(0b00000000); // Data to fill table 5*16 = 80 bits
         cmd_without_stb(0b00000000); // Data to fill table 5*16 = 80 bits
         cmd_4bitsWithout_stb(0b00000000); // Data to fill table 5*16 = 80 bits
         }
    
      //cmd_without_stb(0b00000001); // cmd1 Here I define the 5 grids and 16 Segments
      //cmd_with_stb((0b10001000) | 7); //cmd 4
      digitalWrite(VFD_stb, HIGH);
      delay(1);
      delay(3000);  
}
void AD16311_RunWeels(){
  int j, n;
  char x;
  short v = 0b0000000000000001;  // The short have a size of 16 bits(2 bytes)
 
        for (n=15; n < 16; n++){  //Note: only want write the position 9 & 10 of memory map (5 grids X 2 bytes)
          //clear_VFD();
            for(j = 0; j < 5; j++) {  // execute 8 times the for cycle
              //cmd1 Configure VFD display (grids) 
              cmd_with_stb(0b00000000);//  5 grids
              cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
              delayMicroseconds(1);  // 
              
              //cmd2 Write to memory display, increment address, normal operation 
              cmd_with_stb(0b01000000);//Teste mode setting to normal, Address increment Fixed, Write data to display memory...
              
              digitalWrite(VFD_stb, LOW);
              delayMicroseconds(1);
              //cmd3 Address 15H Start 3 position of memory allocated to the 6ª grid(total of 2bytes and half).
              cmd_without_stb((0b11000000) | n);//Increment active, then test all segments from positions 15 to 17, grid 6!
              delayMicroseconds(1);
               x=0b00000001;
               cmd_without_stb((x << j) & 0xFF); // Block the 8º bit wich brigth the CD symbol.
              delayMicroseconds(1);
              digitalWrite(VFD_stb, HIGH);
              //cmd4 set DIMM/PWM to value
              cmd_with_stb((0b10001000) | 7);//0 min - 7 max  )(0b01010000)//0 min - 7 max  )(0b01010000)
              delay(30);
            }
        }
}
void SGNE_S0791_RunWeels(){   // this is a IC equivalent to the PT6311
  unsigned char  j, n;
  unsigned char x;
  short v = 0b0000000000000001;  // The short have a size of 16 bits(2 bytes)
            //cmd1 Configure VFD display (grids) 
              cmd_with_stb(0b00001101); //       cmd 1 // 14 Grids & 14 Segments
              delayMicroseconds(1);
              cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
              delayMicroseconds(1); 
              //cmd2 Write to memory display, increment address, normal operation 
              cmd_with_stb(0b01000000);//Teste mode setting to normal, Address increment Fixed, Write data to display memory... 
              // 
              clear_VFD();
              //
           
                    for(j = 0; j < 4; j++) {  // execute 8 times the for cycle
                      // clear byte of wheels
                      digitalWrite(VFD_stb, LOW);
                      delayMicroseconds(1);
                      //cmd3 Address 15H Start 3 position of memory allocated to the 6ª grid(total of 2bytes and half).
                      cmd_without_stb((0b11001001));//Increment active, then test all segments from positions 15 to 17, grid 6!
                      delayMicroseconds(1);
                       cmd_without_stb((0b00000000)); // 
                      delayMicroseconds(1);
                      digitalWrite(VFD_stb, HIGH);
                      
                     delay(20);
                      digitalWrite(VFD_stb, LOW);
                      delayMicroseconds(1);
                      //cmd3 Address 15H Start 3 position of memory allocated to the 6ª grid(total of 2bytes and half).
                      cmd_without_stb((0b11001001));//Increment active, then test all segments from positions 15 to 17, grid 6!
                      delayMicroseconds(1);
                       x=0b10000000;
                       cmd_without_stb((x >> j) & 0xFF); // Block the 8º bit wich brigth the CD symbol.
                      delayMicroseconds(1);
                      digitalWrite(VFD_stb, HIGH);
                      delay(20);
                    }
           
}
/******************************************************************/
void swapLed1(){
  byte myByte;
    digitalWrite(VFD_stb, LOW);
    delayMicroseconds(20);
    cmd_without_stb(0b01000001);
    delayMicroseconds(20);
    myByte ^=(0b00000001);  // Here is only to invert bit of led 1.
    cmd_without_stb(myByte);
    delayMicroseconds(20);
    digitalWrite(VFD_stb, HIGH);
    delayMicroseconds(20);
}
/*****************************************************************/
void setup() {
// put your setup code here, to run once:
// initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
   Serial.begin(9600);
//Set PORT
DDRD = 0xFF;  // IMPORTANT: from pin 0 to 7 is port D, from pin 8 to 13 is port B
PORTD=0x00;
DDRB =0xFF;
PORTB =0x00;
pt6311_init();

}
/*****************************************************************/
void clear_VFD(void)
{
  int sum=0;
  /*
  Here I clean all registers 
  Could be done only on the number of grid
  to be more fast. The 12 * 3 bytes = 36 registers
  */
      for (int n=0; n < 16; n++){  //
        cmd_with_stb(0b00001101); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(1);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        delayMicroseconds(1);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
            cmd_without_stb((0b11000000) | sum); // cmd 3 //wich define the start address (00H to 15H)
            cmd_without_stb(0b00000000); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(0b00000000); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.
            //
            digitalWrite(VFD_stb, HIGH);
            delayMicroseconds(2);
            sum=(sum +3);
     }
}
/*****************************************************************/
void seg(void)
{
  int sum=0;
  /*
  Here I test all registers 
  Could be done only on the number of grid
  to be more fast. The 12 * 3 bytes = 36 registers
  */
      for (int n=0; n < 16; n++){  //
        cmd_with_stb(0b00001101); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(1);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary execute a cmd "ON" display.
        delayMicroseconds(1);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
            cmd_without_stb((0b11000000)); // Start address 00;
            // 
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.// bar in middle(center)
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.// bar
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//bar
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.// left grid
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 1
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 2
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 3
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 4
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 5
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 6
            cmd_without_stb(0b00000000); cmd_without_stb(0b00100000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 7
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 8
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//dsp small
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.// right grid
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.
            //
            digitalWrite(VFD_stb, HIGH);
            delayMicroseconds(3000);
            Serial.println(((0b11000000) | sum), BIN);
     }
}
/*********************************************************************/
void led_on(void)
{
  byte myByte;
    digitalWrite(VFD_stb, LOW);
    delayMicroseconds(20);
    cmd_without_stb(0b01000001);
    delayMicroseconds(2);
    myByte ^=(0b00000001);  // Here is only to invert bit of led 1.
    cmd_without_stb(myByte);
    delayMicroseconds(2);
    digitalWrite(VFD_stb, HIGH);
    delayMicroseconds(2);
}
/*********************************************************************/
void all_ON(void)
{
  int sum=0;
  /*
  Here I test all registers 
  Could be done only on the number of grid
  to be more fast. The 12 * 3 bytes = 36 registers
  */
      for (int n=0; n < 16; n++){  //
        cmd_with_stb(0b00001101); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(1);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        delayMicroseconds(1);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
            cmd_without_stb((0b11000000) | sum); // cmd 3 //wich define the start address (00H to 15H)
            cmd_without_stb(0b11111111); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(0b11111111); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            //cmd_without_stb(0b11111111); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_4bitsWithout_stb(0b11111111); // only half byte of third byte.
            //
            digitalWrite(VFD_stb, HIGH);
            delayMicroseconds(3000);
            Serial.println(((0b11000000) | sum), BIN);
            sum=(sum +3);
     }
}
/***********************************************************************/
void segBySeg(void)
{
  int u=0;
  int sum=0;
  unsigned char word1, word2,word3,word4;
  unsigned int mask0=0b0000000000000001;
  unsigned int mask1=0b0000000000000001;
 
      for ( u =0; u < 32; u++){
        cmd_with_stb(0b00001101); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(1);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        delayMicroseconds(1);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
        cmd_without_stb((0b11000000) | sum); // cmd 3 //wich define the start address (00H to 15H)
            
        if (u<=15){
       word4 = (mask0 & 0xff000000) >>  24; // remaning segments 
       word3 = (mask0 & 0x00ff0000) >>  16; // remaning segments
       word2 = (mask0 & 0x0000ff00) >>  8; // grid 9 & 10 and segments
       word1 = (mask0 & 0x000000ff) >>  0; // grids
        mask0=(mask0 <<= 1);
        }
        if (u>15){
       word4 = (mask1 & 0xff000000) >>  24; // remaning segments 
       word3 = (mask1 & 0x00ff0000) >>  16; // remaning segments
       word2 = (mask1 & 0x0000ff00) >>  8; // grid 9 & 10 and segments
       word1 = (mask1 & 0x000000ff) >>  0; // grids
        mask1=(mask1 <<= 1);
        }
       //
       //Serial.println(mask0, BIN);
       //Serial.println(mask1, BIN);
            cmd_without_stb(word1); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(word2); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_4bitsWithout_stb(word3); // only half byte of third byte.
            //
            digitalWrite(VFD_stb, HIGH);
            delay(500);
            //Serial.println(((0b11000000) | sum), BIN);
            sum=(sum +3);
    }
}
/**************************************************************/
void loop() {
  for(int i = 0; i< 50;i++){
    SGNE_S0791_RunWeels();
  }
//
seg();
delay(1000);
//
segBySeg();
delay(500);
//
all_ON();
delay(3000);
//
clear_VFD();
//led_on();
//delay(500);
//
// Zone to test the small display of 7 segments
  for (int i=0; i<10; i++){
    send_char(0x24, segments[i], 0x00, 0x00); // Counter on the small, grid 11, display of 7 sements
    delay(50);
  }
/***************************************************************/
// Zone to do a test of the Bars
char pos = 0b00000001;
send_char(0x06, pos, 0x00, 0x00); // to bright the two bar below, color water marine.
  for (int i=0; i<8; i++){
    send_char(0x00, pos, 0x00, 0x00);
    send_char(0x03, pos, 0x00, 0x00);
    send_char(0x06, pos << 1, 0x00, 0x00);
    pos = pos << 1;
    delay(500);
  }
  send_char(0x00, pos << 1, 0x10, 0x00);
  delay(500);
/***************************************************************/
// Zone to test the numbers and letters
send_char(0x0C,number0[0], number0[1], number0[2]);
send_char(0x0F,number1[0], number1[1], number1[2]);
send_char(0x12,number2[0], number2[1], number2[2]);
send_char(0x15,number3[0], number3[1], number3[2]);
send_char(0x18,number4[0], number4[1], number4[2]);
send_char(0x1B,number5[0], number5[1], number5[2]);
send_char(0x1E,number6[0], number6[1], number6[2]);
send_char(0x21,number7[0], number7[1], number7[2]);
clear_VFD();
send_char(0x0C,number8[0], number8[1], number8[2]);
send_char(0x0F,number9[0], number9[1], number9[2]);
clear_VFD();
send_char(0x0C,letterA[0], letterA[1], letterA[2]);
send_char(0x0F,letterB[0], letterB[1], letterB[2]);
send_char(0x12,letterC[0], letterC[1], letterC[2]);
send_char(0x15,letterD[0], letterD[1], letterD[2]);
send_char(0x18,letterE[0], letterE[1], letterE[2]);
send_char(0x1B,letterF[0], letterF[1], letterF[2]);
send_char(0x1E,letterG[0], letterG[1], letterG[2]);
send_char(0x21,letterH[0], letterH[1], letterH[2]);
clear_VFD();
send_char(0x0C,letterI[0], letterI[1], letterI[2]);
send_char(0x0F,letterJ[0], letterJ[1], letterJ[2]);
send_char(0x12,letterK[0], letterK[1], letterK[2]);
send_char(0x15,letterL[0], letterL[1], letterL[2]);
send_char(0x18,letterM[0], letterM[1], letterM[2]);
send_char(0x1B,letterN[0], letterN[1], letterN[2]);
send_char(0x1E,letterO[0], letterO[1], letterO[2]);
send_char(0x21,letterP[0], letterP[1], letterP[2]);
clear_VFD();
send_char(0x0C,letterQ[0], letterQ[1], letterQ[2]);
send_char(0x0F,letterR[0], letterR[1], letterR[2]);
send_char(0x12,letterS[0], letterS[1], letterS[2]);
send_char(0x15,letterT[0], letterT[1], letterT[2]);
send_char(0x18,letterU[0], letterU[1], letterU[2]);
send_char(0x1B,letterV[0], letterV[1], letterV[2]);
send_char(0x1E,letterX[0], letterX[1], letterX[2]);
send_char(0x21,letterZ[0], letterZ[1], letterZ[2]);
clear_VFD();
send_char(0x0C,letterW[0], letterW[1], letterW[2]);
clear_VFD();
}
 
