#ifndef BASIC_SHELL_BROADCAST_H_
#define BASIC_SHELL_BROADCAST_H_

#define BROADCAST_CHANNEL 21

#include <string.h>
#include <stdio.h>

#include "contiki.h"
#include "contiki-net.h"
#include "random.h"
#include <shell.h>

static struct broadcast_conn broadcast;
static const struct broadcast_callbacks broadcast_call;

static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from);

void basic_shell_broadcast_init(void);

#endif /* BASIC_SHELL_BROADCAST_H_ */