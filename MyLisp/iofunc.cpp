/* input/output functions */

#include "iofunc.h"
#include "read.h"
#include "print.h"
#include "error.h"
#include "gbc.h"
#include <io.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>

extern int toplevel_function();

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

CELLP read_f(CELLP arg)
{
	FILE* nfp = cur_fpi;
	CELLP cp;

	// ファイルディスクリプタが指定された時
	if (dirin(arg) != (CELLP)nil) {
		ec;
		cp = read_s(TOP);
		pushbuf();
		cur_fpi = nfp;
		return cp;
	}
	cp = read_s(TOP);
	return read_s(TOP);
}

CELLP print_f(CELLP args)
{
	CELLP result;
	FILE* nfp = cur_fpo;

	result = prin(args, ESCON);	ec;
	fputc('\n', cur_fpo);
	cur_fpo = nfp;
	return result;
}

CELLP prin1_f(CELLP args)
{
	CELLP result;
	FILE* nfp = cur_fpo;

	result = prin(args, ESCON);
	cur_fpo = nfp;
	return result;
}

CELLP princ_f(CELLP args)
{
	CELLP result;
	FILE* nfp = cur_fpo;

	result = prin(args, ESCOFF);
	cur_fpo = nfp;
	return result;
}

static CELLP prin(CELLP args, int mode)
{
	if (args->id != _CELL) {
		return error(NEA);
	}
	dirout(args->cdr); ec;
	print_s(args->car, mode);
	return args->car;
}

CELLP terpri_f(CELLP arg)
{
	FILE* nfp = cur_fpo;

	dirout(arg); ec;
	fputc('\n', cur_fpo);
	cur_fpo = nfp;
	return (CELLP)nil;
}

CELLP open_f(CELLP args)	// ファイルを開く
{
	char type, fname[NAMELEN] = {0};
	int i;
	FILE *lfp = NULL;
	NUMP np;
	STR mode;

	if (args->id != _CELL) {
		return error(NEA);
	}
	if (args->car->id != _ATOM) {
		return error(IAA);
	}
	getfname((STR)fname, (ATOMP)(args->car));
	if ((args = args->cdr)->id != _CELL) {
		mode = (STR)"r";
	} else if (args->car->id != _ATOM) {
		return error(IAA);
	} else {
		mode = ((ATOMP)(args->car))->name;
	}
	for (i = 3; i < NFILES; ++i) {
		if (fp[i].ptr == NULL) {
			switch (*mode) {
			case 'w':
				type = WRITEFILE;
				break;
			case 'a':
				type = APPENDFILE;
				break;
			default:
				type = READFILE;
			}
			if (*(mode+1) == 'b') {
				type |= BINF;
			}
			lfp = fopen(fname, modestr[type]);
			if (lfp == NULL) {
				return error(FNA);
			}
			fp[i].mode = type;
			fp[i].ptr = lfp;
			np = newnum(); ec;
			np->value.fix = (long)i;
			return (CELLP)np;
		}
	}
	return error(TMF);
}

static void getfname(STR o, ATOMP ap)
{
	int i;
	STR os = o;
	STR s = ap->name;

	for (i = 1; i <= NAMELEN - 5; ++i) {
		*o++ = *s++;
	}
	*o = '\0';
	while (*os != '\0') {
		if (*os++ == '.') {
			return;
		}
	}
	strcpy((char*)os, ".lsp");
}

CELLP close_f(CELLP arg)
{
	int i;
	FILE* lfp;

	if (arg->id != _CELL) {
		return error(NEA);
	}
	if (arg->car->id != _FIX) {
		return error(IFD);
	}
	i = (int)(((NUMP)(arg->car))->value.fix);
	if (i < 3 || i >= NFILES) {
		return error(IFD);
	}
	lfp = fp[i].ptr;
	if (lfp == NULL) {
		return error(FNO);
	}
	if (cur_fpo == lfp) {
		cur_fpo = stdout;
	}
	if (cur_fpi == lfp) {
		*txtp = '\0';
		cur_fpi = stdin;
	}
	fp[i].ptr = NULL;
	if (fclose(lfp)) {
		return error(CCF);
	}
	return arg->car;
}

// 入力バッファの退避
static void pushbuf(void)
{
	STR tp;

	while (isspace(*txtp)) {
		++txtp;
	}
	for (tp = txtp; *tp != '\0'; ++tp);
	for (--tp; tp >= txtp; --tp) {
		ungetc(*tp, cur_fpi);
	}
	*txtp = '\0';
}

CELLP readch_f(CELLP arg)
{
	char ch[2] = {0};
	int i;
	FILE* nfp = cur_fpi;
	ATOMP ap;

	dirin(arg); ec;
	if (_isatty(_fileno(cur_fpi))) {
		i = _getch();
	} else {
		i = fgetc(cur_fpi);
	}

	if (i == EOF) {
		return (CELLP)eofread;
	}
	ch[0] = i;
	ch[1] = '\0';
	cur_fpi = nfp;
	if ((ap = old_atom((STR)ch)) == NULL) {
		return (CELLP)make_atom((STR)ch);
	}
	return (CELLP)ap;
}

