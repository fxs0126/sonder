#ifndef _SONDER_H_
#define _SONDER_H_



#ifndef SONDER_DLL_DECL
#	ifdef WIN32 
# 		define SONDER_DLL_DECL __declspec(dllimport)
#	else 
# 		define SONDER_DLL_DECL 
#	endif
#endif

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct t_sonder_value {
	unsigned char format;
	char*	      pUnits;
	union {
		void* pHandle;
		int   integer;
		double real;
		struct t_sonder_vecval*  vertor;
	};	
} t_sonder_value,*p_sonder_value;

typedef struct t_sonder_vecval {
	unsigned int count;
	struct t_sonder_value* vecval;
}t_sonder_vecval,*p_sonder_vecval;

#define SONDER_H_INTERAL 1
#include "../version/version.h"
#include "../common/common.h"
#include "../messages/messages.h"
#include "../options/options.h"
#undef SONDER_H_INTERAL

#ifdef __cplusplus
} //extern "C"
#endif 

#endif
