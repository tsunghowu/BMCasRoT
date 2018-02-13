//==============================================================================
// MODULE       = GPIO.c
// SHORT DESC.  = Functions for GPIO functions
// DEVICE       = Tiva-C
// =============================================================================

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <stdbool.h>

/* XDCtools Header files */
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* RTOS Driver files */
#include <ti/drivers/GPIO.h>

/* Tiva-Ware Header files */
#include <driverlib/gpio.h>
#include <driverlib/rom_map.h>
#include <inc/hw_memmap.h>
/*


 Remove the confidential parts.
 
*/



//GPIO_HWAttrs user_gpioHWAttrs[USER_GPIOCOUNT] = {
//    {GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_INPUT},    /* IO0 = BMC_GPI0 */
//    {GPIO_PORTJ_BASE, GPIO_PIN_1, GPIO_INPUT},    /* IO1 = BMC_GPI1 */
//    {GPIO_PORTJ_BASE, GPIO_PIN_2, GPIO_INPUT},    /* IO2 = BMC_GPI2 */
//    {GPIO_PORTJ_BASE, GPIO_PIN_3, GPIO_INPUT},    /* IO3 = BMC_GPI3 */

#define SPI_MISO  user_gpioHWAttrs[0]  //GPI0
#define SPI_MOSI  user_gpioHWAttrs[1]  //GPI1
#define SPI_CK    user_gpioHWAttrs[2]  //GPI2
#define SPI_CS    user_gpioHWAttrs[3]  //GPI3


#ifndef SHA256_H
#define SHA256_H

/*************************** HEADER FILES ***************************/

/****************************** MACROS ******************************/
#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int  WORD;             // 32-bit word, change to "long" for 16-bit machines

typedef struct {
	BYTE data[64];
	WORD datalen;
	unsigned long long bitlen;
	WORD state[8];
} SHA256_CTX;

SHA256_CTX ctx;

/*********************** FUNCTION DECLARATIONS **********************/
void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len);
void sha256_final(SHA256_CTX *ctx, BYTE hash[]);

#endif   // SHA256_H


/****************************** MACROS ******************************/
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

/**************************** VARIABLES *****************************/
static const WORD k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

