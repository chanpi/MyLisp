#include "pred.h"
#include "error.h"
#include "calc.h"
#include "functions.h"

CELLP null_f(CELLP args)
{
	if (args->id != _CELL) {
		return error(NEA);
	}
	if (args->car == (CELLP)nil) {
		return (CELLP)t;
	}
	return (CELLP)nil;
}

CELLP zerop_f(CELLP arg)
{
	char c;

	if (arg->id != _CELL) {
		return error(NEA);
	}
	if ((c = arg->car->id) != _FIX && c != _FLT) {
		return error(IAN);
	}
	if (c == _FIX) {
		if (((NUMP)(arg->car))->value.fix == 0) {
			return (CELLP)t;
		}
	} else if (c == _FLT) {
		if (((NUMP)(arg->car))->value.flt == 0) {
			return (CELLP)t;
		}
	}
	return (CELLP)nil;
}

CELLP numberp_f(CELLP arg)
{
	char c;
	if (arg->id != _CELL) {
		return error(NEA);
	}
	if ((c = arg->car->id) == _FIX || c == _FLT) {
		return (CELLP)t;
	}
	return (CELLP)nil;
}

CELLP greaterp_f(CELLP args)
{
	NUM val1, val2;

	args = get1arg(args, &val1); ec;
	if (args == (CELLP)nil) {
		return error(NEA);
	}
	args = get1arg(args, &val2); ec;
	if (val1.id == _FIX) {
		while (val2.id == _FIX) {
			if (val1.value.fix <= val2.value.fix) {
				return (CELLP)nil;
			}
			if (args == (CELLP)nil) {
				return (CELLP)t;
			}
			val1.value.fix = val2.value.fix;
			args = get1arg(args, &val2); ec;
		}
		toflt(&val1);
	} else {
		toflt(&val2);
	}
	for (;;) {
		if (val1.value.flt <= val2.value.flt) {
			return (CELLP)nil;
		}
		if (args == (CELLP)nil) {
			return (CELLP)t;
		}
		val1.value.flt = val2.value.flt;
		args = get1arg(args, &val2); ec;
		toflt(&val2);
	}
}

CELLP lessp_f(CELLP args)
{
	NUM val1, val2;

	args = get1arg(args, &val1); ec;
	if (args == (CELLP)nil) {
		return error(NEA);
	}
	args = get1arg(args, &val2); ec;
	if (val1.id == _FIX) {
		while (val2.id == _FIX) {
			if (val1.value.fix >= val2.value.fix) {
				return (CELLP)nil;
			}
			if (args == (CELLP)nil) {
				return (CELLP)t;
			}
			val1.value.fix = val2.value.fix;
			args = get1arg(args, &val2); ec;
		}
		toflt(&val1);
	} else {
		toflt(&val2);
	}
	for (;;) {
		if (val1.value.flt >= val2.value.flt) {
			return (CELLP)nil;
		}
		if (args == (CELLP)nil) {
			return (CELLP)t;
		}
		val1.value.flt = val2.value.flt;
		args = get1arg(args, &val2); ec;
		toflt(&val2);
	}
}

CELLP member_f(CELLP args)
{
	CELLP factor;

	if (args->id != _CELL || args->cdr->id != _CELL) {
		return error(NEA);
	}
	factor = args->car;
	args = args->cdr->car;
	while (args->id == _CELL) {
		if (equal(factor, args->car)) {
			return args;
		}
		args = args->cdr;
	}
	return (CELLP)nil;
}