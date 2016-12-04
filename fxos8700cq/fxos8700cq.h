#ifndef FXOS8700CQ_H
#define FXOS8700CQ_H

#include "mbed.h" // Building this for the mbed platform

#define I2C_400K 400000

// FXOS8700CQ I2C address
#define FXOS8700CQ_SLAVE_ADDR0 (0x1E<<1) // with pins SA0=0, SA1=0
#define FXOS8700CQ_SLAVE_ADDR1 (0x1D<<1) // with pins SA0=1, SA1=0
#define FXOS8700CQ_SLAVE_ADDR2 (0x1C<<1) // with pins SA0=0, SA1=1
#define FXOS8700CQ_SLAVE_ADDR3 (0x1F<<1) // with pins SA0=1, SA1=1

// FXOS8700CQ internal register addresses
#define FXOS8700CQ_STATUS 0x00
#define FXOS8700CQ_OUT_X_MSB 0x01
#define FXOS8700CQ_WHOAMI 0x0D
#define FXOS8700CQ_M_OUT_X_MSB 0x33

#define FXOS8700CQ_XYZ_DATA_CFG 0x0E

#define FXOS8700CQ_CTRL_REG1 0x2A
#define FXOS8700CQ_CTRL_REG2 0x2B
#define FXOS8700CQ_CTRL_REG3 0x2C
#define FXOS8700CQ_CTRL_REG4 0x2D
#define FXOS8700CQ_CTRL_REG5 0x2E

#define FXOS8700CQ_M_CTRL_REG1 0x5B
#define FXOS8700CQ_M_CTRL_REG2 0x5C
#define FXOS8700CQ_M_CTRL_REG3 0x5D

/* start MPL additions */
//MPL interrupts
#define FXOS8700CQ_INT_SOURCE 0x0C
//#define FXOS8700CQ_CTRL_REG2 0x2B   //bit 2 turns on auto-sleep
//#define FXOS8700CQ_CTRL_REG3 0x2C  //interrupt control register
//#define FXOS8700CQ_CTRL_REG4 0x2D  //interrupt enable register
//#define FXOS8700CQ_CTRL_REG5 0x2E //interrupt routing config register, by default all routed to INT2

//MPL motion detection
#define FXOS8700CQ_A_FFMT_CFG 0x15
#define FXOS8700CQ_A_FFMT_SRC 0x16
#define FXOS8700CQ_A_FFMT_THS 0x17
#define FXOS8700CQ_A_FFMT_COUNT 0x18
#define FXOS8700CQ_A_FFMT_THS_X_MSB 0x73
#define FXOS8700CQ_A_FFMT_THS_X_LSB 0x74
#define FXOS8700CQ_A_FFMT_THS_Y_MSB 0x75
#define FXOS8700CQ_A_FFMT_THS_Y_LSB 0x76
#define FXOS8700CQ_A_FFMT_THS_Z_MSB 0x77
#define FXOS8700CQ_A_FFMT_THS_Z_LSB 0x78
/* end MPL additions */

// FXOS8700CQ configuration macros, per register

#define FXOS8700CQ_CTRL_REG1_ASLP_RATE2(x) (x << 6) // x is 2-bit
#define FXOS8700CQ_CTRL_REG1_DR3(x) (x << 3) // x is 3-bit
#define FXOS8700CQ_CTRL_REG1_LNOISE (1 << 2)
#define FXOS8700CQ_CTRL_REG1_F_READ (1 << 1)
#define FXOS8700CQ_CTRL_REG1_ACTIVE (1 << 0)

#define FXOS8700CQ_CTRL_REG2_ST (1 << 7)
#define FXOS8700CQ_CTRL_REG2_RST (1 << 6)
#define FXOS8700CQ_CTRL_REG2_SMODS2(x) (x << 3) // x is 2-bit
#define FXOS8700CQ_CTRL_REG2_SLPE (1 << 2)
#define FXOS8700CQ_CTRL_REG2_MODS2(x) (x << 0) // x is 2-bit

#define FXOS8700CQ_CTRL_REG3_FIFO_GATE (1 << 7)
#define FXOS8700CQ_CTRL_REG3_WAKE_TRANS (1 << 6)
#define FXOS8700CQ_CTRL_REG3_WAKE_LNDPRT (1 << 5)
#define FXOS8700CQ_CTRL_REG3_WAKE_PULSE (1 << 4)
#define FXOS8700CQ_CTRL_REG3_WAKE_FFMT (1 << 3)
#define FXOS8700CQ_CTRL_REG3_WAKE_A_VECM (1 << 2)
#define FXOS8700CQ_CTRL_REG3_IPOL (1 << 1)
#define FXOS8700CQ_CTRL_REG3_PP_OD (1 << 0)

#define FXOS8700CQ_CTRL_REG4_INT_EN_ASLP (1 << 7)
#define FXOS8700CQ_CTRL_REG4_INT_EN_FIFO (1 << 6)
#define FXOS8700CQ_CTRL_REG4_INT_EN_TRANS (1 << 5)
#define FXOS8700CQ_CTRL_REG4_INT_EN_LNDPRT (1 << 4)
#define FXOS8700CQ_CTRL_REG4_INT_EN_PULSE (1 << 3)
#define FXOS8700CQ_CTRL_REG4_INT_EN_FFMT (1 << 2)
#define FXOS8700CQ_CTRL_REG4_INT_EN_A_VECM (1 << 1)
#define FXOS8700CQ_CTRL_REG4_INT_EN_DRDY (1 << 0)

