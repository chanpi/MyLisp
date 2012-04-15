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
	CELLP open_f(CELLP args);	// ファイルを開く
	CELLP close_f(CELLP arg);
	CELLP readch_f(CELLP arg);
	CELLP dirin_f(CELLP arg);
	CELLP dirout_f(CELLP arg);
	CELLP dirin(CELLP arg);		// 入力先の変更を行う
	CELLP dirout(CELLP arg);	// 出力先の変更を行う
	CELLP fmode_f(CELLP arg);	// ファイルディスクリプタの状態を返す
	CELLP curin_f(CELLP arg);	// 現在の入力先ファイルのファイルディスクリプタを返す
	CELLP curout_f(CELLP arg);	// 現在の出力先ファイルのファイルディスクリプタを返す
	CELLP load_f(CELLP arg);
	CELLP prompt_f(CELLP arg);
	CELLP format_f(CELLP args);

#if defined __cplusplus
}
#endif

#endif /* __MY_IO_FUNC__ */