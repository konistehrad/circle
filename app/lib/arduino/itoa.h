#ifndef itoa_h
#define itoa_h


#ifdef __cplusplus
extern "C"{
#endif


char* itoa(int num, char* str, int base);
char* ltoa(long num, char* str, int base);
char* ultoa(unsigned long num, char* str, int base);


#ifdef __cplusplus
} // extern "C"
#endif


#endif
