[bits 16]
 
org 0x0

cli 

 mov ax , cs ;
 mov ds , ax ;
 mov es , ax ;

sti

 mov bx , data ;
 call print_string ;

 mov bx , ver ;
 call print_string ;
 
 retf ; 

print_string:
    pusha;
	    mov ah, 0x0e ;
	    pnt:
	     mov al , [bx] ;
		 cmp al , 0 ;
		 je done ;
		 int 0x10 ;
		 add bx , 1;
		 jmp pnt ;
	done:
          mov al , 10 ;
	  int 0x10 ;
	  mov al , 13 ;
	  int 0x10 ;
    popa;

ret ;

data: 
	db 'Welcome to AmiOS.', 0 ;

ver:
	db 'Version : 1.0.0' , 0 ;