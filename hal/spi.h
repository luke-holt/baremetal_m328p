#ifndef SPI_H__
#define SPI_H__

#include <stdint.h>
#include <stdlib.h>

/* Pinout
 * B2, B3, B4, B5.
 * SS, MOSI, MISO, SCK,
 * respectively.
 */

typedef enum spi_dorder_t {
  SPI_DORDER_MSB, /* MSB is transmitted first */
  SPI_DORDER_LSB, /* LSB is transmitted first */
} spi_dorder_t;

typedef enum spi_mode_t {
  SPI_MODE_CHILD,  /* Set device mode to child */
  SPI_MODE_PARENT, /* Set device mode to parent */
} spi_mode_t;

typedef enum spi_clock_pol_t {
  SPI_CPOL_0, /* Leading edge: Rising. Trailing edge: Falling. */
  SPI_CPOL_1, /* Leading edge: Falling. Trailing edge: Rising. */
} spi_clock_pol_t;

typedef enum spi_clock_pha_t {
  SPI_CPHA_0, /* Leading edge: Sample. Trailing edge: Setup */
  SPI_CPHA_1, /* Leading edge: Setup. Trailing edge: Sample */
} spi_clock_pha_t;

typedef enum spi_clock_rate_t {
  SPI_CRATE_4M,      /* Clock frequency: f_osc / 4 = 4MHz */
  SPI_CRATE_1M,      /* Clock frequency: f_osc / 16 = 1MHz */
  SPI_CRATE_250K,    /* Clock frequency: f_osc / 64 = 250kHz */
  SPI_CRATE_125K,    /* Clock frequency: f_osc / 128 = 125kHz */
  SPI_CRATE_DS_8M,   /* Double speed. Clock frequency: f_osc / 4 = 8MHz */
  SPI_CRATE_DS_2M,   /* Double speed. Clock frequency: f_osc / 8 = 2MHz */
  SPI_CRATE_DS_500K, /* Double speed. Clock frequency: f_osc / 32 = 500kHz */
  SPI_CRATE_DS_250K, /* Double speed. Clock frequency: f_osc / 64 = 250kHz */
} spi_clock_rate_t;

typedef enum spi_int_enable_t {
  SPI_INT_DISABLE,
  SPI_INT_ENABLE,
} spi_int_enable_t;

typedef struct spi_config_t {
  spi_dorder_t data_order;
  spi_mode_t mode;
  spi_clock_pol_t cpol;
  spi_clock_pha_t cpha;
  spi_clock_rate_t crate;
  spi_int_enable_t int_enable;
} spi_config_t;

typedef void (*spi_set_cfg_t)(spi_config_t *cfg);
typedef void (*spi_enable_t)(void);

typedef void (*spi_write_read_t)(uint8_t tx_byte, uint8_t *rx_byte);
typedef void (*spi_burst_write)(uint8_t *buf, size_t sz);
typedef void (*spi_burst_read)(uint8_t *buf, size_t sz);
typedef void (*spi_burst_write_read)(uint8_t *tx_buf, uint8_t *rx_buf,
                                     size_t sz);

/* TODO: DOC: Add documentation to the api function calls */
typedef struct spi_driver_api_t {
  spi_set_cfg_t set_config;
  spi_enable_t enable;
  spi_write_read_t write_read;
  spi_burst_write burst_write;
  spi_burst_read burst_read;
  spi_burst_write_read burst_write_read;
} spi_driver_api_t;

spi_driver_api_t spi_get_inst(void);

#endif
