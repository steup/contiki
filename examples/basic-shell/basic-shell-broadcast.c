#include "basic-shell-broadcast.h"

/*---------------------------------------------------------------------------*/
PROCESS(basic_shell_broadcast_process, "broadcast");

SHELL_COMMAND(	basic_shell_broadcast_command,
				"broadcast",
				"broadcast <message>: send a broadcast messages",
				&basic_shell_broadcast_process);

static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
	char buffer[255];
	
	memset(buffer, '\0', 255);
	memcpy(buffer, (char *)packetbuf_dataptr(), packetbuf_datalen());
	
	printf("broadcast message received from %d.%d: '%s' \n", from->u8[0], from->u8[1], buffer);
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};


PROCESS_THREAD(basic_shell_broadcast_process, ev, data)
{
	char broadcast_message[255];
	int broadcast_message_size = 0;

	PROCESS_BEGIN();
	
	if ( data != NULL )
	{
		broadcast_message_size = strlen(data);
		strncpy(broadcast_message, (char*)data, broadcast_message_size);
	}
	
	if ( broadcast_message_size == 0 )
	{
		shell_output(&basic_shell_broadcast_command, "no broadcast message", 20, "", 0);
		
		PROCESS_EXIT();
	}
	else
	{	
		packetbuf_copyfrom(broadcast_message, broadcast_message_size);
		broadcast_send(&broadcast);
	}
	
	PROCESS_END();
}

void basic_shell_broadcast_init(void)
{
	shell_register_command(&basic_shell_broadcast_command);
	
	broadcast_open(&broadcast, BROADCAST_CHANNEL, &broadcast_call);
}