#include "control.h"
#include "error.h"
#include "eval.h"
#include "functions.h"
#include "gbc.h"

extern void __cdecl quit(int);
static CELLP setenv(ATOMP var, CELLP val, CELLP env);

// ðŒŽ®
// (cond ((= x 3) (* x x))
//		 (t ()))
CELLP cond_f(CELLP clauses, CELLP env)	// clauses €Að€
{
	CELLP key, bodies, result;

	if (clauses->id != _CELL) {
		return error(NEA);		// ˆø”‚ª1‚Â‚à‚È‚¢
	}
	while (clauses->id == _CELL) {
		if (clauses->car->id != _CELL) {
			return error(CCL);
		}
		key = eval(clauses->car->car, env); ec;	// key = ðŒ•”‚ð•]‰¿(eval)‚µ‚½Œ‹‰Ê
		// ‚±‚ÌðŒ‚É‡‚¤‚Ì‚ÅA–{‘Ì(bodies)‚ð•]‰¿‚µ‚ÄŒ‹‰Ê‚ð•Ô‚·
		if (key != (CELLP)nil) {
			if ((bodies = clauses->car->cdr)->id != _CELL) {	// –{‘Ì‚ª‚È‚¢ê‡‚ÍðŒ•”‚Ì•]‰¿Œ‹‰Ê‚ð•Ô‚·
				return key;
			}
			while (bodies->id == _CELL) {
				result = eval(bodies->car, env); ec;
				bodies = bodies->cdr;
			}
			return result;
		}
		clauses = clauses->cdr;
	}
	return (CELLP)nil;
}

// (setf x 3) -> args = (x 3)
CELLP setf_f(CELLP args, CELLP env)
{
	CELLP val, result;
	ATOMP var;

	while (args->id == _CELL) {
		if (args->cdr->id != _CELL) {
			return error(NEA);
		}
		var = (ATOMP)args->car;
		val = eval(args->cdr->car, env); ec;
		if (var->id != _ATOM) {
			return error(IAA);
		}
		if (var == nil || var == t || var == eofread) {
			return error(CCC);
		}
		result = setenv(var, val, env); ec;
		if (result == NULL) {
			var->value = val;
		}
		args = args->cdr->cdr;
	}
	return val;
}

static CELLP setenv(ATOMP var, CELLP val, CELLP env)
{
	while (env->id == _CELL) {
		if (env->car->id != _CELL) {
			return error(EHA);
		}
		if (env->car->car == (CELLP)var) {
			env->car->cdr = val;	// ã‘‚«
			return env;
		}
		env = env->cdr;
	}
	return NULL;
}

CELLP quote_f(CELLP args, CELLP env)
{
	if (args->id != _CELL) {
		return error(NEA);
	}
	return args->car;
}

// (defun hoge (str) (print str))
CELLP defun_f(CELLP args, CELLP env)
{
	CELLP val;
	ATOMP func;

	if (args->id != _CELL || args->cdr->id != _CELL) {
		return error(NEA);
	}
	if ((func = (ATOMP)args->car)->id != _ATOM) {	// ŠÖ”–¼FATOM
		return error(IAA);
	}
	//printf("--> (%s ...)\n", ((ATOMP)args->car)->name/*, ((ATOMP)args->cdr->car)->name*/);
	val = cons((CELLP)lambda, args->cdr); ec;	// (lambda (str) (print str))
	func->ftype = _EXPR;
	func->fptr = val;
	return (CELLP)func;
}

