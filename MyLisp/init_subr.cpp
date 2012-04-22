#include "init_subr.h"
#include "functions.h"
#include "control.h"
#include "iofunc.h"
#include "calc.h"
#include "read.h"
#include "prog.h"
#include "pred.h"
#include "str.h"

static void init0(void);
static void init1(void);
static void init2(void);
static void init3(void);
static void init4(void);

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
	init4();
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

static void defsubr(STR name, CELLP funcp, char type)
{
	ATOMP ap;

	ap = make_atom(name);
	ap->ftype = type;
	ap->fptr = (CELLP)funcp;
}

static void init1(void)
{
	defsubr((STR)"reclaim",	(CELLP)reclaim_f,	_SUBR);
	defsubr((STR)"verbos",	(CELLP)verbos_f,	_SUBR);
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
	defsubr((STR)"format",		(CELLP)format_f,	_SUBR);
	defsubr((STR)"prompt",		(CELLP)prompt_f,	_SUBR);
	defsubr((STR)"load",		(CELLP)load_f,		_SUBR);
	defsubr((STR)"open",		(CELLP)open_f,		_SUBR);
	defsubr((STR)"close",		(CELLP)close_f,		_SUBR);
	defsubr((STR)"fmode",		(CELLP)fmode_f,		_SUBR);
	defsubr((STR)"readch",		(CELLP)readch_f,	_SUBR);
	defsubr((STR)"dirin",		(CELLP)dirin_f,		_SUBR);
	defsubr((STR)"dirout",		(CELLP)dirout_f,	_SUBR);
	defsubr((STR)"curin",		(CELLP)curin_f,		_SUBR);
	defsubr((STR)"curout",		(CELLP)curout_f,	_SUBR);
}

static void init4(void)
{
	defsubr((STR)"and",			(CELLP)and_f,		_FSUBR);
	defsubr((STR)"or",			(CELLP)or_f,		_FSUBR);
	defsubr((STR)"psetf",		(CELLP)psetq_f,		_FSUBR);
	defsubr((STR)"set",			(CELLP)set_f,		_FSUBR);
	defsubr((STR)"append",		(CELLP)append_f,	_SUBR);
	defsubr((STR)"nconc",		(CELLP)nconc_f,		_SUBR);
	defsubr((STR)"list",		(CELLP)list_f,		_SUBR);
	defsubr((STR)"reverse",		(CELLP)reverse_f,	_SUBR);
	defsubr((STR)"assoc",		(CELLP)assoc_f,		_SUBR);
	defsubr((STR)"rplaca",		(CELLP)rplaca_f,	_SUBR);
	defsubr((STR)"rplacd",		(CELLP)rplacd_f,	_SUBR);
	defsubr((STR)"length",		(CELLP)length_f,	_SUBR);
	defsubr((STR)"member",		(CELLP)member_f,	_SUBR);
	defsubr((STR)"null",		(CELLP)null_f,		_SUBR);
	defsubr((STR)"zerop",		(CELLP)zerop_f,		_SUBR);
	defsubr((STR)"numberp",		(CELLP)numberp_f,	_SUBR);
	defsubr((STR)"<",			(CELLP)lessp_f,		_SUBR);
	defsubr((STR)">",			(CELLP)greaterp_f,	_SUBR);
	defsubr((STR)"ascii",		(CELLP)ascii_f,		_SUBR);
	defsubr((STR)"implode",		(CELLP)implode_f,	_SUBR);
	defsubr((STR)"explode",		(CELLP)explode_f,	_SUBR);
	defsubr((STR)"alen",		(CELLP)alen_f,		_SUBR);
	defsubr((STR)"1+",			(CELLP)add1_f,		_SUBR);
	defsubr((STR)"1-",			(CELLP)sub1_f,		_SUBR);
	defsubr((STR)"+",			(CELLP)plus_f,		_SUBR);
	defsubr((STR)"-",			(CELLP)difference_f,	_SUBR);
	defsubr((STR)"*",			(CELLP)times_f,		_SUBR);
	defsubr((STR)"/",			(CELLP)quotient_f,	_SUBR);
	defsubr((STR)"%",			(CELLP)remainder_f,	_SUBR);
	defsubr((STR)"divide",		(CELLP)divide_f,	_SUBR);
}