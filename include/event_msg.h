#include <stdint.h>


typedef struct event_msg_t {

	uint8_t grpno;
	uint8_t eventno;
	void *ctx;

} event_msg_t;


typedef void (*event_handler_t)(event_msg_t *msg);


/**
 * @brief Register an event handler in exchange for the group number.
 * When sending adding an event to the queue, this group number needs to
 * be included to specify which handler to call.
 * 
 * @return int Group number
 */
int event_register_group(event_handler_t handler);



/**
 * @brief Function to process message from queue.
 * Use this function to manually process messages.
 * The event loop calls this function whenever there are messages in queue.
 * 
 */
void process_msg(void);



/**
 * @brief Begin the event loop.
 * 
 */
void event_begin_loop(void);


/**
 * @brief Add message to queue
 * 
 * @param msg Pointer to message structure
 * @return int errno
 */
int event_enqueue(event_msg_t *msg);
/* TODO: EVENT: Maybe add priority system? */