CELLP oblist_f(void)
{
	int i = 0;
	CELLP cp1, cp2, cp3;
	
	stackcheck;
	cp1 = *++sp = newcell(); ec;
	for (i = 0; i < TABLESIZE; i++) {
		if ((cp2 = oblist[i]) != (CELLP)nil) {	// nilˆÈŠO‚ªŒ©‚Â‚©‚é‚Ü‚Å‘{¸
			break;
		}
	}
	for (; cp2->cdr != (CELLP)nil; cp2 = cp2->cdr) {
		cp1->car = cp2->car;
		cp3 = newcell(); ec;
		cp1->cdr = cp3;
		cp1 = cp3;
	}
	cp1->car = cp2->car;
	for (++i; i < TABLESIZE; ++i) {
		for (cp2 = oblist[i]; cp2 != (CELLP)nil; cp2 = cp2->cdr) {
			cp3 = newcell(); ec;
			cp1->cdr = cp3;
			cp1 = cp3;
			cp1->car = cp2->car;
		}
	}
	return *sp--;
}

CELLP quit_f(void)
{
	quit(0);
	return (CELLP)nil;
}

CELLP reclaim_f(void)
{
	gbc(ON, ON); ec;
	return (CELLP)nil;
}

CELLP verbos_f(CELLP arg)
{
	if (arg == (CELLP)nil) {
		if (verbos) {
			return (CELLP)t;
		} else {
			return (CELLP)nil;
		}
	}
	if (arg->car == (CELLP)nil) {
		verbos = OFF;
		return (CELLP)nil;
	} else {
		verbos = ON;
		return (CELLP)t;
	}
}

CELLP and_f(CELLP args, CELLP env)
{
	CELLP result;

	result = (CELLP)t;
	// nil‚ªo‚Ä‚­‚é‚Ü‚Åbody‚ð‡‚É•]‰¿‚·‚é
	while (args->id == _CELL) {
		result = eval(args->car, env); ec;
		if (result == (CELLP)nil) {
			return (CELLP)nil;
		}
		args = args->cdr;
	}
	return result;
}

CELLP or_f(CELLP args, CELLP env)
{
	CELLP result;

	// nil‚Å‚È‚¢‚à‚Ì‚ªo‚Ä‚­‚é‚Ü‚Åbody‚ð‡‚É•]‰¿‚·‚é
	while (args->id == _CELL) {
		result = eval(args->car, env); ec;
		if (result != (CELLP)nil) {
			return result;
		}
		args = args->cdr;
	}
	return (CELLP)nil;
}

// (psetq x 1 y 2 z 3)
CELLP psetq_f(CELLP args, CELLP env)
{
	CELLP val, cp, *cur_sp, *sp2;
	ATOMP var;

	cur_sp = sp;
	// set‚·‚é’l‚ð‚·‚×‚Äæ‚É•]‰¿‚µAstack‚ÉÏ‚Þ
	for (cp = args; cp->id == _CELL; cp = cp->cdr->cdr) {
		if (cp->cdr->id != _CELL) {
			return error(NEA);
		}
		stackcheck;
		*++sp = eval(cp->cdr->car, env); ec;
	}
	val = *sp;
	sp2 = cur_sp + 1;
	while (args->id == _CELL) {
		var = (ATOMP)args->car;
		if (var->id != _ATOM) {
			return error(IAA);
		}
		// ’l‚ð•Ï‚¦‚Ä‚Í‚¢‚¯‚È‚¢ƒVƒ“ƒ{ƒ‹
		if (var == nil || var == t || var == eofread) {
			return error(CCC);
		}
		cp = setenv(var, *sp2, env); ec;
		if (cp == NULL) {
			var->value == *sp2;
		}
		sp2++;
		args = args->cdr->cdr;
	}
	sp = cur_sp;
	return val;
}

CELLP set_f(CELLP args, CELLP env)
{
	CELLP val, result;
	ATOMP var;

	while (args->id == _CELL) {
		if (args->cdr->id != _CELL) {
			return error(NEA);
		}
		var = (ATOMP)eval(args->car, env); ec;
		val = eval(args->cdr->car, env); ec;
		if (var->id != _ATOM) {
			return error(IAA);
		}
		if (var == nil || var == t || var == eofread) {
			return error(CCC);
		}
		result = setenv(var, val, env); ec;
		if (result == NULL) {
			var->value = val;
		}
		args = args->cdr->cdr;
	}
	return val;
}
