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
  SPI_CRATE_4,     /* Clock frequency: f_osc / 4 */
  SPI_CRATE_16,    /* Clock frequency: f_osc / 16 */
  SPI_CRATE_64,    /* Clock frequency: f_osc / 64 */
  SPI_CRATE_128,   /* Clock frequency: f_osc / 128 */
  SPI_CRATE_DS_4,  /* Double speed. Clock frequency: f_osc / 4 */
  SPI_CRATE_DS_8,  /* Double speed. Clock frequency: f_osc / 8 */
  SPI_CRATE_DS_32, /* Double speed. Clock frequency: f_osc / 32 */
  SPI_CRATE_DS_64, /* Double speed. Clock frequency: f_osc / 64 */
} spi_clock_rate_t;

typedef void (*spi_set_int_en_t)(uint8_t en);
typedef void (*spi_set_dorder_t)(spi_dorder_t order);
typedef void (*spi_enable_t)(void);
typedef void (*spi_set_mode_t)(spi_mode_t mode);
typedef void (*spi_set_cpol_t)(spi_clock_pol_t cpol);
typedef void (*spi_set_cpha_t)(spi_clock_pha_t cpha);
typedef void (*spi_set_clk_rate_t)(spi_clock_rate_t crate);
typedef void (*spi_write_read_t)(uint8_t tx_byte, uint8_t *rx_byte);
typedef void (*spi_burst_write)(uint8_t *buf, size_t sz);
typedef void (*spi_burst_read)(uint8_t *buf, size_t sz);
typedef void (*spi_burst_write_read)(uint8_t *tx_buf, uint8_t *rx_buf,
                                     size_t sz);

/* TODO: DOC: Add documentation to the api function calls */
typedef struct spi_driver_api_t {
  spi_set_int_en_t set_int_enable;
  spi_set_dorder_t set_data_order;
  spi_enable_t enable;
  spi_set_mode_t set_mode;
  spi_set_cpol_t set_clock_polarity;
  spi_set_cpha_t set_clock_phase;
  spi_set_clk_rate_t set_clock_rate;

  spi_write_read_t write_read;
  spi_burst_write burst_write;
  spi_burst_read burst_read;
  spi_burst_write_read burst_write_read;

} spi_driver_api_t;

spi_driver_api_t spi_get_inst(void);

#endif
