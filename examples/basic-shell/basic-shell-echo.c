
#include "basic-shell-echo.h"

PROCESS(basic_shell_echo_process, "echo");

SHELL_COMMAND(	basic_shell_echo_command,
				"echo",
				"echo <text>: print <text>",
				&basic_shell_echo_process);
		  
PROCESS_THREAD(basic_shell_echo_process, ev, data)
{
	PROCESS_BEGIN();
	
	if ( data != NULL )
	{
		char *buffer, *ptr;
		char *delimeter;
		int size;
		
		buffer = (char*)data;
		size = strlen(data);
	
		if ( size == 0 )
		{
			PROCESS_EXIT();
		}
		
		delimeter = " ";
		
		ptr = strtok(buffer, delimeter);
		
		while (ptr != NULL)
		{
			shell_output_str(&basic_shell_echo_command, ptr, "");
			
			ptr = strtok(NULL, delimeter);
		}
	}
	
	PROCESS_END();
}

void basic_shell_echo_init(void)
{
	shell_register_command(&basic_shell_echo_command);
}
