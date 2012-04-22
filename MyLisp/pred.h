#ifndef __MY_PRED__
#define __MY_PRED__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif
	
	CELLP null_f(CELLP args);
	CELLP zerop_f(CELLP arg);
	CELLP numberp_f(CELLP arg);
	CELLP greaterp_f(CELLP args);
	CELLP lessp_f(CELLP args);
	CELLP member_f(CELLP args);

#if defined __cplusplus
}
#endif

#endif /* __MY_PRED__ */