/*********************** FUNCTION DEFINITIONS ***********************/
void sha256_transform(SHA256_CTX *ctx, const BYTE data[])
{
	WORD a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	for ( ; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len)
{
	WORD i;

	for (i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			sha256_transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}
void MemSet(unsigned char *pBuffer, unsigned char value, unsigned long length)
{
  unsigned long i;
  while(length--){
    pBuffer[i] = value;
  }
}

void sha256_final(SHA256_CTX *ctx, BYTE hash[])
{
	WORD i;

	i = ctx->datalen;

	// Pad whatever data is left in the buffer.
	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else {
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;
		sha256_transform(ctx, ctx->data);
		MemSet(ctx->data, 0, 56);
	}

	// Append to the padding the total message's length in bits and transform.
	ctx->bitlen += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	sha256_transform(ctx, ctx->data);

	// Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for (i = 0; i < 4; ++i) {
		hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

unsigned long ALI_6_11_0_BIOS_SIGNED_BSB_bin_len = 16384;
unsigned long gOffsetToALIBSB = 0;
unsigned char *gSPIBufferPtr;

/*
void spiWriteReg(const unsigned char regAddr, const unsigned char regData)
{
  unsigned char SPICount;                               // Counter used to clock out the data
  unsigned char SPIData;                                // Define a data structure for the SPI data.

  SPI_CS = 1;                                           // Make sure we start with /CS high
  SPI_CK = 0;                                           // and CK low

  SPIData = regAddr;                                    // Preload the data to be sent with Address
  SPI_CS = 0;                                           // Set /CS low to start the SPI cycle 25nS
                                                        // Although SPIData could be implemented as an "int", resulting in one
                                                        // loop, the routines run faster when two loops are implemented with
                                                        // SPIData implemented as two "char"s.
  for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Address byte
  {
    if (SPIData & 0x80)                                 // Check for a 1
      SPI_MOSI = 1;                                     // and set the MOSI line appropriately
    else
      SPI_MOSI = 0;
    SPI_CK = 1;                                         // Toggle the clock line
    SPI_CK = 0;
    SPIData <<= 1;                                      // Rotate to get the next bit

  }                                                     // and loop back to send the next bit
                                                        // Repeat for the Data byte
  SPIData = regData;                                    // Preload the data to be sent with Data
  for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Data
  {
    if (SPIData & 0x80)
      SPI_MOSI = 1;
    else
      SPI_MOSI = 0;
    SPI_CK = 1;
    SPI_CK = 0;
    SPIData <<= 1;
  }          
  SPI_CS = 1;
  SPI_MOSI = 0;
  
}

unsigned char spiReadReg (const unsigned char regAddr)
{

  unsigned char SPICount;                               // Counter used to clock out the data
  
  unsigned char SPIData;                  
  
  SPI_CS = 1;                                           // Make sure we start with /CS high
  SPI_CK = 0;                                           // and CK low
  SPIData = regAddr;                                    // Preload the data to be sent with Address & Data

  SPI_CS = 0;                                           // Set /CS low to start the SPI cycle
  for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Address & Data
  {
    if (SPIData & 0x80)
      SPI_MOSI = 1;
    else
      SPI_MOSI = 0;
    SPI_CK = 1;
    SPI_CK = 0;
    SPIData <<= 1;
  }                                                     // and loop back to send the next bit   
  SPI_MOSI = 0;                                         // Reset the MOSI data line
  
  SPIData = 0;
  for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock in the data to be fread
  {
    SPIData <<=1;                                       // Rotate the data
    SPI_CK = 1;                                         // Raise the clock to clock the data out of the MAX7456
    SPIData += SPI_MISO;                                // Read the data bit
    SPI_CK = 0;                                         // Drop the clock ready for th enext bit
  }                                                     // and loop back
  SPI_CS = 1;                                           // Raise CS   
                      
  return ((unsigned char)SPIData);                      // Finally return the read data
}

*/
#define GPIO_HIGH_LOW_MACRO(PIN, level) MAP_GPIOPinWrite(PIN.port, PIN.pin, level)

unsigned char spiWriteRead(unsigned char opcode, unsigned char regAddr[], const unsigned char length)
{
  unsigned char SPICount, i;                               // Counter used to clock out the data
  unsigned char *SPIData;                                // Define a data structure for the SPI data.
  unsigned char SPIReadBackData;
  unsigned char SpiBuffer[64];

  GPIO_HIGH_LOW_MACRO(SPI_CS, GPIO_HIGH); //SPI_CS = 1;                                           // Make sure we start with /CS high
  GPIO_HIGH_LOW_MACRO(SPI_CK, GPIO_LOW); //SPI_CK = 0;                                           // and CK low

  SPIData = &opcode;                                    // Preload the data to be sent with Address
  GPIO_HIGH_LOW_MACRO(SPI_CS, GPIO_LOW);   //SPI_CS = 0;                                           // Set /CS low to start the SPI cycle 25nS
                                                        // Although SPIData could be implemented as an "int", resulting in one
                                                        // loop, the routines run faster when two loops are implemented with
                                                        // SPIData implemented as two "char"s.
  for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Address byte
  {
    if (*SPIData & 0x80)                                 // Check for a 1
      GPIO_HIGH_LOW_MACRO(SPI_MOSI, GPIO_HIGH); //SPI_MOSI = 1;                                     // and set the MOSI line appropriately
    else
      GPIO_HIGH_LOW_MACRO(SPI_MOSI, GPIO_LOW); //SPI_MOSI = 0;
    GPIO_HIGH_LOW_MACRO(SPI_CK, GPIO_HIGH); //SPI_CK = 1;                                         // Toggle the clock line
    GPIO_HIGH_LOW_MACRO(SPI_CK, GPIO_LOW); //SPI_CK = 0;
    *SPIData <<= 1;                                      // Rotate to get the next bit
  }                                                     // and loop back to send the next bit
                                                        // Repeat for the Data byte  
  for(i=0;i<length;i++) {
    SPIData = &regAddr[i];                                    // Preload the data to be sent with Data
    for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Data
    {
      if (*SPIData & 0x80)
        GPIO_HIGH_LOW_MACRO(SPI_MOSI, GPIO_HIGH);  //SPI_MOSI = 1;
      else
        GPIO_HIGH_LOW_MACRO(SPI_MOSI, GPIO_LOW);  //SPI_MOSI = 0;
      GPIO_HIGH_LOW_MACRO(SPI_CK, GPIO_HIGH);  //SPI_CK = 1;
      GPIO_HIGH_LOW_MACRO(SPI_CK, GPIO_LOW);  //SPI_CK = 0;
      *SPIData <<= 1;
    }
  }       
  GPIO_HIGH_LOW_MACRO(SPI_MOSI, GPIO_LOW); //SPI_MOSI = 0;                                         // Reset the MOSI data line

  SPIReadBackData = 0;
  for(i=0;i<64;i++) { 
    for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock in the data to be fread
    {
      SPIReadBackData <<=1;                                       // Rotate the data
      GPIO_HIGH_LOW_MACRO(SPI_CK, GPIO_HIGH); //SPI_CK = 1;                                         // Raise the clock to clock the data out of the MAX7456
      
      SPIReadBackData += 
        MAP_GPIOPinRead(SPI_MISO.port, SPI_MISO.pin);
      
        //SPI_MISO;                                // Read the data bit
      GPIO_HIGH_LOW_MACRO(SPI_CK, GPIO_LOW); //SPI_CK = 0;                                         // Drop the clock ready for th enext bit
      
    }                                                     // and loop back
    SpiBuffer[i] = SPIReadBackData;
  }
  
  GPIO_HIGH_LOW_MACRO(SPI_CS, GPIO_HIGH);   //SPI_CS = 1;
  sha256_update(&ctx, SpiBuffer, 64 );
  
  return SPIReadBackData;
}

void PerformSPIRead(void) {

  unsigned char spi_address[] = {0xFF, 0xFF, 0xC0, 0x00};
  
  unsigned long pSpi;
  unsigned long i;
  GPIO_write(BMC_STATUS, LED_ON);
  
  sha256_init(&ctx);
  
  /* Prepare all GPIO pins */
    // Switch to Fail-Safe-BIOS
    GPIO_write(SPI_CS0_0_EN, GPIO_HIGH);//reverse
    GPIO_write(SPI_CS1_0_EN, GPIO_HIGH);
    GPIO_write(SPI_CS1_1_EN, GPIO_LOW);
    GPIO_write(SPI_CSN_0_EN, GPIO_LOW);
    GPIO_write(SPI_CSN_1_EN, GPIO_LOW);
    GPIO_write(BMC_BBS, GPIO_HIGH);    // 0=LPC, 1=SPI

    MAP_GPIOPinTypeGPIOOutput(SPI_MOSI.port, SPI_MOSI.pin);
  MAP_GPIOPinTypeGPIOOutput(SPI_CK.port, SPI_CK.pin);
  MAP_GPIOPinTypeGPIOOutput(SPI_CS.port, SPI_CS.pin);
  MAP_GPIOPinWrite(SPI_CS.port, SPI_CS.pin, GPIO_HIGH);
  MAP_GPIOPinWrite(SPI_MOSI.port, SPI_MOSI.pin, GPIO_HIGH);
  MAP_GPIOPinWrite(SPI_CK.port, SPI_CK.pin, GPIO_HIGH);

  MAP_GPIOPinTypeGPIOInput(SPI_MISO.port, SPI_MISO.pin);
  GPIOPadConfigSet(SPI_MISO.port, SPI_MISO.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
  /* Prepare all GPIO pins, done. */
  
  i=(16*1024)/64;
  pSpi = 0x3FC00000;
  spi_address[0] = (unsigned char)(pSpi >> 24);
    spi_address[1] = (unsigned char)(pSpi >> 16);
    spi_address[2] = (unsigned char)(pSpi >> 8);
    spi_address[3] = (unsigned char)(pSpi >> 0);
   
  while(i--) {
    spiWriteRead(0x0B, spi_address, sizeof(spi_address));	//Read 64 bytes from SPI and feed to SHA-256 to update hash at the same time.
    pSpi += (0x40 <<8);	
    spi_address[0] = (unsigned char)(pSpi >> 24);
    spi_address[1] = (unsigned char)(pSpi >> 16);
    spi_address[2] = (unsigned char)(pSpi >> 8);
    spi_address[3] = (unsigned char)(pSpi >> 0);
  }
  
  GPIO_write(BMC_STATUS, LED_OFF);

  MAP_GPIOPinTypeGPIOInput(SPI_MOSI.port, SPI_MOSI.pin);
  GPIOPadConfigSet(SPI_MOSI.port, SPI_MOSI.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

  MAP_GPIOPinTypeGPIOInput(SPI_CS.port, SPI_CS.pin);
  GPIOPadConfigSet(SPI_CS.port, SPI_CS.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

  MAP_GPIOPinTypeGPIOInput(SPI_MISO.port, SPI_MISO.pin);
  GPIOPadConfigSet(SPI_MISO.port, SPI_MISO.pin, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
  
/*Below pin must be tri-state or need a circuit to make it not touch SPI CLK pin. */
  MAP_GPIODirModeSet(SPI_CK.port, SPI_CK.pin, GPIO_DIR_MODE_HW);  
  MAP_GPIOPinTypeGPIOInput(SPI_CK.port, SPI_CK.pin);
//  GPIOPadConfigSet(SPI_CK.port, SPI_CK.pin, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_WAKE_HIGH);
  //GPIOPadConfigSet(SPI_CK.port, SPI_CK.pin, GPIO_PIN_TYPE_STD);
  GPIO_HIGH_LOW_MACRO(SPI_CS, GPIO_HIGH);   //SPI_CS = 1;
  
#if 1 
  {
        BYTE buf[SHA256_BLOCK_SIZE];
        BYTE TestPattern[] = { 0x12, 0x23, 0x34, 0x45 };
        unsigned char Customer_611_bin[SHA256_BLOCK_SIZE] = {
            0xdb, 0xed, 0x00, 0x6b, 0xd7, 0x22, 0xdc, 0x8c, 0xa9, 0xe2, 0x56, 0x51, 0xa5, 0xcd, 0x18, 0xad,
            0xe6, 0x09, 0xa3, 0x61, 0xf4, 0xe4, 0x72, 0x39, 0x79, 0x9a, 0xc6, 0x20, 0xc6, 0xc0, 0xe8, 0x9f
         };

	int idx;
        
	sha256_final(&ctx, buf);  
        idx = 0;
        for(idx=0;idx<32;idx++){
          if( Customer_611_bin[idx] != buf[idx] ){
              System_printf("sha 256 mismatch!!! \n");
              while( 1 ){ 
                Task_sleep(1000);
                GPIO_write(BMC_STATUS, LED_ON);
                Task_sleep(1000);
                GPIO_write(BMC_STATUS, LED_OFF);
                
                Task_sleep(200);
                GPIO_write(BMC_STATUS, LED_ON);
                Task_sleep(200);
                GPIO_write(BMC_STATUS, LED_OFF);

                Task_sleep(200);
                GPIO_write(BMC_STATUS, LED_ON);
                Task_sleep(200);
                GPIO_write(BMC_STATUS, LED_OFF);

                Task_sleep(200);
                GPIO_write(BMC_STATUS, LED_ON);
                Task_sleep(200);
                GPIO_write(BMC_STATUS, LED_OFF);
              }
          }
        }
        System_printf("sha 256 match!!! \n");
  }  
#endif
  return;
}