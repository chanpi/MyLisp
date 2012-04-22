#ifndef __MY_CALC__
#define __MY_CALC__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif
	
	CELLP get1arg(CELLP args, NUMP valp);
	void toflt(NUMP np);
	CELLP minus_f(CELLP args);
	CELLP plus_f(CELLP args);

	CELLP add1_f(CELLP arg);
	CELLP sub1_f(CELLP arg);
	CELLP times_f(CELLP args);
	CELLP difference_f(CELLP args);
	CELLP quotient_f(CELLP args);
	CELLP remainder_f(CELLP args);
	CELLP divide_f(CELLP args);

#if defined __cplusplus
}
#endif

#endif /* __MY_CALC__ */