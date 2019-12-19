//#define DEBUG
#ifdef DEBUG
#define debug(__va_arg)		printf(__va_arg)
#else
#define debug(__va_arg)
#endif