CELLP dirin_f(CELLP arg)
{
	CELLP cp;

	if ((cp = dirin(arg)) == (CELLP)nil) {
		return error(NEA);
	}
	return cp;
}

CELLP dirout_f(CELLP arg)
{
	CELLP cp;

	if ((cp = dirout(arg)) == (CELLP)nil) {
		return error(NEA);
	}
	return cp;
}

// 入力先の変更を行う
CELLP dirin(CELLP arg)
{
	int i;

	if (arg->id != _CELL) {
		return (CELLP)nil;
	}
	if (arg->car->id != _FIX) {
		return error(IFD);
	}
	i = (int)((NUMP)(arg->car))->value.fix;
	if (i < 0 || i >= NFILES) {
		return error(IFD);
	}
	if (fp[i].ptr == NULL) {
		return error(FNO);
	}
	if (!(fp[i].mode & IFREAD)) {
		return error(FWO);
	}
	pushbuf();
	cur_fpi = fp[i].ptr;
	return arg->car;
}

// 出力先の変更を行う
CELLP dirout(CELLP arg)
{
	int i;

	if (arg->id != _CELL) {
		return (CELLP)nil;
	}
	if (arg->car->id != _FIX) {
		return error(IFD);
	}
	i = (int)((NUMP)(arg->car))->value.fix;
	if (i < 0 || i >= NFILES) {
		return error(IFD);
	}
	if (fp[i].ptr == NULL) {
		return error(FNO);
	}
	if (fp[i].mode & IFREAD) {
		return error(FRO);
	}
	cur_fpo = fp[i].ptr;
	return arg->car;
}

// ファイルディスクリプタの状態を返す
CELLP fmode_f(CELLP arg)
{
	int i;
	ATOMP ap;
	STR s;

	if (arg->id != _CELL) {
		return error(NEA);
	}
	if (arg->car->id != _FIX) {
		return error(IFD);
	}
	i = (int)((NUMP)(arg->car))->value.fix;
	if (i < 0 || i >= NFILES) {
		return error(IFD);
	}
	if (fp[i].ptr == NULL) {
		return (CELLP)nil;
	}
	s = (STR)modestr[fp[i].mode];
	if ((ap = old_atom(s)) == NULL) {
		return (CELLP)make_atom(s);
	}
	return (CELLP)ap;
}

// 現在の入力先ファイルのファイルディスクリプタを返す
CELLP curin_f(CELLP arg)
{
	NUMP np;
	int i;

	np = newnum(); ec;
	for (i = 0; i < NFILES; ++i) {
		if (cur_fpi == fp[i].ptr) {
			np->value.fix = (long)i;
			return (CELLP)np;
		}
	}
	return error(UNDEF);
}

// 現在の出力先ファイルのファイルディスクリプタを返す
CELLP curout_f(CELLP arg)
{
	NUMP np;
	int i;

	np = newnum(); ec;
	for (i = 0; i < NFILES; ++i) {
		if (cur_fpo == fp[i].ptr) {
			np->value.fix = (long)i;
			return (CELLP)np;
		}
	}
	return error(UNDEF);
}

CELLP load_f(CELLP arg)
{
	FILE *lfp, *bak;
	char fname[NAMELEN] = {0};

	if (arg->id != _CELL) {
		return error(NEA);
	}
	bak = cur_fpi;
	pushbuf();
	for (;;) {
		if (arg->car->id != _ATOM) {
			return error(IAA);
		}
		getfname((STR)fname, (ATOMP)(arg->car));
		fprintf(cur_fpo, "\nloading... %s\n", fname);
		if ((lfp = fopen(fname, "r")) == NULL) {
			return error(FNA);
		}
		cur_fpi = lfp;
		toplevel_function();	// トップレベルループを呼んでS式の読み込みと評価を行う
		fclose(lfp);	ec;
		*txtp = '\0';
		if ((arg = arg->cdr)->id != _CELL) {
			break;
		}
	}
	cur_fpi = bak;
	return (CELLP)t;
}

CELLP prompt_f(CELLP arg)
{
	if (arg->id != _CELL) {
		return (CELLP)prompt;		// 引数がなければ現在のpromptを返す
	}
	if ((arg = arg->car)->id != _ATOM) {
		return error(IAA);
	}
	if ((ATOMP)arg == t) {
		prompt = stdprompt;		// 引数がtなら標準プロンプト
	} else {
		prompt = (ATOMP)arg;
	}
	return (CELLP)prompt;
}

CELLP format_f(CELLP args)
{
	FILE* nfp = cur_fpo;

	if (args->id != _CELL) {
		return error(NEA);
	}
	if (args->car != (CELLP)nil) {
		dirout(args);
	}
	ec;
	if ((args = args->cdr)->id != _CELL) {
		return error(NEA);
	}
	if (args->car->id != _ATOM) {
		return error(IAA);
	}
	subform(((ATOMP)(args->car))->name, args->cdr);
	cur_fpo = nfp;
	return (CELLP)nil;
}

