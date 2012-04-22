#include "functions.h"
#include "error.h"
#include "gbc.h"
#include "prog.h"
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

// (key1 val1 key2 val2 key3 val3 ...)　偶数個
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

CELLP append_f(CELLP args)
{
	CELLP cp1, cp2;

	if (args->id != _CELL) {
		return error(NEA);
	}
	while (args->car->id != _CELL) {
		if (args->car != (CELLP)nil) {
			return error(IAL);
		}
		if (args->cdr->id != _CELL) {
			return args->car;
		}
		args = args->cdr;
	}
	// 最後の引数ならそのまま返す
	if (args->cdr->id != _CELL) {
		return args->car;
	}
	// 前につなぐリストはコピーを取る
	*++sp = newcell(); ec;
	cp1 = *sp;
	cp2 = args->car;
	cp1->car = cp2->car;
	cp2 = cp2->cdr;
	while (cp2->id == _CELL) {
		cp1->cdr = newcell(); ec;
		cp1 = cp1->cdr;
		cp1->car = cp2->car;
		cp2 = cp2->cdr;
	}

	// argsを追加セルへ
	args = args->cdr;
	// append対象がある限り行う
	while (args->cdr->id == _CELL) {
		while (args->car->id != _CELL) {
			if (args->car != (CELLP)nil) {
				return error(IAL);
			}
			if (args->cdr->id != _CELL) {
				cp1->cdr = (CELLP)nil;
				return *sp--;
			}
			args = args->cdr;
		}
		cp2 = args->car;
		while (cp2->id == _CELL) {
			cp1->cdr = newcell(); ec;
			cp1 = cp1->cdr;
			cp1->car = cp2->car;
			cp2 = cp2->cdr;
		}
		args = args->cdr;
	}
	if (args->car->id != _CELL && args->car != (CELLP)nil) {
		return error(IAL);
	}
	cp1->cdr = args->car;
	return *sp--;
}

CELLP nconc_f(CELLP args)
{
	CELLP cp1, cp2;

	if (args->id != _CELL) {
		return error(NEA);
	}
	// nilは無視する
	while (args->car == (CELLP)nil && args->cdr->id == _CELL) {
		args = args->cdr;
	}
	if (args->car->id != _CELL && args->car != (CELLP)nil) {
		return error(IAL);
	}
	if (args->cdr->id != _CELL) {
		return args->car;
	}
	cp1 = args->car;
	cp2 = cp1;
	args = args->cdr;
	// 最初のリストの最後のリストを取り出し、cdrを書き換える
	while (cp2->cdr->id == _CELL) {
		cp2 = cp2->cdr;
	}
	cp2->cdr = args->car;
	while (args->cdr->id == _CELL) {
		args = args->cdr;
		if (args->car->id != _CELL && args->car != (CELLP)nil) {
			return error(IAL);
		}
		while (cp2->cdr->id == _CELL) {
			cp2 = cp2->cdr;
		}
		cp2->cdr = args->car;
	}
	return cp1;
}

CELLP reverse_f(CELLP args)
{
	CELLP cp;

	if (args->id != _CELL) {
		return error(NEA);
	}
	cp = args->car;
	*++sp = (CELLP)nil;
	while (cp->id == _CELL) {
		*sp = cons(cp->car, *sp);
		cp = cp->cdr;
	}
	return *sp--;
}

CELLP list_f(CELLP args)
{
	CELLP cp;

	if (args->id != _CELL) {
		return (CELLP)nil;
	}
	*++sp = newcell(); ec;
	cp = *sp;
	cp->car = args->car;
	args = args->cdr;
	while (args->id == _CELL) {
		cp->cdr = newcell(); ec;
		cp = cp->cdr;
		cp->car = args->car;
		args = args->cdr;
	}
	cp->cdr = (CELLP)nil;
	return *sp--;
}

// 連想リストを検索する
CELLP assoc_f(CELLP args)
{
	CELLP result;

	if (args->id != _CELL || args->cdr->id != _CELL) {
		return error(NEA);
	}
	result = assoc(args->car, args->cdr->car); ec;
	if (!result) {
		return (CELLP)nil;
	}
	return result;
}

CELLP rplaca_f(CELLP args)
{
	CELLP cp;

	if (args->id != _CELL || args->cdr->id != _CELL) {
		return error(NEA);
	}
	if ((cp = args->car)->id != _CELL) {
		return error(IAL);
	}
	cp->car = args->cdr->car;
	return cp;
}

CELLP rplacd_f(CELLP args)
{
	CELLP cp;

	if (args->id != _CELL || args->cdr->id != _CELL) {
		return error(NEA);
	}
	if ((cp = args->car)->id != _CELL) {
		return error(IAL);
	}
	cp->cdr = args->cdr->car;
	return cp;
}

CELLP length_f(CELLP args)
{
	long i = 0;
	NUMP np;

	if (args->id != _CELL) {
		return error(NEA);
	}
	if (args->car->id != _CELL & args->car != (CELLP)nil) {
		return error(IAL);
	}
	for (args = args->car; args->id == _CELL; args = args->cdr) {
		++i;
	}
	np = newnum(); ec;
	np->value.fix = i;
	return (CELLP)np;
}
