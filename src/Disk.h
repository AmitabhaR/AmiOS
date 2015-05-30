#ifndef DISK_H
#define DISK_H

/* FATX File System */

#include "Types.h"
#include "String.h"
#include "Memory.h"

struct FAT_File
{
	char file_name[7];
	char file_ext[3];
	int8_t clustor_start;
};

struct FAT_Clustor_Record
{
	int8_t cur_num; // Stores the current clustor number.
	int8_t ptr;	// Pointer to the next clustor for data.
	bool free;      // To see if the clustor is free.
	bool end;      // For check the last clustor of file or not.
};

struct FAT_Clustor
{
	struct FAT_Clustor_Record clustor_record; // Clustor ID Record.
	int8_t data_bytes[510];
	int32_t tot_bytes;
};

RESULT readSector(int16_t _seg,int16_t address,int8_t tot_sectors,int8_t start_sector,int8_t track_numb,int8_t head_numb,int8_t drive_num); 
RESULT writeSector(int16_t _seg,int16_t address,int8_t tot_sectors,int8_t start_sector,int8_t track_numb,int8_t head_numb,int8_t drive_num);
struct FAT_Clustor getClustor(int16_t clustor_num);
RESULT writeClustor(int16_t clustor_num, const int8_t * data_array);
RESULT setClustorRecord(int16_t clustor_num,struct FAT_Clustor_Record clustor_record);
RESULT loadTable(int8_t boot_drv);
RESULT saveTable();
struct FAT_File getFile(String file_nm);
byte readByte(String file_nm,int32_t byte_pos);
RESULT createFile(struct FAT_File file_handle);
RESULT deleteFile(String file_nm);


#endif // DISK_H