static int subform(STR tp, CELLP args)
{
	CELLP tmp = args;
	STR tps;
	char c, s, fmt[NAMELEN] = {0};

	while (*tp != '\0') {
		// "~"が出てくるまでは単に出力するだけ
		for (;; ++tp) {
			if (*tp == '\0') {
				return 0;
			}
			if (*tp == '~') {
				break;
			}
			if (iskanji(*tp)) {
				fputc(*tp++ , cur_fpo);
			}
			fputc(*tp, cur_fpo);
		}
		// 各種指定のチェック
		tps = tp;
		if (*(++tp) == '-') {
			++tp;
		}
		while (isdigit(*tp)) {
			++tp;
		}
		if (*tp == ',') {
			++tp;
		}
		while (isdigit(*tp)) {
			++tp;
		}
		if (*tp == ':') {
			++tp;
			s = 0;
		} else {
			s = 1;
		}
		if (*tp == '@') {
			++tp;
			c = 1;
		} else {
			c = 0;
		}
		switch (*tp) {
		case 'd':
		case 'x':
		case 'o':
			if (s) {
				// 継続オプションのないときは引数をすすめる
				tmp = args;
				args = args->cdr;
			}
			if (tmp->id != _CELL) {
				return (int)error(NEA);
			}
			if (tmp->car->id != _FIX) {
				return (int)error(IAF);
			}
			ins_l((STR)fmt, tps, ++tp);
			fprintf(cur_fpo, fmt, ((NUMP)(tmp->car))->value.fix);
			break;

		case 'e':
		case 'f':
		case 'g':
			if (s) {
				tmp = args;
				args = args->cdr;
			}
			if (tmp->id != _CELL) {
				return (int)error(IAFL);
			}
			if (tmp->car->id != _FLT) {
				return (int)error(IAFL);
			}
			copyform((STR)fmt, tps, ++tp);
			fprintf(cur_fpo, fmt, ((NUMP)(tmp->car))->value.flt);
			break;

		case 'c':
			if (s) {
				tmp = args;
				args = args->cdr;
			}
			if (tmp->id != _CELL) {
				return (int)error(NEA);
			}
			if (tmp->car->id != _FIX) {
				return (int)error(IAF);
			}
			copyform((STR)fmt, tps, ++tp);
			fprintf(cur_fpo, fmt, ((NUMP)(tmp->car))->value.fix);
			break;

		case 'a':
			if (s) {
				tmp = args;
				args = args->cdr;
			}
			if (tmp->id != _CELL) {
				return (int)error(NEA);
			}
			if (tmp->car->id != _ATOM) {	// 引数がシンボルでないときはprincで出力
				print_s(tmp->car, ESCOFF);
				++tp;
			} else {	// 引数がシンボルのときは、引字名を"%s"出力する
				copyform((STR)fmt, tps, ++tp);
				*(fmt + (tp - tps) -1) = 's';
				fprintf(cur_fpo, fmt, ((ATOMP)(tmp->car))->name);
			}
			break;

		case 's':
			if (s) {
				tmp = args;
				args = args->cdr;
			}
			if (tmp->id != _CELL) {
				return (int)error(NEA);
			}
			print_s(tmp->car, ESCON);
			++tp;
			break;

		case 'p':
			if (s) {
				tmp = args;
				args = args->cdr;
			}
			++tp;
			if (tmp->id != _CELL) {
				return (int)error(NEA);
			}
			if (tmp->car->id != _FIX) {
				break;
			}
			if (((NUMP)(tmp->car))->value.fix == 1) {
				if (c) {
					fputc('y', cur_fpo);
				}
				break;
			}
			if (c) {
				fprintf(cur_fpo, "ies");
			} else {
				fputc('s', cur_fpo);
			}
			break;

		case 'n':
			fputc('\n', cur_fpo);
			++tp;
			break;

		case 'r':
			fputc('\r', cur_fpo);
			++tp;
			break;

		case 't':
			fputc('\t', cur_fpo);
			++tp;
			break;

		case 'b':
			fputc('\b', cur_fpo);
			++tp;
			break;

		default:
			args = tmp;
			copyform((STR)fmt, ++tps, ++tp);
			fprintf(cur_fpo, fmt);
		}
	}
	return 0;
}

static void copyform(STR o, STR s, STR e)
{
	while (s != e) {
		if (*s == ':') {	// :は無視する
			++s;
			continue;
		}
		if (*s == '%') {
			*o++ = '%';
			*o = '%';
		} else if (*s == '~') {
			*o = '%';
		} else {
			*o = (*s == ',' ? '.' : *s);
		}
		++s;
		++o;
	}
	*o = '\0';
}

static void ins_l(STR o, STR s, STR e)
{
	while (s != e - 1) {
		if (*s == ':') {
			++s;
			continue;
		}
		if (*s == '%') {
			*o++ = '%';
			*o = '%';
		} else if (*s == '~') {
			*o = '%';
		} else {
			*o = (*s == ',' ? '.' : *s);
		}
		++s;
		++o;
	}
	*o++ = 'l';
	*o++ = *s;
	*o = '\0';
}
