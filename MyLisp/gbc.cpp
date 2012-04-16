#include "gbc.h"
#include "error.h"
#include <string.h>

static int mark(CELLP cp, int n);	// �g�p���̍\���̂Ƀ}�[�N��t����Bn�͐��l�̈�Ƀ}�[�N�����邩�ǂ����̃t���O�B
static void remove_mark_num(void);
static void remove_mark_atom(void);
static int collect_cell(void);
static int collect_num(void);
static int collect_atom(void);
static int collect_str(void);

CELLP	newcell()
{
	CELLP cp;
	if (freecell == (CELLP)nil) {
		gbc(OFF, OFF); ec;
	}
	cp = freecell;
	freecell = freecell->cdr;
	cp->cdr = (CELLP)nil;
	return cp;
}

ATOMP	newatom()
{
	ATOMP ap;

	if (freeatom == (ATOMP)nil) {
		gbc(OFF, ON); ec;
	}
	ap = freeatom;
	freeatom = (ATOMP)freeatom->plist;	// �v���p�e�B�i�����j���X�g
	return ap;
}

NUMP	newnum()
{
	NUMP np;
	
	if (freenum == (NUMP)nil) {
		gbc(ON, OFF); ec;
	}
	np = freenum;
	freenum = freenum->value.ptr;
	return np;
}

int gbc(int n, int a)
{
	CELLP* sp1;
	int i, s;

	if (verbos) {
		if (n & a) {
			fprintf(stdout, "\nYou surprised Gbc,\n");
		} else {
			fprintf(stdout, "\nGbc surprised you,\n");
		}
	}

	// oblist�ƃ\�t�g�E�F�A�X�^�b�N����g�p���̍\���̂���J���Ă���
	for (i = 0; i < TABLESIZE; ++i) {
		mark(oblist[i], n);
	}
	for (sp1 = stacktop; sp1 <= sp; ++sp1) {
		mark(*sp1, n);
	}

	i = collect_cell(); ec;
	if (n) {
		n = collect_num(); ec;
	}
	if (a) {
		s = collect_str();
		a = collect_atom();
	} else {
		remove_mark_atom();	// �A�g���̈��GC�����Ȃ��Ƃ��͕����A�g���\���̂ɂ܂��}�[�N���c���Ă���̂ŏ���
	}

	if (verbos) {
		fprintf(stdout, "\tfree cell = %d\n", i);
		if (n) {
			fprintf(stdout, "\tfree num  = %d\n", n);
		}
		if (a) {
			fprintf(stdout, "\tfree atom = %d\n", a);
			fprintf(stdout, "\tfree str  = %d\n", s);
		}
	}
	return 0;
}

static int mark(CELLP cp, int n)	// �g�p���̍\���̂Ƀ}�[�N��t����Bn�͐��l�̈�Ƀ}�[�N�����邩�ǂ����̃t���O�B
{
	char c = cp->id;

	if (cp == (CELLP)nil) {
		return 0;		// nil�ɂ̓}�[�N��t���Ȃ�
	}
	if (c & USED) {
		return 0;
	}
	switch (c) {
	case _ATOM:
		cp->id |= USED;
		mark(((ATOMP)cp)->value, n);
		mark(((ATOMP)cp)->plist, n);
		if (!((((ATOMP)cp)->ftype) & NONMARK)) {
			mark(((ATOMP)cp)->fptr, n);
		}
		break;

	case _CELL:
		cp->id |= USED;
		mark(cp->car, n);
		mark(cp->cdr, n);
		break;

	case _FIX:
	case _FLT:
		if (n) {
			cp->id |= USED;
		}
		break;

	default:
		return (int)error(ULO);
	}
	return 0;
}

static void remove_mark_num(void)
{
	NUMP np;

	for (np = numtop; np < numtop + NUMSIZE; ++np) {
		np->id &= FREE;
	}
}

static void remove_mark_atom(void)
{
	ATOMP ap;

	for (ap = atomtop + 1; ap < atomtop + ATOMSIZE; ++ap) {
		ap->id = _ATOM;
	}
}

static int collect_cell(void)
{
	int n = 1;
	CELLP end, cp = celltop;

	while (cp->id & USED) {	// �ŏ��̖��g�p�̃Z����T��
		cp->id &= FREE;
		if (++cp >= celltop + CELLSIZE) {
			// �G���[�ŒE�o����O��GC�����Ă��܂����}�[�N�������Ă����i�Z���̈�̃}�[�N�͂��łɏ����Ă��܂����̂ł��̂܂܂ł悢�j
			remove_mark_num();
			remove_mark_atom();
			return (int)error(CELLUP);
		}
	}
	freecell = end = cp++;
	end->car = (CELLP)nil;

	for (; cp < celltop + CELLSIZE; ++cp) {
		if (cp->id & USED) {
			cp->id &= FREE;
			continue;
		}
		end->cdr = cp;
		end = cp;
		end->car = (CELLP)nil;
		++n;
	}
	end->cdr = (CELLP)nil;
	return n;
}

static int collect_num(void)
{
	int n = 1;
	NUMP end, np = numtop;

	for (;;) {
		if (!(np->id & USED)) {
			break;
		}
		np->id &= FREE;
		if (++np >= numtop + NUMSIZE) {
			remove_mark_atom();
			return (int)error(NUMUP);
		}
	}
	freenum = end = np++;
	end->id = _FIX;

	for (; np < numtop + NUMSIZE; ++np) {
		if (np->id & USED) {
			np->id &= FREE;
			continue;
		}
		end->value.ptr = np;
		end = np;
		end->id = _FIX;
		++n;
	}
	end->value.ptr = (NUMP)nil;
	return n;
}

static int collect_atom(void)
{
	int n = 1;
	ATOMP end, ap = atomtop + 1;	// nil������

	for (;;) {
		if (!(ap->id & USED)) {
			break;
		}
		ap->id &= FREE;
		if (++ap >= atomtop + ATOMSIZE) {
			return (int)error(ATOMUP);
		}
	}
	freeatom = end = ap++;

	for (; ap < atomtop + ATOMSIZE; ++ap) {
		if (ap->id & USED) {
			ap->id &= FREE;
			continue;
		}
		end->plist = (CELLP)ap;
		end = ap;
		++n;
	}
	end->plist = (CELLP)nil;
	return n;
}

static int collect_str(void)
{
	STR s, end;
	ATOMP ap;

	*newstr = '\0';
	for (s = end = strtop + strlen("nil") + 1; s < newstr; ) {	// nil������
		for (ap = atomtop + 1; ap < atomtop + ATOMSIZE; ++ap) {
			if (!(ap->id & USED)) {
				continue;
			}
			if (ap->name == s) {
				if (end != s) {
					strcpy((char*)end, (char*)s);
					ap->name = end;
				}
				while (*end++ != '\0');
				break;
			}
		}
		while (*s++ != '\0');
		while (*s == '\0' && s < newstr) {
			++s;
		}
	}
	newstr = end;
	return (int)((strtop + STRSIZE) - newstr);
}
