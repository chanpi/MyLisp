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
	CELLP open_f(CELLP args);	// �t�@�C�����J��
	CELLP close_f(CELLP arg);
	CELLP readch_f(CELLP arg);
	CELLP dirin_f(CELLP arg);
	CELLP dirout_f(CELLP arg);
	CELLP dirin(CELLP arg);		// ���͐�̕ύX���s��
	CELLP dirout(CELLP arg);	// �o�͐�̕ύX���s��
	CELLP fmode_f(CELLP arg);	// �t�@�C���f�B�X�N���v�^�̏�Ԃ�Ԃ�
	CELLP curin_f(CELLP arg);	// ���݂̓��͐�t�@�C���̃t�@�C���f�B�X�N���v�^��Ԃ�
	CELLP curout_f(CELLP arg);	// ���݂̏o�͐�t�@�C���̃t�@�C���f�B�X�N���v�^��Ԃ�
	CELLP load_f(CELLP arg);
	CELLP prompt_f(CELLP arg);
	CELLP format_f(CELLP args);

#if defined __cplusplus
}
#endif

#endif /* __MY_IO_FUNC__ */