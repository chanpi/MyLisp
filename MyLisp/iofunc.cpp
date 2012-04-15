/* input/output functions */

#include "iofunc.h"
#include "read.h"
#include "print.h"
#include "error.h"

static CELLP prin(CELLP args, int mode);

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
