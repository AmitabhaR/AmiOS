#include "Disk.h"
#include "Display.h"

static struct FAT_File * fat_files;
static int32_t file_count = 0;
static int8_t boot_drive = 0;

static struct FAT_File * addEntry(int32_t newsize)
{
	struct FAT_File * fat_fl = (struct FAT_File *) kmalloc(newsize);
	int32_t cnt = 0;
	
	for(cnt = 0;cnt < file_count;cnt++)
	{
		int32_t c = 0;
		
		for(c = 0;c < 7;c++)
		{
			fat_fl[cnt].file_name[c] = fat_files[cnt].file_name[c];
		}
		
		for(c = 0;c < 3;c++)
		{
			fat_fl[cnt].file_ext[c] = fat_files[cnt].file_ext[c];
		}
		
		fat_fl[cnt].clustor_start = fat_files[cnt].clustor_start;
	}
	
	kfree(fat_files);
	file_count++;

	return fat_fl;
}

RESULT readSector(int16_t _seg,int16_t address,int8_t tot_sectors,int8_t start_sector,int8_t track_numb,int8_t head_numb,int8_t drive_num)
{
	__asm
	{
		push es ;
		mov ax , _seg ;
		mov es , ax ;	
		mov bx , address ;
		mov al , tot_sectors ;	
		mov cl , start_sector ;
		mov ch , track_numb ;
		mov dh , head_numb ;
		mov dl , 0 ;
		mov ah , 0x2 ;
		int 0x13 ;

		pop es ;

		jc on_error ; 
	};

	return TRUE;

on_error:

	return FALSE;
}

RESULT writeSector(int16_t _seg,int16_t address,int8_t tot_sectors,int8_t start_sector,int8_t track_numb,int8_t head_numb,int8_t drive_num)
{
	RESULT res;

	return res;
}

struct FAT_Clustor getClustor(int16_t clustor_num)
{
	struct FAT_Clustor clustor;
	int8_t clus_num = (24 + clustor_num) % 18;

	if (clus_num < 1)
	{
		clus_num = 1;
	}

	if (SUCCES(readSector(0x1000,0x1,1,clus_num,((24 + clustor_num) / 18) - 1,((24 + clustor_num) / 18) % 2,boot_drive)) == TRUE)
	{
		bool isend = TRUE;
		bool isptr = FALSE;
		int16_t cur_adr = 1;
		int16_t data_cntr = 0;

		__asm
		{
			mov ax , 0x1000 ;
			mov es , ax ;
		};

		clustor.clustor_record.cur_num = clustor_num;
		clustor.tot_bytes = 0;

		for(cur_adr = 1;cur_adr <= 512;cur_adr++) 
		{
			if (isend == TRUE)
			{
				bool val = FALSE;

				__asm
				{
					mov bx , cur_adr ;
					mov al , es:[bx] ;
					mov val , al ;
				};

				if (val == TRUE)
				{
					clustor.clustor_record.end = TRUE;
				}
				
				isptr = TRUE;
				isend = FALSE;
			}
			else if (isptr == TRUE)
			{
				int8_t pt = 0;

				__asm
				{
					mov bx , cur_adr ;
					mov al , es:[bx] ;
					mov pt , al ;
				};

				if (pt == 0)
				{
					if (clustor.clustor_record.end == FALSE)
					{
						clustor.clustor_record.free = TRUE;
					}

				}
				else
				{
					clustor.clustor_record.ptr = pt;
				}

				isptr = FALSE;
			}
			else
			{
				int8_t _ch = 0;

				__asm
				{
					mov bx , cur_adr ;
					mov al , es:[bx] ;
					mov _ch , al ;
				};

				
				clustor.data_bytes[data_cntr] = _ch;
				clustor.tot_bytes++;
				data_cntr++;
			}
		}

		if (clustor.tot_bytes < 1)
		{ 
			printf("Clustor not valid!",0);
		}

		__asm
		{
			mov ax , cs ;
			mov es , ax ;
		};
	}

	return clustor;
}

RESULT writeClustor(int16_t clustor_num, const int8_t * data_array)
{
	RESULT res;

	return res;
}

RESULT setClustorRecord(int16_t clustor_num,struct FAT_Clustor_Record clustor_record)
{
	RESULT res;

	return res;
}

