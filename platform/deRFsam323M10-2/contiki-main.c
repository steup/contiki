#include "contiki-main.h"

int main()
{
	dbg_setup_uart_default();
	
	printf("Initialising\n");

	clock_init();
	process_init();
	
	process_start(&etimer_process, NULL);
	
	autostart_start(autostart_processes);
	
	printf("Processes running\n");
	
	while (1)
	{
		while (process_run() > 0);
		
		++idle_count;
		
		/* Idle! */
		/* Stop processor clock */
		/* asm("wfi"::); */ 
	}
	
	return 0;
}
