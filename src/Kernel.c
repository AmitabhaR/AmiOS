#include "Display.h"
#include "Disk.h"
#include "Memory.h"
#include "System.h"
#include "Process.h"

void kmain()
{
	int8_t boot_drive = 0;
	struct Process newProcess;

	__asm
	{
		mov boot_drive , dl ;
	};

	kmem_init( );
	
	clearScreen(TEXT_MODE); // Kernel Text MODE.

	printf(" $$$$$$$$$ Welcome to AmiOS Kernel $$$$$$$$$ ",0);

	sys_delay( );

	printf("    $$$$$$$$$ Version : 1.0.1 $$$$$$$$$ ", 0);

	sys_delay( );

	printf("",0);

	printf("PC Configure", 0);

	printf("============",0 );

	printf("File System Check[...]", 0);

	sys_delay( );

	if (SUCCES(loadTable(boot_drive)) == TRUE)
	{
		printf("File System Ok                [T]", 0);
	}
	else
	{
		printf("File System Failure           [F]", 0);
	}

	printf("APM Services Check            [...]", 0);

	sys_delay( );

	if (SUCCES(sys_checkAPM( )) == TRUE)
	{
		printf("APM Services Ok               [T]", 0);
	}
	else
	{
		printf("APM Services Failed           [F]", 0);
	}

	clearScreen(TEXT_MODE);

	// Load our first process .

	newProcess = process_create("Shellii");

	if (newProcess.isReady == TRUE)
	{
		if (process_start(newProcess) == FALSE)
		{
			printf("Cannot start Shell!", 0);
		}
	}
	else
	{
		printf("Cannot find Shell!", 0);
	}

	sys_shutdown();
}