#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"

static void set_int_enable(uint8_t en) {
  if (en == 1) {
    SPCR |= (1 << SPIE);
  } else if (en == 0) {
    SPCR &= ~(1 << SPIE);
  } else {
    /* TODO: ERROR: Add error handling. EN must be 0 or 1. */
  }
}

static inline void set_data_order(spi_dorder_t order) {
  SPCR &= ~(1 << DORD);
  SPCR |= (order << DORD);
}

static inline void enable(void) { SPCR |= (1 << SPE); }

static void set_mode(spi_mode_t mode) {
  SPCR &= ~(1 << MSTR);
  SPCR |= (mode << MSTR);

  /* Clear SPI bits */
  DDRB &= ~(7 << DDB2);

  if (mode == SPI_MODE_CHILD) {
    DDRB |= (1 << DDB4); /* MOSI output */
  } else {
    /* SS, MISO, SCK as output */
    DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
  }
}

static void set_clock_polarity(spi_clock_pol_t cpol) {
  switch (cpol) {
    case SPI_CPOL_0:
      SPCR &= ~(1 << CPOL);
      break;
    case SPI_CPOL_1:
      SPCR |= (1 << CPOL);
      break;
    default:
      /* TODO: ERROR: How did you even manage to do this? */
      break;
  }
}

static void set_clock_phase(spi_clock_pha_t cpha) {
  switch (cpha) {
    case SPI_CPHA_0:
      SPCR &= ~(1 << CPHA);
    case SPI_CPHA_1:
      SPCR |= (1 << CPHA);
      break;
    default:
      /* TODO: ERROR: How did you even manage to do this? */
      break;
  }
}

static void set_clock_rate(spi_clock_rate_t crate) {
  /* Clear bits */
  SPCR &= ~(3 << SPR0);
  SPSR &= ~(1 << SPI2X);

  /* Set bits */
  SPCR |= ((crate & 3) << SPR0);
  SPCR |= ((crate >> 2) << SPI2X);
}

static void write_read(uint8_t tx_byte, uint8_t *rx_byte) {
  SPDR = tx_byte;

  while ((SPSR & (1 << SPIF)) != (1 << SPIF))
    ;

  if (rx_byte == NULL) return;

  *rx_byte = SPDR;
}

static void burst_write(uint8_t *buf, size_t sz) {
  /* Transmit the buffer while ignoring any possible received bits */
  int i = (int)sz;
  while (i--) {
    write_read(*buf, NULL);
    buf++;
  }
}

static void burst_read(uint8_t *buf, size_t sz) {
  /* While the read buffer fills up, transmit 0 */
  int i = (int)sz;
  while (i--) {
    write_read(0, buf);
    buf++;
  }
}

static void burst_write_read(uint8_t *tx_buf, uint8_t *rx_buf, size_t sz) {
  /* TODO: This. */
}

spi_driver_api_t spi_get_inst(void) {
  spi_driver_api_t api = {
      .set_int_enable = set_int_enable,
      .set_data_order = set_data_order,
      .enable = enable,
      .set_mode = set_mode,
      .set_clock_polarity = set_clock_polarity,
      .set_clock_phase = set_clock_phase,
      .set_clock_rate = set_clock_rate,
      .write_read = write_read,
      .burst_write = burst_write,
      .burst_read = burst_read,
      .burst_write_read = burst_write_read,
  };

  return api;
}
