#ifndef __Log_H__
#define __Log_H__
#include <stdio.h>
#include "Uart.h"

#define DEBUG			false
//#define DEBUG			true


#define M1(a,b) 		a##b

#ifndef Log

#if DEBUG
//	#define Log			  printf
	#define Log 		  print
#else
	#define Log(...) 		
#endif

#endif


#endif
