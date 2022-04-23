/*  -== Xternal ==-
 *
 *  Utility and functions that rely on external libs for common usage
 *
 *  @autors
 *   - Maeiky
 *  
 * Copyright (c) 2021 - V·Liance
 *
 * The contents of this file are subject to the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * If a copy of the Apache License Version 2.0 was not distributed with this file,
 * You can obtain one at https://www.apache.org/licenses/LICENSE-2.0.html
 *
*/
#ifndef HDef_xIO
#define HDef_xIO

#include "Xternal/Types.h"

#define DbgIn_ImGUI
#ifdef DbgIn_ImGUI
	#define NO_ERR_PRINTF
	typedef struct ExampleAppConsole ExampleAppConsole;
	extern ExampleAppConsole*  console_main;
	fn void AddToConsole(ExampleAppConsole* cls, const char* format, ...);
	#define _CR_ ""
#else
	#define _CR_ "\n"
#endif

	fn void _Sleep(uint32_t _nMillisecond);
	
	fn void _exit_FAILURE();
	fn void _exit_SUCCESS();
	
	#ifdef D_Debug
		#define dbg_printf _printf
	#else
		#define dbg_printf 
	#endif
	
	fn int _printf(const char *_format, ...);
	
	#ifdef NO_ERR_PRINTF
		#define err_print(...) _printf(_CR_"E: " __VA_ARGS__);
	#else
		fn int err_printf(const char *_format, ...);
		#define err_print(...) err_printf(_CR_"E: " __VA_ARGS__);
	#endif
	
	#define _print(...)  		_printf(_CR_ __VA_ARGS__);
	#define dbg_print(...)  	dbg_printf(_CR_ __VA_ARGS__);
	#define warn_print(...)    _printf(_CR_"W: " __VA_ARGS__);
	#define pass_print(...)    _printf(_CR_"P: " __VA_ARGS__);
	#define info_print(...)    _printf(_CR_"I: " __VA_ARGS__);
	#define trace_print(...)   _printf(_CR_"T: " __VA_ARGS__);
	#define log_print(...)     _printf(_CR_"L: " __VA_ARGS__);
	#define action_print(...)  _printf(_CR_"A: " __VA_ARGS__);
	
#endif