RESULT loadTable(int8_t boot_drv)
{
	boot_drive = boot_drv;

	if (SUCCES(readSector(0x1000,0x1,3,4,0,1,boot_drv)) == TRUE)
	{
		bool isName = TRUE;
		bool isExt = FALSE;
		bool startClustor = FALSE;
		char file_nm[7];
		char file_ext[3];
		int8_t start_clustor = 0;
		int8_t cnt = 0;
		int16_t cur_adr = 1;
		
		int16_t sg = 0x1000;

		__asm
		{
			mov ax , sg ;
			mov es , ax ;
		};

		while(TRUE)
		{
			if (isName == TRUE)
			{
				if (cnt < 7)
				{
					char _ch = 0;

					__asm
					{
						mov bx , cur_adr ;
						mov al , es:[bx] ;
						mov _ch , al ;
					}

					if (_ch == 0)
					{
						fat_files = addEntry(sizeof(struct FAT_File) * (file_count + 1));			
					
						for(cnt = 0;cnt < 7;cnt++)
						{
							fat_files[file_count].file_name[cnt] = file_nm[cnt];	
						}	 
					
						fat_files[file_count].file_ext[0] = 'N';
						fat_files[file_count].file_ext[1] = 'U';
						fat_files[file_count].file_ext[2] = 'L'; 
					
						fat_files[file_count].clustor_start = 0;
						file_count++;
						
						break;
					}
					else
					{
						file_nm[cnt] = _ch;
						cnt++;
					}

					if (cnt > 6)
					{
						isName = FALSE;
						isExt = TRUE;
						cnt = 0;
					}
				}
				else
				{
					isName = FALSE;
					isExt = TRUE;
					cnt = 0;
				}
			}
			else if (isExt == TRUE)
			{
				if (cnt < 3)
				{
					char _ch = 0;
					
					__asm
					{
						mov bx , cur_adr ;
						mov al , es:[bx] ;
						mov _ch , al ;
					}

					if (_ch == 0)
					{
						fat_files = addEntry(sizeof(struct FAT_File) * (file_count + 1));			
					
						for(cnt = 0;cnt < 7;cnt++)
						{
							fat_files[file_count].file_name[cnt] = file_nm[cnt];	
						}	 
					
						for(cnt = 0;cnt < 3;cnt++)
						{
							fat_files[file_count].file_ext[cnt] = file_ext[cnt];
						}
					
						fat_files[file_count].clustor_start = 0;
						file_count++;
						
						break;
					}
					else
					{
						file_ext[cnt] = _ch;
						cnt++;
					}

					if (cnt > 2)
					{
						isExt = FALSE;
						startClustor = TRUE;
						cnt = 0;
					}
				}
				else
				{
					isExt = FALSE;
					startClustor = TRUE;
					cnt = 0;
				}	
			}
			else if (startClustor == TRUE)
			{
				char _ch = 0;

				__asm
					{
						mov bx , cur_adr
						mov al , es:[bx] ;
						mov _ch , al ;
					}

				if (_ch == 0)
				{
					int8_t cnt = 0;
							
					start_clustor = _ch;
					fat_files = addEntry(sizeof(struct FAT_File) * (file_count + 1));			
					
					for(cnt = 0;cnt < 7;cnt++)
					{
						fat_files[file_count].file_name[cnt] = file_nm[cnt];	
					} 
					
					for(cnt = 0;cnt < 3;cnt++)
					{
						fat_files[file_count].file_ext[cnt] = file_ext[cnt];
					}
					
					fat_files[file_count].clustor_start = 0;
					file_count++;
					isName = TRUE;
					startClustor = FALSE;
					
					break;
				}
				else
				{		
					int8_t cnt = 0;
							
					start_clustor = _ch;
					fat_files = addEntry(sizeof(struct FAT_File) * (file_count + 1));			
					
					for(cnt = 0;cnt < 7;cnt++)
					{
						fat_files[file_count].file_name[cnt] = file_nm[cnt];	
					} 
					
					for(cnt = 0;cnt < 3;cnt++)
					{
						fat_files[file_count].file_ext[cnt] = file_ext[cnt];
					}
					
					fat_files[file_count].clustor_start = start_clustor;
					file_count++;
					isName = TRUE;
					startClustor = FALSE;
				}
			}
			
			cur_adr++;
		}

		__asm
		{
			mov ax , cs;
			mov es , ax ;
		};

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

RESULT saveTable()
{
	RESULT res;

	return res;
}

struct FAT_File getFile(String file_nm)
{
	struct FAT_File fat_f;

	int32_t cnt = 0;

	for(cnt = 0;cnt < file_count;cnt++)
	{
		if (strlen(file_nm) > 6)
		{
			int32_t c = 0;
			bool succes = TRUE;

			for(c = 0;c < 7;c++)
			{
				if (file_nm[c] != fat_files[cnt].file_name[c])
				{
					succes = FALSE;
					break;
				}
			}

			if (succes == TRUE)
			{
				fat_f = fat_files[cnt];
				break;
			}
		}
	}

	return fat_f;
}

byte readByte(String file_nm,int32_t byte_pos)
{
	struct FAT_File cur_file = getFile(file_nm);

	if (cur_file.file_name[0] != 0)
	{
		int32_t adder = 0;
		int32_t byte_cnt = 0;
		struct FAT_Clustor clus = getClustor(cur_file.clustor_start);

re:
		for(byte_cnt = 0;byte_cnt < 510;byte_cnt++)
		{
			if (byte_cnt + adder == (byte_pos - 1))
			{
				return clus.data_bytes[byte_cnt];
			}
		}

		byte_cnt = 0;
		adder += 510;

		if (clus.clustor_record.end == FALSE)
		{
			clus = getClustor(clus.clustor_record.ptr);
			goto re;
		}
	}

	return 0;
}

RESULT createFile(struct FAT_File file_handle)
{
	fat_files = addEntry(sizeof(struct FAT_File) * (file_count + 1));

	fat_files[file_count] = file_handle;

	file_count++;

	return TRUE;
}

RESULT deleteFile(String file_nm)
{
	int32_t cnt = 0;
	int32_t f_cnt = -1;

	for(cnt = 0;cnt < file_count;cnt++)
	{
		int32_t c = 0;
		bool succes = TRUE;

		for(c = 0;c < 7;c++)
		{
			if (fat_files[cnt].file_name[c] != file_nm[c])
			{
				succes = FALSE;
				break;
			}
		}

		if (succes == TRUE)
		{
			f_cnt = cnt;
			break;
		}
	}
	
	if (f_cnt == -1)
	{
		return FALSE;
	}
	else
	{
		int32_t i = 0;

		for(cnt = 0;cnt < file_count - 1;cnt++)
		{
			if (cnt == f_cnt)
			{
				fat_files[cnt] = fat_files[cnt + 1];
				i = 1;
			}
			else
			{
				fat_files[cnt] = fat_files[cnt + i];
			}
		}

		file_count--;
	}

	return TRUE;
}