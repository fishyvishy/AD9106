/******************************************************************************
    @file:  ad9106.h

    @brief: Library for EVAL-AD9106 DAC arbitrary wave-form generator designed
            to work as header for Arduino UNO R3
******************************************************************************/

#ifndef __ad9106_h__
#define __ad9106_h__

// specify if on-board op-amps are used

#include "Arduino.h"

// enum for DAC Channels to constrain function parameters
enum CHNL { CHNL_1 = 1, CHNL_2, CHNL_3, CHNL_4 };

// enum for channel properties to constrain function parameters
enum CHNL_PROP {
  DOFFSET = 0x0025,
  DGAIN = 0x0035,
  DDS_PHASE = 0x0043,
  START_DELAY = 0x005c
};

enum ErrorCode {
  SUCCESS = 0,
  MEM_READ_ERR = 2,
  ODD_ADDR_ERR = 3,
  PERIOD_SHORT_ERR = 5,
  DOUT_START_SHORT_ERR = 7,
  PAT_DLY_SHORT_ERR = 11,
  DOUT_START_LG_ERR = 13
};

class AD9106 {
 public:
  int cs;
  int reset;
  float fclk;

  /*** SPI register addresses ***/
  uint16_t reg_add[66] = {
      0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
      0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x001f, 0x0020, 0x0022,
      0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002a, 0x002b,
      0x002c, 0x002d, 0x002e, 0x002f, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034,
      0x0035, 0x0036, 0x0037, 0x003e, 0x003f, 0x0040, 0x0041, 0x0042, 0x0043,
      0x0044, 0x0045, 0x0047, 0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055,
      0x0056, 0x0057, 0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e,
      0x005f, 0x001e, 0x001d};

  /*** 4-Wire SPI, Reset, and Trigger configuration & constructor ***/
  AD9106(int CS = 10,
         int RESET = 8,
         int TRIGGER = 7,
         int EN_CVDDX = 2,
         int SHDN = 4);

  // Function to initialize GPIO pins on Arduino
  void begin(bool OP_AMPS = false, float FCLK = NULL);

  // Function to reset register values
  void reg_reset();

  // Function to start pattern generation
  void start_pattern();

  // Function to stop pattern generation
  void stop_pattern();

  // Function to update pattern with new register values
  void update_pattern();

  // Function to end AD9106 operation
  void end();

  // Function to set register properties
  int set_CHNL_prop(CHNL_PROP property, CHNL chnl, int16_t value);

  // Wrapper Functions for set_CHNL_prop
  int set_CHNL_DOFFSET(CHNL chnl, int16_t offset);
  int set_CHNL_DGAIN(CHNL chnl, int16_t gain);
  int set_CHNL_DDS_PHASE(CHNL chnl, int16_t phase);
  int set_CHNL_START_DELAY(CHNL chnl, int16_t start_delay);

  // Functions to set/get DDS frequency
  int setDDSfreq(float freq);
  float getDDSfreq();

  // Function to setup SPI with communication speed of [hz]
  void spi_init(uint32_t hz);

  // SPI write function
  int16_t spi_write(uint16_t addr, int16_t data);

  // SPI read function
  int16_t spi_read(uint16_t addr);

  // Publicly accessible register addresses for common operations
  static const uint16_t RAMUPDATE;
  static const uint16_t PAT_STATUS;
  static const uint16_t WAV4_3CONFIG;
  static const uint16_t WAV2_1CONFIG;
  static const uint16_t DDSTW_MSB;
  static const uint16_t DDSTW_LSB;
  static const uint16_t CFG_ERROR;

 private:
  int _en_cvddx;
  int _trigger;
  int _shdn;

  /*
   * @brief Get address for specific DAC based on given base address
   * @param base_addr - constant base address
   * @param dac - the specific dac channel
   * @return dac_addr - the address for dac relative to base
   */
  uint16_t get_dac_addr(uint16_t base_addr, CHNL dac) {
    // DAC specific registers after 0x0050 are grouped in 4s
    if (base_addr < 0x0050) {
      return base_addr - (dac - 1);
    }
    return base_addr - 4 * (dac - 1);
  }

  // Error Handling Function
  int get_error();
};

#endif
