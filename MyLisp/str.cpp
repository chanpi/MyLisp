#include "str.h"
#include "error.h"
#include "read.h"
#include "gbc.h"
#include <ctype.h>
#include <string.h>

static bool strappend(STR str1, STR str2);

CELLP ascii_f(CELLP arg)
{
	char tp[2];
	CELLP cp;
	
	if (arg->id != _CELL) {
		return error(NEA);
	}
	// シンボル -> 数値アトム
	if (arg->car->id == _ATOM) {
		*tp = *(((ATOMP)(arg->car))->name);
		cp = (CELLP)newnum(); ec;
		((NUMP)cp)->value.fix = (long)*tp;
		return cp;

	// 数値アトム -> シンボル
	} else if (arg->car->id == _FIX) {
		*tp = (char)(((NUMP)(arg->car))->value.fix);
		tp[1] = '\0';
		if ((cp = (CELLP)old_atom((STR)tp)) == NULL) {
			cp = (CELLP)make_atom((STR)tp);
		}
		return cp;
	}
	return error(IAAN);
}

CELLP implode_f(CELLP args)
{
	uchar namebuf[NAMELEN] = {0};
	ATOMP ap;

	if (args->id != _CELL) {
		return error(NEA);
	}
	if ((args = args->car)->id != _CELL) {
		return error(IAL);
	}
	*namebuf = '\0';
	while (args != (CELLP)nil) {
		if (args->car->id != _ATOM) {
			return error(IAA);
		}
		if (strappend(namebuf, ((ATOMP)(args->car))->name)) {
			break;
		}
		args = args->cdr;
	}
	if ((ap = old_atom(namebuf)) == NULL) {
		return (CELLP)make_atom(namebuf);
	}
	return (CELLP)ap;
}

CELLP explode_f(CELLP arg)
{
	char name[3] = {0}, str[50] = {0};
	STR s = (STR)str;
	STR _name;
	ATOMP ap;
	CELLP cp1, cp2;

	if (arg->id != _CELL) {
		return error(NEA);
	}
	switch ((arg = arg->car)->id) {
	case _CELL:
		return error(IAAN);
	case _ATOM:
		s = ((ATOMP)arg)->name;
		break;
	case _FIX:
		sprintf(str, "%ld", ((NUMP)arg)->value.fix);
		break;
	case _FLT:
		sprintf(str, "%g", ((NUMP)arg)->value.flt);
		break;
	}
	stackcheck;
	*(++sp) = cp1 = newcell(); ec;
	for (;;) {
		_name = (STR)name;
		if (iskanji(*s)) {
			*_name++ = *s++;
		}
		*_name++ = *s++;
		*_name = '\0';
		if ((ap = old_atom((STR)name)) == NULL) {
			ap = make_atom((STR)name);
		}
		if (*s != '\0') {
			cp2 = newcell(); ec;
			cp1->car = (CELLP)ap;
			cp1->cdr = cp2;
			cp1 = cp2;
			continue;
		}
		cp1->car = (CELLP)ap;
		return *(sp--);
	}
}

CELLP alen_f(CELLP arg)
{
	NUMP np;

	if (arg->id != _CELL) {
		return error(NEA);
	}
	if ((arg = arg->car)->id != _ATOM) {
		return error(IAA);
	}
	np = newnum(); ec;
	np->value.fix = (long)strlen((char*)((ATOMP)arg)->name);
	return (CELLP)np;
}

static bool strappend(STR str1, STR str2)
{
	STR base = str1;

	while (*str1 != '\0') {
		str1++;
	}
	while (*str2 != '\0') {
		*str1++ = *str2++;
		if (str1 - base == NAMELEN - 1) {
			*str1 = '\0';
			return TRUE;
		}
	}
	*str1 = '\0';
	return FALSE;
}