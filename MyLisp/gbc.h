#ifndef __MY_GBC__
#define __MY_GBC__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif

	CELLP	newcell();	// �Z���\���̂����R�̈悩����o��
	ATOMP	newatom();
	NUMP	newnum();

	int gbc(int n, int a);

#if defined __cplusplus
}
#endif


#endif	/* __MY_GBC__ */