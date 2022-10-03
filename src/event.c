#include <stdlib.h>
#include <stdint.h>

#include "event.h"


#define WATCHLIST_MAX	8


typedef struct event_group_t {
	uint8_t *flag_reg;
	event_handler_t handler;
} event_group_t;


static event_group_t *watchlist[WATCHLIST_MAX];
static uint8_t wl_i = 0;


static uint8_t not_in_regs(uint8_t *reg)
{
	/* empty watchlist */
	if (wl_i == 0) {
		return 1;
	}

	/* Check existing members */
	for (int i = 0; i < wl_i; i++) {
		event_group_t *cur = watchlist[i];
		if (cur->flag_reg == reg) {
			return 0;
		}
	}

	/* Not in list */
	return 1;
}


static void new_grp(uint8_t *reg, event_handler_t handler)
{
	event_group_t *grp = malloc(sizeof(event_group_t));
	grp->flag_reg = reg;
	grp->handler = handler;

	/* Add new group to watchlist and increment index */
	watchlist[wl_i] = grp;
	wl_i++;
}


void event_handler_register(uint8_t *flag_byte, event_handler_t handler)
{
	if (not_in_regs(flag_byte)) {
		new_grp(flag_byte, handler);
	} else {
		/* TODO: ERROR: callback already registered for this flag byte. */
	}
}


static void exec_cbs(event_group_t *grp)
{
	/* Check each bit */
	for (int bit = 0; bit < 8; bit++) {

		uint8_t mask = (1 << bit);

		/* Check if flag is set */
		if ((*(grp->flag_reg) & mask) == mask) {

			/* Exec handler */
			grp->handler(mask);

			/* Clear the flag */
			*(grp->flag_reg) &= ~mask;

			/* No more flags in reg? */
			if (*(grp->flag_reg) == 0) {
				break;
			}
		}
	}
}


void event_loop_start(void)
{
	while (1) {
		event_group_t *cur;
		for (int i = 0; i < wl_i; i++) {
			cur = watchlist[i];

			/* If true, there is at least one flag set */
			if (*(cur->flag_reg) > 0) {
				exec_cbs(cur);
			}
		}
	}
}
