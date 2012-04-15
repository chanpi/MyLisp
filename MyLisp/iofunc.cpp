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
static void pushbuf(void);	// ���̓o�b�t�@�̑ޔ�
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

//CELLP open_f(CELLP args);	// �t�@�C�����J��
//CELLP close_f(CELLP arg);
//CELLP readch_f(CELLP arg);
//CELLP dirin_f(CELLP arg);
//CELLP dirout_f(CELLP arg);
//CELLP dirin(CELLP arg);		// ���͐�̕ύX���s��
//CELLP dirout(CELLP arg);	// �o�͐�̕ύX���s��
//CELLP fmode_f(CELLP arg);	// �t�@�C���f�B�X�N���v�^�̏�Ԃ�Ԃ�
//CELLP curin_f(CELLP arg);	// ���݂̓��͐�t�@�C���̃t�@�C���f�B�X�N���v�^��Ԃ�
//CELLP curout_f(CELLP arg);	// ���݂̏o�͐�t�@�C���̃t�@�C���f�B�X�N���v�^��Ԃ�
//CELLP load_f(CELLP arg);
//CELLP prompt_f(CELLP arg);
//CELLP format_f(CELLP args);
//static void getfname(STR o, ATOMP ap);
//static void pushbuf(void);	// ���̓o�b�t�@�̑ޔ�
//static int subform(STR tp, CELLP args);
//static void copyform(STR o, STR s, STR e);
//static void ins_l(STR o, STR s, STR e);
