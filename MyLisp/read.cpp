#include "lisp.h"
#include <ctype.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include "read.h"
#include "print.h"
#include "error.h"
#include "gbc.h"

#define	SUP		0
#define NORM	1

static STR		getstr();
static int		skipspace();
static CELLP	escopt(int level);
static NUMP		make_num();
static ATOMP	return_atom();
static unsigned int	hash(STR name);
static ATOMP	make_sub(STR name);				// �V���{���̍\���̂ɒl���Z�b�g����
static int		getname(STR strp);				// �V���{���̈󎚖������o��
static CELLP	make_list(int level);			// ���X�g�̍쐬
static void		getcar(CELLP cp, int level);
static int		getcdr(CELLP cp, int level);

// �v�����v�g�̏o�͋y�ѓ��͕�����̎�荞��
static STR getstr()
{
	if (_isatty(fileno(cur_fpi))) {		// �t�@�C���L�q�q�������f�o�C�X�Ɋ֘A�t�����Ă��邩
		print_s((CELLP)prompt, ESCOFF);
	}
	*(txtp = oneline) = '\0';
	return (STR)fgets((char*)oneline, LINESIZE, cur_fpi);
}

static int skipspace()
{
	for (;;) {
		while (isspace(*txtp)) {
			++txtp;
		}
		if (*txtp != '\0' && *txtp != ';') {
			return TRUE;
		}
		if (getstr() == NULL)  {
			return NULL;			// EOF��ǂ�
		}
		ec;
	}
}

// S���̓���
CELLP read_s(int level)
{
	if (skipspace() == NULL) {	// EOF
		return (CELLP)eofread;
	} else if (num(txtp)) {		// ����+-�̂��Ƃ��l���ă|�C���^�œn��
		return (CELLP)make_num();
	} else if (isesc(*txtp)) {
		return escopt(level);
	} else if (isprkana(*txtp)) {
		return (CELLP)return_atom();	// �{�́Areturn (CELLP)return_atom(ON);	
	} else if (iskanji(*txtp)) {
		return (CELLP)return_atom();	// �{�́Areturn (CELLP)return_atom(ON);	
	} else {
		return error(CTRLIN);
	}
}

// ���ꕶ���ɑ΂��鏈���̊���U��
static CELLP	escopt(int level)
{
	switch (*txtp) {
	case '(':
	case '[':
		return make_list(level);
	case '|':
	case '\\':
		return (CELLP)return_atom();	// return_atom(ON)
	default:
		return error(PSEXP);			// �����_�ł͂��̑��̓G���[
	}
}

// ���l�A�g���̍쐬
static NUMP		make_num()
{
	char type = _FIX;
	uchar numbuf[128], *bufp = numbuf;
	NUMP np;

	if (*txtp == '+' || *txtp == '-') {
		*bufp++ = *txtp++;
	}
	while (isdigit(*txtp)) {
		*bufp++ = *txtp++;
	}
	if (*txtp == '.') {
		type = _FLT;
		*bufp++ = *txtp++;
	}
	while (isdigit(*txtp)) {
		*bufp++ = *txtp++;
	}
	if (tolower(*txtp) == 'e') {
		type = _FLT;
		*bufp++ = *txtp++;
		if (*txtp == '+' || *txtp == '-') {
			*bufp++ = *txtp++;
		}
		while (isdigit(*txtp)) {
			*bufp++ = *txtp++;
		}
	}
	*bufp = '\0';
	np = newnum();
	ec;
	if (type == _FIX) {
		np->value.fix = atol((const char*)numbuf);
	} else {
		np->id = _FLT;
		np->value.flt = atof((const char*)numbuf);
	}
	return np;
}

// oblist�ɓo�^���Ă���΂�����A������Γo�^������ŕԂ�
static ATOMP	return_atom()
{
	uchar namebuf[NAMELEN+1];
	ATOMP ap;

	getname(namebuf);
	if ((ap = old_atom(namebuf)) == NULL) {
		return make_atom(namebuf);
	}
	return ap;
}

static unsigned int	hash(STR name)
{
	unsigned int i = 0;
	while (*name != '\0') {
		i += *name++;
	}
	return (i % TABLESIZE);
}

ATOMP	old_atom(STR name)	// ���̈󎚖���oblist�ɓo�^����Ă��邩
{
	int i = 0;
	ATOMP ap;
	CELLP cp;

	i = hash(name);
	for (cp = oblist[i]; cp != (CELLP)nil; cp = cp->cdr) {
		ap = (ATOMP)cp->car;
		if (!strcmp((const char*)ap->name, (const char*)name)) {
			return ap;
		}
	}
	return NULL;
}

ATOMP	make_atom(STR name)	// �V���{�������Aoblist�ɓo�^����
{
	ATOMP ap;

	ap = make_sub(name);
	ec;
	intern(ap);
	return ap;
}

static ATOMP	make_sub(STR name)				// �V���{���̍\���̂ɒl���Z�b�g����
{
	int length = strlen((char*)name) + 1;	// '\0'�̂Ԃ���܂�
	ATOMP ap;

	if (newstr + length > strtop + STRSIZE) {
		gbc(OFF, ON);
		if (newstr + length > strtop + STRSIZE) {
			return (ATOMP)error(STRUP);
		}
	}

	name = (STR)strcpy((char*)newstr, (const char*)name);
	newstr += length;

	ap  = newatom();
	ec;
	ap->id = _ATOM;
	ap->value = (CELLP)ap;
	ap->name = name;
	ap->plist = (CELLP)nil;
	ap->ftype = _NFUNC;
	return ap;
}

