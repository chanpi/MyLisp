/* function for calclation numbers */

#include "calc.h"
#include "error.h"
#include "gbc.h"

static CELLP setfirst(CELLP args, NUMP* npp);

// 最初の引数を返す（数値の場合）
CELLP get1arg(CELLP args, NUMP valp)
{
	char c;
	if (args->id != _CELL) {
		return error(NEA);
	}
	if ((c = args->car->id) == _FIX) {
		valp->id = _FIX;
		valp->value.fix = ((NUMP)(args->car))->value.fix;
		return args->cdr;
	}
	if (c == _FLT) {
		valp->id = _FLT;
		valp->value.flt = ((NUMP)(args->car))->value.flt;
		return args->cdr;
	}
	return error(IAN);
}

static CELLP setfirst(CELLP args, NUMP* npp)
{
	NUM val;
	args = get1arg(args, &val);	// 1番目(car)はvalに、2番目以降(cdr)はvalに収められる
	ec;
	*npp = newnum();
	ec;
	if (val.id == _FIX) {
		(*npp)->value.fix = val.value.fix;
	} else {
		(*npp)->id = _FLT;
		(*npp)->value.flt = val.value.flt;
	}
	return args;
}

void toflt(NUMP np)
{
	if (np->id != _FLT) {
		np->id = _FLT;
		np->value.flt = (double)(np->value.fix);
	}
}

CELLP minus_f(CELLP args)
{
	char c;
	NUMP np;

	if (args->id != _CELL) {
		return error(NEA);
	}
	if ((c = args->car->id) != _FIX && c != _FLT) {
		return error(IAN);
	}
	np = newnum();
	ec;
	if (c == _FIX) {
		np->value.fix = ((NUMP)(args->car))->value.fix;
	} else {
		np->id = _FLT;
		np->value.flt = ((NUMP)(args->car))->value.flt;
	}
	if ((c = args->cdr->car->id) != _FIX && c != _FLT) {
		return error(IAN);
	}
	if (c == _FLT || np->id == _FLT) {
		np->id = _FLT;
		np->value.flt -= ((NUMP)(args->cdr->car))->value.flt;
	} else {
		np->value.fix -= ((NUMP)(args->cdr->car))->value.fix;
	}
	return (CELLP)np;
}

CELLP plus_f(CELLP args)
{
	NUM val;
	NUMP np;

	args = setfirst(args, &np);
	ec;
	if (args == (CELLP)nil) {
		return (CELLP)np;
	}
	args = get1arg(args, &val);
	ec;
	if (np->id == _FIX) {
		while (val.id == _FIX) {
			np->value.fix += val.value.fix;
			if (args == (CELLP)nil) {
				return (CELLP)np;
			}
			args = get1arg(args, &val);
			ec;
		}
		toflt(np);
	} else {
		toflt(&val);
	}
	for (;;) {
		np->value.flt += val.value.flt;
		if (args == (CELLP)nil) {
			return (CELLP)np;
		}
		args = get1arg(args, &val);
		ec;
		toflt(&val);
	}
}

CELLP add1_f(CELLP arg)
{
	char c;
	NUMP np;

	if (arg->id != _CELL) {
		return error(NEA);
	}
	if ((c = arg->car->id)  != _FIX && c != _FLT) {
		return error(IAN);
	}
	np = newnum(); ec;
	if (c == _FIX) {
		np->value.fix = ((NUMP)(arg->car))->value.fix + 1;
	} else {
		np->id = _FLT;
		np->value.flt = ((NUMP)(arg->car))->value.flt + 1;
	}
	return (CELLP)np;
}

CELLP sub1_f(CELLP arg)
{
	char c;
	NUMP np;

	if (arg->id != _CELL) {
		return error(NEA);
	}
	if ((c = arg->car->id)  != _FIX && c != _FLT) {
		return error(IAN);
	}
	np = newnum(); ec;
	if (c == _FIX) {
		np->value.fix = ((NUMP)(arg->car))->value.fix - 1;
	} else {
		np->id = _FLT;
		np->value.flt = ((NUMP)(arg->car))->value.flt - 1;
	}
	return (CELLP)np;
}

CELLP times_f(CELLP args)
{
	NUM val;
	NUMP np;

	args = setfirst(args, &np); ec;
	if (args == (CELLP)nil) {
		return (CELLP)np;
	}
	args = get1arg(args, &val); ec;
	if (np->id == _FIX) {
		while (val.id == _FIX) {
			np->value.fix *= val.value.fix;
			if (args == (CELLP)nil) {
				return (CELLP)np;
			}
			args = get1arg(args, &val); ec;
		}
		toflt(np);
	} else {
		toflt(&val);
	}

	for (;;) {
		np->value.flt *= val.value.flt;
		if (args == (CELLP)nil) {
			return (CELLP)np;
		}
		args = get1arg(args, &val); ec;
		toflt(&val);
	}
	// ここには来ない
}

CELLP difference_f(CELLP args)
{
	NUM val;
	NUMP np;

	args = setfirst(args, &np); ec;
	if (args == (CELLP)nil) {
		return (CELLP)np;
	}
	args = get1arg(args, &val); ec;
	if (np->id == _FIX) {
		while (val.id == _FIX) {
			np->value.fix -= val.value.fix;
			if (args == (CELLP)nil) {
				return (CELLP)np;
			}
			args = get1arg(args, &val); ec;
		}
		toflt(np);
	} else {
		toflt(&val);
	}
	for (;;) {
		np->value.flt -= val.value.flt;
		if (args == (CELLP)nil) {
			return (CELLP)np;
		}
		args = get1arg(args, &val); ec;
		toflt(&val);
	}
	// ここには来ない
}

CELLP quotient_f(CELLP args)
{
	NUM val;
	NUMP np;

	args = setfirst(args, &np); ec;
	if (args == (CELLP)nil) {
		return (CELLP)np;
	}
	args = get1arg(args, &val); ec;
	if (np->id == _FIX) {
		while (val.id == _FIX) {
			if (val.value.fix == 0) {
				return error(DIVZERO);
			}
			np->value.fix /= val.value.fix;
			if (args == (CELLP)nil) {
				return (CELLP)np;
			}
			args = get1arg(args, &val); ec;
		}
		toflt(np);
	} else {
		toflt(&val);
	}
	for (;;) {
		if (val.value.flt == 0) {
			return error(DIVZERO);
		}
		np->value.flt /= val.value.flt;
		if (args == (CELLP)nil) {
			return (CELLP)np;
		}
		args = get1arg(args, &val); ec;
		toflt(&val);
	}
	// ここには来ない
}

CELLP remainder_f(CELLP args)
{
	NUM val;
	NUMP np;

	args = setfirst(args, &np); ec;
	if (args == (CELLP)nil) {
		return (CELLP)np;
	}
	args = get1arg(args, &val); ec;
	if (np->id == _FIX) {
		while (val.id == _FIX) {
			np->value.fix %= val.value.fix;
			if (args == (CELLP)nil) {
				return (CELLP)np;
			}
			args = get1arg(args, &val); ec;
		}
	}
	return error(IAF);
}

CELLP divide_f(CELLP args)
{
	CELLP cp;

	stackcheck;
	*(++sp) = quotient_f(args); ec;
	stackcheck;
	*(++sp) = remainder_f(args); ec;
	stackcheck;
	*(++sp) = newcell(); ec;
	cp = newcell(); ec;
	cp->car = *(sp-2);
	cp->cdr = *sp;
	(*sp)->car = *(sp-1);
	sp -= 3;
	return cp;
}
