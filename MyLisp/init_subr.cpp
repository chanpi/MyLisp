#include "init_subr.h"
#include "functions.h"
#include "control.h"
#include "iofunc.h"
#include "calc.h"
#include "read.h"
#include "prog.h"

static void init0(void);
static void init1(void);
static void init2(void);
static void init3(void);

static void defsubr(STR name, CELLP funcp, char type);

//#define _SUBR		0x06	// ˆø”•]‰¿EC
//#define _EXPR		0x04	// ˆø”•]‰¿
//#define _FSUBR	0x02	// ˆø”•]‰¿‚µ‚È‚¢EC
// ‘g‚Ýž‚ÝŠÖ”‚Ì“o˜^
void init_subr()
{
	init0();
	init1();
	init2();
	init3();
}

static void init0(void)
{
	defsubr((STR)"car",		(CELLP)car_f,		_SUBR);
	defsubr((STR)"cdr",		(CELLP)cdr_f,		_SUBR);
	defsubr((STR)"cons",	(CELLP)cons_f,		_SUBR);
	defsubr((STR)"atom",	(CELLP)atom_f,		_SUBR);
	defsubr((STR)"eq",		(CELLP)eq_f,		_SUBR);
	defsubr((STR)"equal",	(CELLP)equal_f,		_SUBR);
	defsubr((STR)"defun",	(CELLP)defun_f,		_FSUBR);
	defsubr((STR)"cond",	(CELLP)cond_f,		_FSUBR);
	defsubr((STR)"setf",	(CELLP)setf_f,		_FSUBR);
	defsubr((STR)"oblist",	(CELLP)oblist_f,	_SUBR);
	defsubr((STR)"quit",	(CELLP)quit_f,		_SUBR);
	defsubr((STR)"putprop",	(CELLP)putprop_f,	_SUBR);
	defsubr((STR)"get",		(CELLP)get_f,		_SUBR);
	defsubr((STR)"remprop", (CELLP)remprop_f,	_SUBR);
	defsubr((STR)"read",	(CELLP)read_f,		_SUBR);
	defsubr((STR)"terpri",	(CELLP)terpri_f,	_SUBR);
	defsubr((STR)"print",	(CELLP)print_f,		_SUBR);
	defsubr((STR)"prin1",	(CELLP)prin1_f,		_SUBR);
	defsubr((STR)"princ",	(CELLP)princ_f,		_SUBR);
	defsubr((STR)"minus",	(CELLP)minus_f,		_SUBR);
	defsubr((STR)"plus",	(CELLP)plus_f,		_SUBR);
}

static void init1(void)
{
	defsubr((STR)"reclaim",	(CELLP)reclaim_f,	_SUBR);
	defsubr((STR)"verbos",	(CELLP)verbos_f,	_SUBR);
}

static void defsubr(STR name, CELLP funcp, char type)
{
	ATOMP ap;

	ap = make_atom(name);
	ap->ftype = type;
	ap->fptr = (CELLP)funcp;
}

static void init2(void)
{
	defsubr((STR)"progn",		(CELLP)progn_f,	_FSUBR);
	defsubr((STR)"throw",		(CELLP)throw_f,	_FSUBR);
	defsubr((STR)"catch",		(CELLP)catch_f,	_FSUBR);
	defsubr((STR)"catcherror",	(CELLP)cerr_f,	_FSUBR);
	defsubr((STR)"let",			(CELLP)let_f,	_FSUBR);
	defsubr((STR)"let*",		(CELLP)lets_f,	_FSUBR);
	defsubr((STR)"prog",		(CELLP)prog_f,	_FSUBR);
	defsubr((STR)"return",		(CELLP)ret_f,	_SUBR);
	defsubr((STR)"go",			(CELLP)go_f,	_FSUBR);
}

static void init3(void)
{
	//defsubr((STR)"format",		(CELLP)format_f,	_SUBR);
	//defsubr((STR)"go",			(CELLP)go_f,	_FSUBR);
}