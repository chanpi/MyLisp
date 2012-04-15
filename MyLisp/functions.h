#ifndef __MY_FUNCTIONS__
#define __MY_FUNCTIONS__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif

	CELLP car_f(CELLP args);
	CELLP cdr_f(CELLP args);
	CELLP cons_f(CELLP args);
	CELLP cons(CELLP arg1, CELLP arg2);
	CELLP eq_f(CELLP args);
	CELLP atom_f(CELLP args);
	CELLP equal_f(CELLP args);
	int equal(CELLP arg1, CELLP arg2);
	CELLP putprop_f(CELLP args);
	CELLP get_f(CELLP args);
	CELLP remprop_f(CELLP args);

#if defined __cplusplus
}
#endif

#endif /* __MY_FUNCTIONS__ */