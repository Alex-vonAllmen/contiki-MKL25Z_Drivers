/*
 * mpl115a2.h
 *
 *  Created on: May 11, 2014
 *      Author: alexanderwiniger
 */

#ifndef MPL115A2_H_
#define MPL115A2_H_

/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* I2C Commands */
#define MPL115A2_WRITE_CMD      (0x01)
#define MPL115A2_READ_CMD       (0x00)

/* I2C Address */
#define MPL115A2_ADDRESS        (0xC0) // 11000000

/* MPL115A2 - Peripheral register structure */
typedef struct MPL115A2_MemMap {
  uint8_t PADC_MSB;             /**< 10-Bit Pressure Output Value MSB, offset: 0x00 */
  uint8_t PADC_LSB;             /**< 10-Bit Pressure Output Value LSB, offset: 0x01 */
  uint8_t TADC_MSB;             /**< 10-Bit Temperature Output Value MSB, offset: 0x02 */
  uint8_t TADC_LSB;             /**< 10-Bit Temperature Output Value LSB, offset: 0x03 */
  uint8_t A0MSB;                /**< A0 Coefficient MSB, offset: 0x04 */
  uint8_t A0LSB;                /**< A0 Coefficient LSB, offset: 0x05 */
  uint8_t B1MSB;                /**< B1 Coefficient MSB, offset: 0x06 */
  uint8_t B1LSB;                /**< B1 Coefficient LSB, offset: 0x07 */
  uint8_t B2MSB;                /**< B1 Coefficient MSB, offset: 0x08 */
  uint8_t B2LSB;                /**< B1 Coefficient LSB, offset: 0x09 */
  uint8_t C12MSB;               /**< C12 Coefficient MSB, offset: 0x0A */
  uint8_t C12LSB;               /**< C12 Coefficient LSB, offset: 0x0B */
  uint8_t Reserved[6];
  uint8_t CONVERT;              /**< System Command to Start both Pressure and Temperature Conversion, offset: 0x12 */
} volatile *MPL115A2_MemMapPtr;

/* MPL115A2 - Register accessors */
#define MPL115A2_PADC_MSB_REG(base)             ((base)->PADC_MSB)
#define MPL115A2_PADC_LSB_REG(base)             ((base)->PADC_LSB)
#define MPL115A2_TADC_MSB_REG(base)             ((base)->TADC_MSB)
#define MPL115A2_TADC_LSB_REG(base)             ((base)->TADC_LSB)
#define MPL115A2_A0MSB_REG(base)                ((base)->A0MSB)
#define MPL115A2_A0LSB_REG(base)                ((base)->A0LSB)
#define MPL115A2_B1MSB_REG(base)                ((base)->B1MSB)
#define MPL115A2_B1LSB_REG(base)                ((base)->B1LSB)
#define MPL115A2_B2MSB_REG(base)                ((base)->B2MSB)
#define MPL115A2_B2LSB_REG(base)                ((base)->B2LSB)
#define MPL115A2_C12MSB_REG(base)               ((base)->C12MSB)
#define MPL115A2_C12LSB_REG(base)               ((base)->C12LSB)
#define MPL115A2_CONVERT_REG(base)              ((base)->CONVERT)

/* Register Masks */
#define MPL115A2_PADC_MSB_MASK                  0xFF
#define MPL115A2_PADC_LSB_MASK                  0x03 /* 00000011 */
#define MPL115A2_TADC_MSB_MASK                  0xFF
#define MPL115A2_TADC_LSB_MASK                  0x03 /* 00000011 */
#define MPL115A2_A0MSB_MASK                     0xFF
#define MPL115A2_A0LSB_MASK                     0xFF
#define MPL115A2_B1MSB_MASK                     0xFF
#define MPL115A2_B1LSB_MASK                     0xFF
#define MPL115A2_B2MSB_MASK                     0xFF
#define MPL115A2_B2LSB_MASK                     0xFF
#define MPL115A2_C12MSB_MASK                    0xFF
#define MPL115A2_C12LSB_MASK                    0x3F /* 00111111 */

/* MPL115A2 - Base Pointer */
#define MPL115A2_BASE_PTR                       ((MPL115A2_MemMapPtr)0x00u)

/* MPL115A2 - Register instance definitions */
#define MPL115A2_PADC_MSB                       (MPL115A2_BASE_PTR->PADC_MSB)
#define MPL115A2_PADC_LSB                       (MPL115A2_BASE_PTR->PADC_LSB)
#define MPL115A2_TADC_MSB                       (MPL115A2_BASE_PTR->TADC_MSB)
#define MPL115A2_TADC_LSB                       (MPL115A2_BASE_PTR->TADC_LSB)
#define MPL115A2_A0MSB                          (MPL115A2_BASE_PTR->A0MSB)
#define MPL115A2_A0LSB                          (MPL115A2_BASE_PTR->A0LSB)
#define MPL115A2_B1MSB                          (MPL115A2_BASE_PTR->B1MSB)
#define MPL115A2_B1LSB                          (MPL115A2_BASE_PTR->B1LSB)
#define MPL115A2_B2MSB                          (MPL115A2_BASE_PTR->B2MSB)
#define MPL115A2_B2LSB                          (MPL115A2_BASE_PTR->B2LSB)
#define MPL115A2_C12MSB                         (MPL115A2_BASE_PTR->C12MSB)
#define MPL115A2_C12LSB                         (MPL115A2_BASE_PTR->C12LSB)
#define MPL115A2_CONVERT                        (MPL115A2_BASE_PTR->CONVERT)

uint8_t mpl115a2_init(void);
uint8_t mpl115a2_measure_temp(void);
uint8_t mpl115a2_measure_press(void);

#endif /* MPL115A2_H_ */
