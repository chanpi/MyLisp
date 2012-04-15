/* function for calclation numbers */

#include "calc.h"
#include "error.h"
#include "gbc.h"

static CELLP setfirst(CELLP args, NUMP* npp);

// �ŏ��̈�����Ԃ��i���l�̏ꍇ�j
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
	args = get1arg(args, &val);	// 1�Ԗ�(car)��val�ɁA2�Ԗڈȍ~(cdr)��val�Ɏ��߂���
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

