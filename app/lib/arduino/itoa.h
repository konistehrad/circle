#ifndef itoa_h
#define itoa_h


#ifdef __cplusplus
extern "C"{
#endif

int toupper (int c);
int tolower(int c);
int isspace(int c);
char *strrchr(const char *cp, int ch);
char *strrev(char *str);
char* itoa(int num, char* str, int base);
char* ltoa(long num, char* str, int base);
char* ultoa(unsigned long num, char* str, int base);


#ifdef __cplusplus
} // extern "C"
#endif


#endif
