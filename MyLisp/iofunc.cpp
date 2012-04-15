/* input/output functions */

#include "iofunc.h"
#include "read.h"
#include "print.h"
#include "error.h"

static char* modestr[] = {
	"a", "w", "r", "",
	"ab", "wb", "rb",
};

static CELLP prin(CELLP args, int mode);
static void getfname(STR o, ATOMP ap);
static void pushbuf(void);	// 入力バッファの退避
static int subform(STR tp, CELLP args);
static void copyform(STR o, STR s, STR e);
static void ins_l(STR o, STR s, STR e);

CELLP read_f(CELLP args)
{
	return read_s(TOP);
}

CELLP print_f(CELLP args)
{
	CELLP result;

	result = prin(args, ESCON);
	ec;
	fputc('\n', cur_fpo);
	return result;
}

CELLP prin1_f(CELLP args)
{
	return prin(args, ESCON);
}

CELLP princ_f(CELLP args)
{
	return prin(args, ESCOFF);
}

static CELLP prin(CELLP args, int mode)
{
	CELLP cp;
	if (args->id != _CELL) {
		return error(NEA);
	}
	print_s(cp = args->car, mode);
	return cp;
}

CELLP terpri_f()
{
	fputc('\n', cur_fpo);
	return (CELLP)nil;
}

//CELLP open_f(CELLP args);	// ファイルを開く
//CELLP close_f(CELLP arg);
//CELLP readch_f(CELLP arg);
//CELLP dirin_f(CELLP arg);
//CELLP dirout_f(CELLP arg);
//CELLP dirin(CELLP arg);		// 入力先の変更を行う
//CELLP dirout(CELLP arg);	// 出力先の変更を行う
//CELLP fmode_f(CELLP arg);	// ファイルディスクリプタの状態を返す
//CELLP curin_f(CELLP arg);	// 現在の入力先ファイルのファイルディスクリプタを返す
//CELLP curout_f(CELLP arg);	// 現在の出力先ファイルのファイルディスクリプタを返す
//CELLP load_f(CELLP arg);
//CELLP prompt_f(CELLP arg);
//CELLP format_f(CELLP args);
//static void getfname(STR o, ATOMP ap);
//static void pushbuf(void);	// 入力バッファの退避
//static int subform(STR tp, CELLP args);
//static void copyform(STR o, STR s, STR e);
//static void ins_l(STR o, STR s, STR e);
