#ifndef STRING_H
#define STRING_H

#include "Types.h"
#include "Memory.h"

#define EXP_STRING(exp) # exp

typedef char far* String;

int16_t strlen(const String str);
bool strcmp(const String str1, const String str2);
String strcpy(const String dest, const String src);
String str_substr(const String src, int16_t start_pos, int16_t end_pos);
int16_t str_getchar_pos(const String src, char ch);
bool str_setchar(const String src, int16_t pos, char ch);

#endif // STRING_H