void	intern(ATOMP ap)	// �V���{����oblist�ɓo�^����
{
	int i = 0;
	CELLP cp;

	cp = newcell();
	if (err) {
		return;
	}
	i = hash(ap->name);
	cp->car = (CELLP)ap;
	cp->cdr = oblist[i];
	oblist[i] = cp;
}

static int		getname(STR strp)				// �V���{���̈󎚖������o��
{
	int i, ifesc = OFF;
	for (i = 1; i < NAMELEN; i++) {
		while (*txtp == '|') {		// '|'�ň͂܂�镶����͕��ʂ̕�����Ƃ��Ĉ���
			if (ifesc) {
				ifesc = OFF;
			} else {
				ifesc = ON;
			}
			++txtp;
		}
		// �}���`�v���G�X�P�[�v�łȂ��Ƃ�
		if (!ifesc) {
			if (*txtp == '\\') {
				++txtp;
			} else if (isspace(*txtp) || isesc(*txtp)) {
				if (*txtp == ';') {
					*txtp = '\0';
				}
				*strp = '\0';
				return 1;
			}
		}
		// �V�������͕�����̎�荞��
		if (*txtp == '\n' || *txtp == '\0') {
			if (getstr() == NULL) {
				return (int)error(EOFERR);
			}
			i--;
			continue;
		}
		// �����R�[�h�̎�荞��
		if (iskanji(*txtp)) {
			*strp++ = *txtp++;
			if (*txtp == '\0' && getstr() == NULL) {
				*strp = '\0';
				return 1;
			} else if (iskanji2(*txtp)) {
				*strp++ = *txtp++;
				i++;
				continue;
			}
		}
		if (!isprkana(*txtp)) {
			return (int)error(CTRLIN);
		}
		*strp++ = *txtp++;
	}
	*strp = '\0';
	return 1;
}

// ���X�g�̍쐬
static CELLP	make_list(int level)
{
	char mode;
	CELLP cp1, cp2;

	if (*txtp++ == '[') {
		mode = SUP;
	} else {
		mode = NORM;
	}

	if (skipspace() == NULL) {
		return error(EOFERR);
	}
	if (*txtp == ')') {
		++txtp;
		return (CELLP)nil;
	}
	if (*txtp == ']') {
		if (mode == SUP || level == TOP) {
			++txtp;
		}
		return (CELLP)nil;
	}
	if (*txtp == '.') {
		return error(PSEXP);
	}

	stackcheck;

	cp1 = *++sp = newcell();
	ec;
	getcar(cp1, UNDER);
	ec;
	if (skipspace() == NULL) {
		return error(EOFERR);
	}
	while (*txtp != ')' && *txtp != ']') {
		// �h�b�g�΂����
		if (*txtp == '.') {
			++txtp;
			if (skipspace() == NULL) {
				return error(EOFERR);
			}
			if (*txtp == ')' || *txtp == ']') {
				return error(PSEXP);
			}
			getcdr(cp1, UNDER);
			ec;
			break;
		}
		cp2 = newcell();
		ec;
		cp1->cdr = cp2;
		getcar(cp2, UNDER);
		ec;
		cp1 = cp2;	// ���̃Z���̂��߂ɁB
		if (skipspace() == NULL) {
			return error(EOFERR);
		}
	}
	if (*txtp == ']') {
		if (mode == NORM && level == UNDER) {
			return *sp--;
		}
	}
	++txtp;
	return *sp--;
}

static void		getcar(CELLP cp, int level)
{
	CELLP cp1;
	int ret;
	cp1 = read_s(level);
	if (err) {
		return;
	}
	cp->car = cp1;
}

static int		getcdr(CELLP cp, int level)
{
	CELLP cp1;
	cp1 = read_s(level);
	ec;
	cp->cdr = cp1;
	if (skipspace() == NULL) {
		return (int)error(EOFERR);
	}
	if (*txtp != ')' && *txtp != ']') {
		return (int)error(PSEXP);
	}
}

int		num(STR x)
{
	if (isdigit(*x)) {
		return TRUE;
	}
	if (*x == '-' || *x == '+') {
		if (isdigit(*++x)) {
			return TRUE;
		}
	}
	return FALSE;
}

int		isesc(uchar c)		// ���ꕶ�����ǂ���
{
	switch (c) {
	case ' ':
	case '#':
	case '.':
	case ';':
	case '(':
	case ')':
	case '[':
	case ']':
	case '\\':
	case '^':
	case '`':		// �ǉ����Ă݂�
	case ',':
	case '|':
	case '\'':
		return TRUE;
	}
	return FALSE;
}

int		isprkana(uchar c)
{
	if ((' ' <= c) && (c <= '~')) {
		return TRUE;
	}
	if ((0xA1 <= c) && (c <= 0xDF)) {
		return TRUE;
	}
	return FALSE;
}

int		iskanji(uchar c)
{
	if ((0x80 < c) && (c < 0xA0)) {
		return TRUE;
	}
	if ((0xDF < c) && (c < 0xF1)) {
		return TRUE;
	}
	return FALSE;
}

int		iskanji2(uchar c)
{
	if ((0x3F < c) && (c < 0x7F)) {
		return TRUE;
	}
	if ((0x7F < c) && (c < 0xFD)) {
		return TRUE;
	}
	return FALSE;
}

