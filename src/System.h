#ifndef SYSTEM_H
#define SYSTEM_H

#include "Types.h"
#include "String.h"

typedef bool PrinterStatus;


void sys_delay();
void sys_shutdown();
RESULT sys_checkAPM();
RESULT sys_ready_printer();
RESULT sys_write_printer(String str);
PrinterStatus sys_getstatus_printer();

#endif // SYSTEM_H
