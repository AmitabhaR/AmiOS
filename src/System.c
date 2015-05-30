#include "System.h"
#include "Display.h"

void sys_delay()
{
	int64_t cnt = 0;
	
	for(cnt = 0;cnt < 10000000;cnt++); // Delayed Loop.
}

void sys_shutdown()
{
	//if (SUCCES(sys_checkAPM( )) == TRUE)
//	{

	//}
	//else
//	{
		printf("Shut down requires direct power off . You data is saved properly.",0);

		__asm
		{
			cli ;
			hlt ;
		}
	//}
}

RESULT sys_checkAPM( )
{
	__asm
	{ 
		mov ah , 0x53 ;
		mov al , 0 ;
		xor bx , bx ;

		int 0x15 ;

		jc on_error;

	};

	return TRUE;

on_error:

	return FALSE;
}

RESULT sys_ready_printer()
{
	RESULT res;

	return res;
}

RESULT sys_write_printer(String str)
{
	RESULT res;

	return res;
}

PrinterStatus sys_getstatus_printer()
{
	PrinterStatus pstat;

	return pstat;
}