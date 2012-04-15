#ifndef __MY_PROG__
#define __MY_PROG__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif

	CELLP progn_f(CELLP args, CELLP env);
	CELLP prog_f(CELLP args, CELLP env);
	CELLP go_f(CELLP args, CELLP env);
	CELLP ret_f(CELLP args);
	CELLP catch_f(CELLP args, CELLP env);
	CELLP cerr_f(CELLP args, CELLP env);
	CELLP throw_f(CELLP args, CELLP env);
	CELLP let_f(CELLP args, CELLP env);
	CELLP lets_f(CELLP args, CELLP env);
	CELLP assoc(CELLP key, CELLP alist);

#if defined __cplusplus
}
#endif

#endif /* __MY_PROG__ */