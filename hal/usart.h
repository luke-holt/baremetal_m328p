#include <stdint.h>


typedef void (*usart_set_baudrate_t)(int baud_rate);
typedef void (*usart_frame_cfg_t)(uint8_t n_data, uint8_t n_parity, uint8_t n_stop);
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
