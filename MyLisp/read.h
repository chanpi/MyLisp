#ifndef __MY_READ__
#define __MY_READ__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif

CELLP	read_s(int level);
ATOMP	old_atom(STR name);	// ���̈󎚖���oblist�ɓo�^����Ă��邩
ATOMP	make_atom(STR name);	// �V���{�������Aoblist�ɓo�^����
void	intern(ATOMP ap);	// �V���{����oblist�ɓo�^����
int		num(STR x);
int		isesc(uchar c);		// ���ꕶ�����ǂ���
int		isprkana(uchar c);
int		iskanji(uchar c);
int		iskanji2(uchar c);

#if defined __cplusplus
}
#endif

#endif	/* __MY_READ__ */