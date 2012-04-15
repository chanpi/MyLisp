#define MAIN

#include "lisp.h"
#include "error.h"
#include "read.h"
#include "eval.h"
#include "print.h"
#include "init_subr.h"
#include "control.h"
#include <io.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int toplevel_function();
void __cdecl quit(int);
void reset_stdin();

static void reset_error();
static void init();
static void make_nil();
static void make_sys_atoms();	// システムが必要とするシンボルを作る
static void greeting();

int main(void) {
	greeting();
	init();
	for (;;) {
		if (err == ERR) {
			print_error((CELLP)nil);	// トップレベルで起きたエラーの処理
		}
		reset_error();
		toplevel_function();
		reset_stdin();
	}
	return 0;
}

int toplevel_function()
{
	CELLP *argp1, *argp2;

	argp1 = ++sp;
	stackcheck;
	argp2 = ++sp;

	for (;;) {
		// Read
		*argp1 = read_s(TOP);
		if (*argp1 == (CELLP)eofread) {
			break;
		}
		ec;
		// Eval
		*argp2 = eval(*argp1, (*argp2, (CELLP)nil));
		switch (err) {
		case ERROK:
		case ERR:
			return (int)(err == ERROK);
		case THROW:
			return (int)error(TWC);
		case GO:
			return (int)error(NSG);
		case RET:
			return (int)error(RWP);
		}
		print_s(*argp2, ESCON);
		ec;
		if (isatty(fileno(cur_fpi))) {
			fputc('\n', cur_fpo);
		}
		fputc('\n', cur_fpo);
	}
	sp -= 2;
}

static void reset_error()
{
	cur_fpi = stdin;
	cur_fpo = stdout;
	err = NONERR;
	txtp = oneline;
	*txtp = '\0';
	for (sp = stacktop; sp < stacktop + STACKSIZE; ++sp) {	// ソフトウェアスタックの初期化
		*sp = (CELLP)nil;
	}
	sp = stacktop - 1;
	verbos = ON;
	throwlabel = throwval = (CELLP)nil;
	prompt = stdprompt;	// プロンプトを元に戻す
}

static void init()
{
	int i;
	CELLP cp;
	ATOMP ap;
	NUMP np;

	freecell = celltop = (CELLP)malloc(sizeof(CELL) * CELLSIZE);
	freeatom = atomtop = (ATOMP)malloc(sizeof(ATOM) * ATOMSIZE);
	freenum = numtop = (NUMP)malloc(sizeof(NUM) * NUMSIZE);
	newstr = strtop = (STR)malloc(STRSIZE);
	sp = stacktop = (CELLP*)malloc(sizeof(CELLP) * STACKSIZE);

	if (freecell == NULL || freeatom == NULL
		|| freenum == NULL || newstr == NULL || sp == NULL) {
			fprintf(stderr, "Oops! Alloc Error : Too Large Data Area.\n");
			fprintf(stderr, "Please change ==SIZE (defined in lisp.h)\n");
			exit(1);
	}
	nil = freeatom++;

	for (cp = celltop; cp < celltop + CELLSIZE; cp++) {
		cp->id = _CELL;
		cp->car = (CELLP)nil;
		cp->cdr = cp + 1;
	}
	(--cp)->cdr = (CELLP)nil;

	for (ap = atomtop; ap < atomtop + ATOMSIZE; ap++) {
		ap->id = _ATOM;
		ap->plist = (CELLP)(ap + 1);
	}
	(--ap)->plist = (CELLP)nil;

	for (np = numtop; np < numtop + NUMSIZE; np++) {
		np->id= _FIX;
		np->value.ptr = np + 1;
	}
	(--np)->value.ptr = (NUMP)nil;

	fp[0].mode = READFILE;
	fp[0].ptr = stdin;
	fp[1].mode = WRITEFILE;
	fp[1].ptr = stdout;
	fp[2].mode = WRITEFILE;
	fp[2].ptr = stderr;
	for (i = 3; i < NFILES; ++i) {
		fp[i].ptr = NULL;
	}

	for (i = 0; i < TABLESIZE; i++) {
		oblist[i] = (CELLP)nil;
	}

	make_sys_atoms();
	init_subr();
	signal(SIGINT, quit);	// Ctrl-Cの飛び先を変える
}

static void make_nil()
{
	char* s = strcpy((char*)newstr, "nil");

	newstr += strlen(s) + 1;
	nil->id = _ATOM;
	nil->value = (CELLP)nil;
	nil->plist = (CELLP)nil;
	nil->name = (STR)s;
	nil->ftype = _NFUNC;
	nil->fptr = (CELLP)nil;
	intern(nil);
}

// システムが必要とするシンボルを作る
static void make_sys_atoms()
{
	make_nil();
	t = make_atom((STR)"t");
	lambda = make_atom((STR)"lambda");
	eofread = make_atom((STR)"EOF");
	prompt = make_atom((STR)"% ");
	quote = make_atom((STR)"quote");
	quote->ftype = _FSUBR;
	quote->fptr = (CELLP)quote_f;
}

static void greeting()
{
	fprintf(stdout, "\n");
	fprintf(stdout, "\tHekeheke Lisp Interpreter\n");
	fprintf(stdout, "\t    (c) 2012 Teemo\n\n");
}

void __cdecl quit(int)
{
	fprintf(stdout, "\n\n\tMay the force be with you!\n");
	fprintf(stdout, "\t\t\t--From Will o'Lisp will Love.\n\n");
	exit(0);
}

void reset_stdin()		// EOFによりコンソールが閉じてしまうのを防ぐ
{
	if (isatty(fileno(stdin))) {
		rewind(stdin);
	} else {
		freopen("CON", "r", stdin);
	}
}
