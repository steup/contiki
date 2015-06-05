
#include "basic-shell-time.h"

PROCESS(basic_shell_time_process, "time");

SHELL_COMMAND(	basic_shell_time_command, 
				"time", 
				"time: get the current time", 
				&basic_shell_time_process);

PROCESS_THREAD(basic_shell_time_process, ev, data)
{
	char buffer[80];
	time_t rawtime;
	struct tm *timeinfo;
	
	PROCESS_BEGIN();
		
	rawtime = clock_seconds();	
	timeinfo = localtime(&rawtime);
	
	
	strftime (buffer,80,"%Y-%m-%d %H:%M:%S",timeinfo);
	
	shell_output_str(&basic_shell_time_command, buffer, "");
	
	PROCESS_END();
}

void basic_shell_time_init(void)
{
	shell_register_command(&basic_shell_time_command);
}
