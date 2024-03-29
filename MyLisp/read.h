#ifndef __MY_READ__
#define __MY_READ__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif

CELLP	read_s(int level);
ATOMP	old_atom(STR name);	// その印字名がoblistに登録されているか
ATOMP	make_atom(STR name);	// シンボルを作り、oblistに登録する
void	intern(ATOMP ap);	// シンボルをoblistに登録する
int		num(STR x);
int		isesc(uchar c);		// 特殊文字かどうか
int		isprkana(uchar c);
int		iskanji(uchar c);
int		iskanji2(uchar c);

#if defined __cplusplus
}
#endif

#endif	/* __MY_READ__ */