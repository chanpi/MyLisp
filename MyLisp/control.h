#ifndef __MY_CONTROL__
#define __MY_CONTROL__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif

	CELLP cond_f(CELLP clauses, CELLP env);
	CELLP setf_f(CELLP args, CELLP env);
	CELLP quote_f(CELLP args, CELLP env);
	CELLP defun_f(CELLP args, CELLP env);
	CELLP oblist_f(void);
	CELLP quit_f(void);
	CELLP reclaim_f(void);
	CELLP verbos_f(CELLP arg);

	CELLP and_f(CELLP args, CELLP env);
	CELLP or_f(CELLP args, CELLP env);
	CELLP psetq_f(CELLP args, CELLP env);
	CELLP set_f(CELLP args, CELLP env);

#if defined __cplusplus
}
#endif

#endif /* __MY_CONTROL__ */