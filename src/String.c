#include "String.h"

int16_t strlen(const String str)
{
	int16_t len = 0;

	while(str[len])
	{
		len++;
	}

	return len;
}

bool strcmp(const String str1,const String str2)
{ 
	int16_t len_f = strlen(str1);
	int16_t len_s = strlen(str2);
	int16_t cnt = 0;

	if (len_f < len_s || len_f > len_s)
	{
		return FALSE;
	}

	for(cnt = 0;cnt < len_f;cnt++)
	{
		if (str1[cnt] != str2[cnt])
		{
			return FALSE;
		}
	}

	return TRUE;
}

String strcpy(const String dest,const String src)
{
	String ret_str = (String) kmalloc(sizeof(char) * (strlen(dest) + strlen(src) + 1));
	int16_t cnt = 0;
	int16_t cntr = 0;

	for(cnt = 0;cnt < strlen(dest);cnt++)
	{
		ret_str[cnt] = dest[cnt];
	}
	
	for(cntr = 0;cntr < strlen(src);cnt++,cntr++)
	{ 
		ret_str[cnt] = src[cntr];
	}
	
	ret_str[cnt] = 0;

	return ret_str;
}

String str_substr(const String src,int16_t start_pos,int16_t end_pos)
{
	if (start_pos > 0 && start_pos < strlen(src) && end_pos > 0 && end_pos < strlen(src) && start_pos <= end_pos)
	{ 
		String ret_str = (String) kmalloc(sizeof(char) * (end_pos - start_pos));
		int16_t cnt = 0;
		int16_t cntr = 0;
		
		for(cnt = start_pos;cnt <= end_pos;cnt++,cntr++)
		{ 
		   ret_str[cntr] = src[cnt];
		}
		
		return ret_str;
	}
	
	return 0;
}

int16_t str_getchar_pos(const String src,char ch)
{
	int16_t len = strlen(src);
	int16_t cnt = 0;
	
	for(cnt = 0;cnt < len;cnt++)
	{ 
	   if (src[cnt] == ch)
	   { 
		return (cnt + 1);
	   }
	}
	
	return -1;
}

bool str_setchar(String src,int16_t pos,char ch)
{
	if (pos < strlen(src) && pos > 0)
	{ 
	     src[pos - 1] = ch;
	     
	     return TRUE;
	}
	
	return FALSE;
}
