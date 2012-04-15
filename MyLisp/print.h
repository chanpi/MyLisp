#ifndef __MY_PRINT__
#define __MY_PRINT__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif

void print_s(CELLP cp, int mode);       // S式の表示
void print_atom(CELLP cp, int mode);    // アトムの表示

#if defined __cplusplus
}
#endif

#endif	/* __MY_PRINT__ */