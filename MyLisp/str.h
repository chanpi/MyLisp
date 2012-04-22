#ifndef __MY_STR__
#define __MY_STR__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif
	
	CELLP ascii_f(CELLP arg);
	CELLP implode_f(CELLP args);
	CELLP explode_f(CELLP arg);
	CELLP alen_f(CELLP arg);

#if defined __cplusplus
}
#endif

#endif /* __MY_STR__ */