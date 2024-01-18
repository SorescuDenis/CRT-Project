//different modes
#define I2C_MODE_ACK0 0
#define I2C_MODE_ACK1 1
#define I2C_MODE_READ 2

#define CRYSTAL_FREQUENCY 12000000
#define PLL_FACTOR        1
#define VPBDIV_FACTOR     4


/******************************************************************************
 * Defines, macros, and typedefs
 *****************************************************************************/
//Return codes
#define I2C_CODE_OK     1
#define I2C_CODE_ERROR  0
#define I2C_CODE_EMPTY -1
#define I2C_CODE_BUSY  -2

//Command codes for transmitting extra bytes
#define I2C_EXTRA_NONE 0
#define I2C_EXTRA_BYTE 1
#define I2C_EXTRA_WORD 2

