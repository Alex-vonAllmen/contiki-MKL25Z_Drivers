/*
 * mpl115a2.c
 *
 *  Created on: May 11, 2014
 *      Author: alexanderwiniger
 */

#include "mpl115a2.h"
#include "WAIT1.h"
#include "GI2C1.h"

#define MPL_WAIT_MS(x)          WAIT1_Waitms(x)
#define MPL_ACTIVATE()          MPL_SHDN_SetVal()
#define MPL_DEACTIVATE()        MPL_SHDN_ClrVal()

static float mpl115a2_a0;
static float mpl115a2_b1;
static float mpl115a2_b2;
static float mpl115a2_c12;

/*---------------------------------------------------------------------------*/
uint8_t
mpl115a2_read_coeff(void)
{
  uint8_t res, data[8];

  res = GI2C1_SelectSlave(MPL115A2_ADDRESS);
  if (res != ERR_OK)
    {
      (void) GI2C1_UnselectSlave();
      return res;
    }
  data[0] = (uint8_t) (MPL115A2_ADDRESS | MPL115A2_WRITE_CMD);
  data[1] = (uint8_t) (MPL115A2_A0MSB | MPL115A2_READ_CMD);
  data[2] = (uint8_t) (MPL115A2_ADDRESS | MPL115A2_READ_CMD);
  res = GI2C1_WriteBlock(data, sizeof(data), GI2C1_SEND_STOP);
  if (res != ERR_OK)
    {
      (void) GI2C1_UnselectSlave();
      return res;
    }

  res = GI2C1_ReadBlock(&data, sizeof(data), GI2C1_SEND_STOP);
  if (res != ERR_OK)
    {
      (void) GI2C1_UnselectSlave();
      return res;
    }

  mpl115a2_a0 = (float) (((uint16_t) data[0]) | data[1]) / 8;
  mpl115a2_b1 = (float) (((uint16_t) data[2]) | data[3]) / 8192;
  mpl115a2_b2 = (float) (((uint16_t) data[4]) | data[5]) / 16384;
  mpl115a2_c12 = (float) (((uint16_t) data[6]) | data[7]);
  mpl115a2_c12 /= 4194304.0;

  return GI2C1_UnselectSlave();
}
/*---------------------------------------------------------------------------*/
uint8_t
mpl115a2_measure_press_temp(float* temperature, float* pressure)
{
  uint8_t res, data[8];

  /* Select slave with MPL115A2 address */
  res = GI2C1_SelectSlave(MPL115A2_ADDRESS);
  if (res != ERR_OK)
    {
      (void) GI2C1_UnselectSlave();
      return res;
    }

  /* Send START CONVERSION command */
  data[0] = (uint8_t) (MPL115A2_ADDRESS | MPL115A2_WRITE_CMD);
  data[1] = (uint8_t) MPL115A2_CONVERT;
  res = GI2C1_WriteBlock(data, 2 * sizeof(uint8_t), GI2C1_SEND_STOP); /* send address and data */
  if (res != ERR_OK)
    {
      (void) GI2C1_UnselectSlave();
      return res;
    }
  MPL_WAIT_MS(5);

  /* Send READ PRESSURE MSB command */
  data[0] = (uint8_t) (MPL115A2_ADDRESS | MPL115A2_WRITE_CMD);
  data[1] = (uint8_t) MPL115A2_PADC_MSB;
  data[2] = (uint8_t) (MPL115A2_ADDRESS | MPL115A2_READ_CMD);
  res = GI2C1_WriteBlock(data, 3 * sizeof(uint8_t), GI2C1_SEND_STOP); /* send address and data */
  if (res != ERR_OK)
    {
      (void) GI2C1_UnselectSlave();
      return res;
    }

  res = GI2C1_ReadBlock(&data, sizeof(data), GI2C1_SEND_STOP);
  if (res != ERR_OK)
    {
      (void) GI2C1_UnselectSlave();
      return res;
    }
  return GI2C1_UnselectSlave();
}
/*---------------------------------------------------------------------------*/
int16_t
mpl115a2_calc_press_comp(uint16_t Padc, uint16_t Tadc, int16_t a0, int16_t b1,
    int16_t b2, int16_t c12)
{
  int32_t c12x2, a1, a1x1, y1, a2x2, PComp;

  Padc >>= 6; //Note that the Padc is the raw value from Pegasus >>6 since its 10 bit unsigned
  Tadc >>= 6; //Note that the Tadc is the raw value from Pegasus >>6 since its 10 bit unsigned

  c12x2 = (((int32_t) c12) * Tadc) >> 11; // c12x2 = c12 * Tadc a1 = (sint32)b1 + c12x2; // 
  a1 = b1 + c12x2;
  a1x1 = a1 * Padc; // a1x1 =a1 *Padc
  y1 = (((int32_t) a0) << 10) + a1x1; // y1 = a0 + a1x1
  a2x2 = (((int32_t) b2) * Tadc) >> 1;
  PComp = (y1 + a2x2) >> 9;
  return (int16_t) PComp;
}
/*---------------------------------------------------------------------------*/
uint8_t
mpl115a2_init(void)
{
  MPL_ACTIVATE(); /* Enable device */
  MPL_WAIT_MS(5); /* Wakeup time */
  return mpl115a2_read_coeff();
}
/*---------------------------------------------------------------------------*/
uint8_t
mpl115a2_measure_press(void)
{
  return 0;
}
/*---------------------------------------------------------------------------*/
uint8_t
mpl115a2_measure_temp(void)
{
  return 0;
}
/*---------------------------------------------------------------------------*/

