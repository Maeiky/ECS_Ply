///[Redirect fprintf errors]///
#ifdef D_Platform_Web_Emsc
#include <../libc/stdio.h>
#else
#include <../include/stdio.h>
#endif

#ifndef fn
	#ifdef __cplusplus
	#define fn extern "C" 	 
	#else
	#define fn 	  
	#endif
	fn int _printf(const char *_format, ...);
#endif

#ifndef _CR_
	#ifdef DbgIn_ImGUI
		#define _CR_ ""
	#else
		#define _CR_ "\n"
	#endif
#endif

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

#ifdef D_Debug
#define fprintf(FileStream, ...)  _printf(_CR_"E[Rive]" "(" __BASE_FILE__ ":" LINE_STRING  ")" "> " __VA_ARGS__);
#else
#define fprintf(FileStream, ...)  _printf(_CR_"E[Rive]> " __VA_ARGS__);
#endif

///////////////////////////////////