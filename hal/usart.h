#ifndef USART_H__
#define USART_H__

#include <stdint.h>

typedef enum usart_ndata_bits_t {
  USART_NDATA_5,
  USART_NDATA_6,
  USART_NDATA_7,
  USART_NDATA_8,
} usart_ndata_bits_t;

typedef enum usart_parity_mode_t {
  USART_PARITY_DISABLED,
  USART_PARITY_EVEN = 2,
  USART_PARITY_ODD,
} usart_parity_mode_t;

typedef enum usart_nstop_bits_t {
  USART_NSTOP_1,
  USART_NSTOP_2,
} usart_nstop_bits_t;

typedef void (*usart_set_baudrate_t)(int baud_rate);
typedef void (*usart_frame_cfg_t)(usart_ndata_bits_t ndata,
                                  usart_parity_mode_t parity,
                                  usart_nstop_bits_t nstop);
typedef void (*usart_set_int_en_t)(uint8_t en);
typedef void (*usart_enable_t)(void);
typedef void (*usart_tx_byte_t)(uint8_t byte);
typedef void (*usart_rx_byte_t)(uint8_t *byte);

/* TODO: DOCS: Write documentation for api fns */
typedef struct usart_driver_api_t {
  usart_set_baudrate_t set_baudrate;
  usart_frame_cfg_t set_frame_cfg;
  usart_set_int_en_t set_int_enable;
  usart_enable_t enable;
  usart_tx_byte_t tx_byte;
  usart_rx_byte_t rx_byte;

} usart_driver_api_t;

usart_driver_api_t usart_get_inst(void);

#endif
