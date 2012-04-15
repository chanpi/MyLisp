#include "functions.h"
#include "error.h"
#include "gbc.h"
#include <string.h>

/* Pure lisp functions */
CELLP car_f(CELLP args)
{
	if (args->id != _CELL) {
		return error(NEA);
	}
	if (args->car == (CELLP)nil) {
		return (CELLP)nil;
	}
	if (args->car->id != _CELL) {
		return error(IAL);
	}
	return args->car->car;
}

CELLP cdr_f(CELLP args)
{
	if (args->id != _CELL) {
		return error(NEA);
	}
	if (args->car == (CELLP)nil) {
		return (CELLP)nil;
	}
	if (args->car->id != _CELL) {
		return error(IAL);
	}
	return args->car->cdr;
}

CELLP cons_f(CELLP args)
{
	CELLP cp;

	if (args->id != _CELL || args->cdr->id != _CELL) {
		return error(NEA);
	}
	cp = cons(args->car, args->cdr->car); ec;
	return cp;
}

CELLP cons(CELLP arg1, CELLP arg2)
{
	CELLP cp;

	cp = newcell(); ec;
	cp->car = arg1;
	cp->cdr = arg2;
	return cp;
}

CELLP eq_f(CELLP args)
{
	if (args->id != _CELL || args->cdr->id != _CELL) {
		return error(NEA);
	}
	if (args->car == args->cdr->car) {
		return (CELLP)t;
	}
	return (CELLP)nil;
}

CELLP atom_f(CELLP args)
{
	if (args->id != _CELL) {
		return error(NEA);
	}
	if (args->car->id != _CELL) {
		return (CELLP)t;
	}
	return (CELLP)nil;
}

CELLP equal_f(CELLP args)
{
	if (args->id != _CELL || args->cdr->id != _CELL) {
		return error(NEA);
	}
	while (args->cdr->id == _CELL) {
		if (!equal(args->car, args->cdr->car)) {
			return (CELLP)nil;
		}
		args = args->cdr;
	}
	return (CELLP)t;
}

int equal(CELLP arg1, CELLP arg2)
{
	if (arg1 == arg2) {
		return TRUE;
	}
	if (arg1->id != arg2->id) {
		return FALSE;
	}
	switch (arg1->id) {
	case _FIX:
		if (((NUMP)arg1)->value.fix == ((NUMP)arg2)->value.fix) {
			return TRUE;
		} else {
			return FALSE;
		}
		break;
	case _FLT:
		if (((NUMP)arg1)->value.flt == ((NUMP)arg2)->value.flt) {
			return TRUE;
		} else {
			return FALSE;
		}
		break;
	case _ATOM:
		if (!strcmp((const char*)((ATOMP)arg1)->name, (const char*)((ATOMP)arg2)->name)) {
			return TRUE;
		} else {
			return FALSE;
		}
		break;
	case _CELL:
		if (equal(arg1->car, arg2->car) && equal(arg1->cdr, arg2->cdr)) {
			return TRUE;
		} else {
			return FALSE;
		}
		break;
	default:
		return (int)error(ULO);
	}
}

/* Property list */

// (key1 val1 key2 val2 key3 val3 ...)�@������
CELLP putprop_f(CELLP args)
{
	CELLP val, cp;
	ATOMP key, ap;

	if (args->id != _CELL ||
		args->cdr->id != _CELL ||
		args->cdr->cdr->id != _CELL) {
			return error(NEA);
	}
	if ((ap = (ATOMP)args->car)->id != _ATOM ||
		(key = (ATOMP)args->cdr->cdr->car)->id != _ATOM) {
			return error(IAA);
	}
	val = args->cdr->car;
	for (cp = ap->plist; cp->id == _CELL; cp = cp->cdr->cdr) {
		if ((ATOMP)cp->car == key) {
			return (cp->cdr->car = val);
		}
	}
	stackcheck;
	*++sp = newcell(); ec;
	cp = *sp;
	cp->car = (CELLP)key;
	cp->cdr = newcell();
	cp->cdr->car = val;
	cp->cdr->cdr = ap->plist;
	ap->plist = *sp--;
	return val;
}

CELLP get_f(CELLP args)
{
	CELLP cp;
	ATOMP key, ap;

	if (args->id != _CELL || args->cdr->id != _CELL) {
		return error(NEA);
	}
	if ((ap = (ATOMP)args->car)->id != _ATOM ||
		(key = (ATOMP)args->cdr->car)->id != _ATOM) {
			return error(IAA);
	}
	for (cp = ap->plist; cp->id == _CELL; cp = cp->cdr->cdr) {
		if ((ATOMP)cp->car == key) {
			return cp->cdr->car;
		}
	}
	return (CELLP)nil;
}

CELLP remprop_f(CELLP args)
{
	CELLP val, cp;
	ATOMP key, ap;

	if (args->id != _CELL || args->cdr->id != _CELL) {
		return error(NEA);
	}
	if ((ap = (ATOMP)args->car)->id != _ATOM ||
		(key = (ATOMP)args->cdr->car)->id != _ATOM) {
			return error(IAA);
	}
	if ((cp = ap->plist) == (CELLP)nil) {
		return (CELLP)nil;
	}
	if ((ATOMP)cp->car == key) {
		ap->plist = cp->cdr->cdr;
		return cp->cdr->car;
	}
	for (cp = cp->cdr; cp->cdr->id == _CELL; cp = cp->cdr->cdr) {
		if ((ATOMP)cp->cdr->car == key) {
			val = cp->cdr->cdr->car;
			cp->cdr = cp->cdr->cdr->cdr;
			return val;
		}
	}
	return (CELLP)nil;
}