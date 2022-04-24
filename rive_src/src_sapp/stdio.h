///[Redirect fprintf errors]///
#ifdef D_Platform_Web_Emsc
#include <../libc/stdio.h>
#else
#include <../include/stdio.h>
#endif

#ifdef __cplusplus
#define fn extern "C" 	 
#else
#define fn 	  
#endif
fn int _printf(const char *_format, ...);
//FILE *stream
#define DbgIn_ImGUI
#ifdef DbgIn_ImGUI
	#define _CR_ ""
#else
	#define _CR_ "\n"
#endif

#define fprintf(FileStream, ...)  _printf(_CR_"E[Rive]> " __VA_ARGS__);
///////////////////////////////////