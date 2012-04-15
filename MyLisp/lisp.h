/* -*- coding: utf-8 -*- */
#ifndef __MY_LISP__
#define __MY_LISP__

#include <stdio.h>

typedef unsigned char	uchar;
typedef uchar			*STR;

typedef struct cell {
	char			id;
	struct cell*	car;
	struct cell*	cdr;
} CELL;

typedef CELL* CELLP;

typedef struct atom {
	char	id;
	char	ftype;
	CELLP	value;
	CELLP	plist;		// 属性リスト key, val, key, valで並ぶ
	STR		name;
	CELLP	fptr;
} ATOM;

typedef ATOM*	ATOMP;

typedef struct num {
	char id;
	union {
		struct num*	ptr;
		long		fix;
		double		flt;
	} value;
} NUM;

typedef NUM*	NUMP;

// tag number and mask of id and function type

#define _CELL	1		// このidを持つ構造体はセル
#define _ATOM	2		// このidを持つ構造体はシンボルアトム
#define _FIX	3		// このidを持つ構造体は整数アトム
#define _FLT	4		// このidを持つ構造体は実数アトム

#define _NFUNC	0x01
#define _SUBR	0x06	// 引数評価・C
#define _EXPR	0x04	// 引数評価
#define _FSUBR	0x02	// 引数評価しない・C

#define _UD		0x01	// そのシンボルアトムが関数として定義されていないときにON
#define _SR		0x02	// その関数がCで記述されているときにON
#define _EA		0x04	// 引数が評価されるタイプの関数の場合にON

// sizeof...

#define CELLSIZE	0x1996	// 6550(Max of large model)
#define ATOMSIZE	0x0500	// 1280
#define STRSIZE		0x1000	// 4096
#define NUMSIZE		0x1000	// 4096
#define TABLESIZE	64		// oblist用のハッシュテーブルの数
#define LINESIZE	100		// １回に取り込める入力行のバイト数
#define NAMELEN		100		// シンボルアトムの印字名の最大長

// switches

#define ESCON	1
#define ESCOFF	0
#define ON		1
#define OFF		0

#define TOP		0
#define UNDER	1

#define TRUE	(-1)
#define FALSE	0

#define NONERR	0
#define ERR		1
#define ERROK	(-1)

#define THROW	2
#define GO		3
#define RET		4

// error check
#define ec	if(err)return(NULL)

// error number

#define STRUP	1	// String area used up
#define NUMUP	2	// Number area used up
#define ATOMUP	3	// Atom area used up
#define CELLUP	4	// Cell area used up
#define ULO		5	// Undefined Lisp Object
#define PSEXP	6	// Pseudo S-expression
#define CTRLIN	7	// Controll CHR. in the text
#define UDF		8	// Undefined function
#define IFF		9	// Illegal function form
#define NEA		10	// Not enough arguments
#define IAA		11	// Illegal argument--Atom required
#define IAN		12	// Illegal argument--Number required
#define IAL		13	// Illegal argument--List required
#define IAAL	14	// Illegal argument--Atom or List required
#define IAAN	15	// Illegal argument--Atom or Number required
#define IALN	16	// Illegal argument--List or Number required
#define IAF		17	// Illegal argument--Fix num required
#define IAFL	18	// Illegal argument--Float num required
#define ILS		19	// Illegal structure (Illegal form)
#define IASSL	20	// Illegal A-list
#define IPL		21	// Illegal property list
#define EIA		22	// Environment is an Atom
#define EHA		23	// Environment has an Atom
#define CCL		24	// Condition Clause must be a List
#define EOFERR	25	// Unexpected EOF
#define CCC		26	// Cannot Change Constant value
#define	UNDEF	27	// Error undefined
#define STACKUP	28	// Softwear stack used up
#define NSG		29	// No such go-label
#define RWP		30	// Return Without Prog
#define TTA		31	// Throw Tag must be an Atom
#define TWC		32	// Throw Without Catch
#define ILV		33	// Illegal Local Variable List

// GC関係
#define NONMARK		(_UD | _SR)
#define USED		0x80
#define FREE		(~USED)
#define STACKSIZE	0x0500		/* 1280 */
#define stackcheck	if(sp>=stacktop+STACKSIZE){error(STACKUP);return NULL;}

// external variable
#ifdef MAIN
FILE	*cur_fpi, *cur_fpo;

ATOMP	t, nil, lambda, eofread, prompt;

CELLP	oblist[ TABLESIZE ];

CELLP	celltop, freecell;
ATOMP	atomtop, freeatom;
NUMP	numtop, freenum;
STR		strtop, newstr;

uchar	oneline[ LINESIZE ];
STR		txtp;

int err, err_no;
CELLP	*stacktop, *sp;
int		verbos;	// GCのメッセージ用フラグ

CELLP	throwlabel, throwval;

#else
extern FILE	*cur_fpi, *cur_fpo;

extern ATOMP	t, nil, lambda, eofread, prompt;

extern CELLP	oblist[ TABLESIZE ];

extern CELLP	celltop, freecell;
extern ATOMP	atomtop, freeatom;
extern NUMP		numtop, freenum;
extern STR		strtop, newstr;

extern uchar	oneline[ LINESIZE ];
extern STR		txtp;

extern int err, err_no;
extern CELLP	*stacktop, *sp;
extern int		verbos;	// GCのメッセージ用フラグ
extern CELLP	throwlabel, throwval;
#endif

#endif /* __MY_LISP__ */