#define FXOS8700CQ_CTRL_REG5_INT_CFG_ASLP (1 << 7)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_FIFO (1 << 6)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_TRANS (1 << 5)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_LNDPRT (1 << 4)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_PULSE (1 << 3)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_FFMT (1 << 2)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_A_VECM (1 << 1)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_DRDY (1 << 0)

#define FXOS8700CQ_XYZ_DATA_CFG_HPF_OUT (1 << 4)
#define FXOS8700CQ_XYZ_DATA_CFG_FS2(x) (x << 0) // x is 2-bit

#define FXOS8700CQ_M_CTRL_REG1_M_ACAL (1 << 7)
#define FXOS8700CQ_M_CTRL_REG1_M_RST (1 << 6)
#define FXOS8700CQ_M_CTRL_REG1_M_OST (1 << 5)
#define FXOS8700CQ_M_CTRL_REG1_MO_OS3(x) (x << 2) // x is 3-bit
#define FXOS8700CQ_M_CTRL_REG1_M_HMS2(x) (x << 0) // x is 2-bit

#define FXOS8700CQ_M_CTRL_REG2_HYB_AUTOINC_MODE (1 << 5)
#define FXOS8700CQ_M_CTRL_REG2_M_MAXMIN_DIS (1 << 4)
#define FXOS8700CQ_M_CTRL_REG2_M_MAXMIN_DIS_THS (1 << 3)
#define FXOS8700CQ_M_CTRL_REG2_M_MAXMIN_RST (1 << 2)
#define FXOS8700CQ_M_CTRL_REG2_M_RST_CNT2(x) (x << 0) // x is 2-bit

#define FXOS8700CQ_M_CTRL_REG3_M_RAW (1 << 7)
#define FXOS8700CQ_M_CTRL_REG3_M_ASLP_OS3(x) (x << 4) // x is 3-bit
#define FXOS8700CQ_M_CTRL_REG3_M_THS_XYZ_UPDATE (1 << 3)
#define FXOS8700CQ_M_CTRL_REG3_M_ST_Z (1 << 2)
#define FXOS8700CQ_M_CTRL_REG3_M_ST_XY2(x) (x << 0) // x is 2-bit

// FXOS8700CQ WHOAMI production register value
#define FXOS8700CQ_WHOAMI_VAL 0xC7

// 6 channels of two bytes = 12 bytes; read from FXOS8700CQ_OUT_X_MSB
#define FXOS8700CQ_READ_LEN 12

// For processing the accelerometer data to right-justified 2's complement
#define UINT14_MAX 16383

// TODO: struct to hold the data out of the sensor
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} SRAWDATA;


/**
* A driver on top of mbed-I2C to operate the FXOS8700CQ accelerometer/magnetometer
* on the FRDM-K64F.
*
* Code has been completed, but likely not optimized and potentially buggy.
*/
class FXOS8700CQ
{
public:
    /**
    * FXOS8700CQ constructor
    *
    * @param sda SDA pin
    * @param sdl SCL pin
    * @param addr address of the I2C peripheral in (7-bit << 1) form
    */
    FXOS8700CQ(PinName sda, PinName scl, int addr);

    /**
    * FXOS8700CQ destructor
    */
    ~FXOS8700CQ(void);

    void enable(void);
    void disable(void);

    /**
    * @return the contents of device register FXOS8700CQ_WHOAMI 0x0D,
    * should be FXOS8700CQ_WHOAMI_VAL 0xC7
    */
    uint8_t get_whoami(void);
    
    /**
    * @return the contents of device register FXOS8700CQ_STATUS 0x00
    */
    uint8_t status(void);

    /**
    * Data retrieval from the FXOS8700CQ
    *
    * @param accel_data destination XYZ accelerometer data struct
    * @param magn_data destination XYZ magnetometer data struct
    * @return 0 on success, non-zero on failure
    */
    uint8_t get_data(SRAWDATA *accel_data, SRAWDATA *magn_data);

    /**
    * Retrieve the full-range scale value of the accelerometer
    *
    * @return 2, 4, or 8, depending on part configuration; 0 on error
    */
    uint8_t get_accel_scale(void);

    /**
    * configure external interrupts
    *
    * @return 2, 4, or 8, depending on part configuration; 0 on error
    */
    uint8_t config_int(void);
    
    /**
    * configure feature (tap detection, motion detection, etc)
    *
    * @return 2, 4, or 8, depending on part configuration; 0 on error
    */
    uint8_t config_feature(void);

    /**
    * clear interrupt
    *
    * @return 2, 4, or 8, depending on part configuration; 0 on error
    */    
    void clear_int(void);

private:
    I2C dev_i2c; // instance of the mbed I2C class
    uint8_t dev_addr; // Device I2C address, in (7-bit << 1) form
    bool enabled; // keep track of enable bit of device

    // I2C helper methods
    void read_regs(int reg_addr, uint8_t* data, int len);
    void write_regs(uint8_t* data, int len);

};

#endif
