[bits 16]
[org 0x7c00]
		jmp short _start 
		nop
; FAT 12 Labels.
OEMLabel db "AmiBOOT"	; Disk label
BytesPerSector dw 512	; Bytes per sector
SectorsPerCluster db 1	; Sectors per cluster
ReservedForBoot dw 1	; Reserved sectors for boot record
NumberOfFats db 2	; Number of copies of the FAT
RootDirEntries dw 224	; Number of entries in root dir
; (224 * 32 = 7168 = 14 sectors to read)
LogicalSectors dw 2880	; Number of logical sectors
MediumByte db 0F0h	; Medium descriptor byte
SectorsPerFat dw 9	; Sectors per FAT
SectorsPerTrack dw 18	; Sectors per track (36/cylinder)
Sides dw 2	; Number of sides/heads
HiddenSectors dd 0	; Number of hidden sectors
LargeSectors dd 0	; Number of LBA sectors
DriveNo dw 0	; Drive No: 0
Signature db 41	; Drive signature: 41 for floppy
VolumeID dd 00000000h	; Volume ID: any number
VolumeLabel db "AmiOS "; Volume Label: any 11 chars
FileSystem db "FAT12 "	; File system type: don't change!		
		
_start:
	
	cli ; Disable Interrupts.
		mov ax , cs 
		mov ds , ax
		mov es , ax
		mov ss , ax 
	sti ; Enable Interrupts.
	
	mov [boot_drive] , dl ;
		
	mov bp , 9000h ;
	mov sp , bp ;
	
	mov bx , 0x1000
	mov ah , 2
	mov al , 20
	mov dh , 0
	mov dl , [boot_drive]
	mov ch , 0
	mov cl , 2
	
	int 0x13
	
	jc error

	mov dl , [boot_drive] ;
	jmp 0000:0x1000
	
error:

	jmp $ ;
	
boot_drive:
		db 0
		
times 510 - ($ - $$) db 0
dw 0xaa55 ; Boot Sector Signature.
