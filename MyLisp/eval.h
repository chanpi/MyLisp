#ifndef __MY_EVAL__
#define __MY_EVAL__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif

	CELLP eval(CELLP form, CELLP env);
	CELLP bind(CELLP keys, CELLP values, CELLP env);	// ŠÂ‹«‚É‘Î‚µ‚Ä•Ï”‚Ì‘©”›‚ğs‚¤

#if defined __cplusplus
}
#endif

#endif	/* __MY_EVAL__ */