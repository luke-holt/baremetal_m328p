#include <stdlib.h>

#include "event.h"


#define WATCHLIST_MAX	8

#define MASK0			0x01
#define MASK1			0x02
#define MASK2			0x04
#define MASK3			0x08
#define MASK4			0x10
#define MASK5			0x20
#define MASK6			0x40
#define MASK7			0x80


static struct event_group {
	uint8_t *flag_reg;
	uint8_t *cbs[8];
} event_group;


static uint8_t *watchlist[WATCHLIST_MAX]
static uint8_t wl_i;


static uint8_t not_in_regs(uint8_t *reg)
{
	/* empty watchlist */
	if (wl_i == NULL) {
		return 1;
	}

	/* Check existing members */
	for (int i = 0; i < wl_i; i++) {
		if (watchlist[i]->flag_reg == reg) {
			return 0;
		}
	}

	return 1;
}

/**
 * @brief Get group from watchlist[]. If group doesn't exist,
 * create new one and return it.
 * 
 * @param reg
 * @return event_group* 
 */
static event_group *get_grp(uint8_t *reg)
{
	/* empty watchlist */
	if (wl_i == NULL) {
		return NULL;
	}

	for (int i = 0; i < wl_i; i++) {
		event_group *cur = (event_group *)watchlist[i];
		if (cur->flag_reg == reg) {
			return cur;
		}
	}

	event_group *grp = (event_group *)malloc(sizeof(event_group));
	grp->flag_reg = reg;

	/* Add new group to watchlist and increment index */
	watchlist[wl_i] = grp;
	wl_i++;

	return grp;
}


void event_register_cb(uint8_t *reg, uint8_t bit, void (*callback)(void))
{
	if (bit >= 8) {
		/* TODO: ERROR: Can only have 8 flags in one byte */
		return;
	}

	event_group *grp = get_grp(reg);
	grp->cbs[bit] = callback;
}


static inline void do_cb(event_group *grp, uint8_t bit)
{
	if (grp->flag_reg == 0x00) {
		return;
	}

	/* if the bit is set in the flag register, exec the cb */
	if ((*grp->flag_reg & (1 << bit)) == (1 << bit)) {
		grp->cbs[bit]();
		
		/* Clear flag */
		*grp->flag_reg &= ~(1 << bit);
	}
}


void event_loop_start(void)
{
	while (1) {
		event_group *cur;
		for (int i = 0; i < wl_i; i++) {
			cur = (event_group *)watchlist[i];

			/* If true, there is at least one flag set */
			if (*cur->flag_reg != 0) {
				/* Check every bit */
				for (int i = 0; i < 8; i++) {
					do_cb(cur, i);
					/* If 0, no more flags */
					if (*cur->flag_reg == 0) break;
				}
			}
		}
	}
}
