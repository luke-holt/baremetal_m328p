#include "event_msg.h"


#define EVENT_MAX_GRPS		32
#define EVENT_MAX_MSGS		128


/* List of registered event handlers */
static event_handler_t handlers[EVENT_MAX_GRPS];
static uint8_t nhandlers;

/* Circular message buffer */
static event_msg_t *messages[EVENT_MAX_MSGS];
static uint8_t nmsgs;
static uint8_t front;
static uint8_t rear;


int event_register_group(event_handler_t handler)
{
	if (nhandlers >= EVENT_MAX_GRPS) {
		/* TODO: ERROR: Handler list full */
		return -1;
	}

	/* Add the handler to the list */
	handlers[nhandlers] = handler;

	/* Inc */
	nhandlers++;

	/* The index of the handler in the list is the group no */
	return nhandlers - 1;
}


void process_msg(void)
{
	if (nmsgs == 0) {
		/* TODO: ERROR: Queue empty */
		return;
	}

	event_msg_t *msg = messages[front];

	/* Increment front. Set to 0 if it looped around */
	front++;
	if (front == EVENT_MAX_MSGS) {
		front = 0;
	}

	/* Just processed a message, so dec */
	nmsgs--;

	/* Call associated handler */
	handlers[msg->grpno](msg);
}


void event_begin_loop(void)
{
	while (1) {
		process_msg();
	}
}


int event_enqueue(event_msg_t *msg)
{
	if (nmsgs >= EVENT_MAX_MSGS) {
		/* TODO: ERROR: Buffer full */
		return -1;
	}

	if (rear == EVENT_MAX_MSGS) {
		/* Circle to front */
		rear = 0;
	}

	messages[rear] = msg;
	rear++;
	nmsgs++;

	return 0;
}
