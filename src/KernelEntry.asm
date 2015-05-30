; Boot Loader Entry .

.286 ; CPU (x86)
.model TINY , C; Tiny model.

extrn kmain:near ; Symbol for boot main.
public process_start ;
public syscall_startprocess ;
extrn  process_handler:near ;

.code
org 1000h       ; Load address of kernel.

main:
		jmp short start ;
		nop ;

start:		
		cli ; Disable Interrupts.
			mov ax , cs ;
			mov ds , ax ;
			mov es , ax ;
			mov ss , ax ;
		sti ; Enable Interrupts.
					
		jmp kmain ; Call Boot Main.

	    ret ;

process_start:
		pop ax ;

		mov cx , ax ; Store IP for return.

		call process_handler ;

		mov ax , cx ;

		push ax ;

		ret ;

syscall_startprocess:
   pop ax ;
   
   mov cx , ax ;
   
   pop ax ;
   
   mov dx , ax ; 

   call process_handler ;

   push dx ;
   push cx ;

   retf ;

END main ; End of main.

END

