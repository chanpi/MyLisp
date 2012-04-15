#ifndef __MY_READ__
#define __MY_READ__

#include "lisp.h"

#if defined __cplusplus
extern "C" {
#endif

CELLP	read_s(int level);
ATOMP	old_atom(STR name);	// ‚»‚Ìˆóš–¼‚ªoblist‚É“o˜^‚³‚ê‚Ä‚¢‚é‚©
ATOMP	make_atom(STR name);	// ƒVƒ“ƒ{ƒ‹‚ğì‚èAoblist‚É“o˜^‚·‚é
void	intern(ATOMP ap);	// ƒVƒ“ƒ{ƒ‹‚ğoblist‚É“o˜^‚·‚é
int		num(STR x);
int		isesc(uchar c);		// “Áê•¶š‚©‚Ç‚¤‚©
int		isprkana(uchar c);
int		iskanji(uchar c);
int		iskanji2(uchar c);

#if defined __cplusplus
}
#endif

#endif	/* __MY_READ__ */