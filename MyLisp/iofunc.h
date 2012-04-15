#ifndef __MY_IO_FUNC__
#define __MY_IO_FUNC__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif

	CELLP read_f(CELLP args);
	CELLP print_f(CELLP args);
	CELLP prin1_f(CELLP args);
	CELLP princ_f(CELLP args);
	CELLP terpri_f();

#if defined __cplusplus
}
#endif

#endif /* __MY_IO_FUNC__ */