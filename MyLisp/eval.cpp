#include "eval.h"
#include "error.h"
#include "gbc.h"

static int eval_arg_p(ATOMP func);				// 関数の引数を評価するか否かを決定する
static CELLP evallist(CELLP args, CELLP env);	// 関数の引数のリストを作る
static CELLP atomvalue(ATOMP ap, CELLP env);	// シンボルの値を取り出す
static CELLP apply(CELLP func, CELLP args, CELLP env);	// 関数の評価
static CELLP push(CELLP key, CELLP value, CELLP env);	// 環境リストに新しい項を付け加える

CELLP eval(CELLP form, CELLP env)
{
	CELLP cp = (CELLP)nil;
	ATOMP func;

	switch (form->id) {
	case _ATOM:
		cp = atomvalue((ATOMP)form, env);
		break;
	case _FIX:
	case _FLT:
		return form;
	case _CELL:		// 評価時に大抵(FUNC x)みたいな感じでくる。carはfunc名、cdrは引数。
		stackcheck;
		++sp;

		func = (ATOMP)form->car;
		if (eval_arg_p(func)) {	// 引数を先に評価しておく必要があるか
			*sp = evallist(form->cdr, env);	// 先に評価されたリストを代入。引数リストはスタックに退避
			if (err) {	// 引数が不十分とか？
				break;
			}
		} else {
			*sp = form->cdr;	// 引数リストはスタックに退避
		}

		// 評価
		cp = apply((CELLP)func, *sp, env);
		sp--;
		//if (cp->car->id == _ATOM) {
		//	printf("Yeeeeeeeeeees1 %s\n", ((ATOMP)cp->car)->name);
		//	printf("%s\n", ((ATOMP)((ATOMP)cp->car)->fptr->car)->name);
		//}
		break;

	default:
		error(ULO);
		break;
	}
	if (err == ERR) {
		print_error(form);
		return NULL;
	}
	return cp;
}

// 関数の引数を評価するか否かを決定する
static int eval_arg_p(ATOMP func)
{
	if (func->id == _ATOM && func->ftype & _EA) {
		return TRUE;
	}
	if (func->id == _CELL && ((CELLP)func)->car == (CELLP)lambda) {
		return TRUE;
	}
	return FALSE;
}

// 関数の引数のリストを作る（引数で与えられた環境下でargsを評価しリストにして返す）
static CELLP evallist(CELLP args, CELLP env)
{
	CELLP cp1;

	if (args->id != _CELL) {
		return (CELLP)nil;
	}
	stackcheck;
	*++sp = newcell(); ec;
	cp1 = *sp;
	cp1->car = eval(args->car, env); ec;
	args = args->cdr;
	while (args->id == _CELL) {
		cp1->cdr = newcell(); ec;
		cp1 = cp1->cdr;
		cp1->car = eval(args->car, env); ec;
		args = args->cdr;
	}
	cp1->cdr = (CELLP)nil;
	return *sp--;
}

// シンボルの値を取り出す
static CELLP atomvalue(ATOMP ap, CELLP env)
{
	// env ((a . 1) (b . 2)) みたいなもの
	while (env->id == _CELL) {
		if (env->car->id != _CELL) {
			return error(EHA);
		}
		if (env->car->car == (CELLP)ap) {
			return env->car->cdr;
		}
		env = env->cdr;
	}
	return ap->value;	// 環境リストに見つからなかったため、ATOM自身のvalueメンバの値が戻り値となる
}

// 関数の評価
static CELLP apply(CELLP func, CELLP args, CELLP env)
{
	CELLP bodies, result = (CELLP)nil;
	CELLP (*funcp1)(CELLP args), (*funcp2)(CELLP args, CELLP env);
	char functype;

	switch (func->id) {
	case _ATOM:
		functype = ((ATOMP)func)->ftype;
		// funcがATOMってどんなときだ？ ->Cで実装されておりfptrにその関数定義へのポインタが格納されている
		// funcがATOMってどんなときだ？ ->Lispで実装されておりfptrにその関数定義(lambda式)へのポインタが格納されている
		if (functype & _UD) {
			return error(UDF);
		}
		if (functype & _SR) {
			if (functype & _EA) {
				funcp1 = (CELLP (*)(CELLP))((ATOMP)func)->fptr;
				return (*funcp1)(args);
			} else {
				funcp2 = (CELLP (*)(CELLP,CELLP))((ATOMP)func)->fptr;
				return (*funcp2)(args, env);
			}
		}
		func = ((ATOMP)func)->fptr;
	case _CELL:		// lambda式
		if (func->cdr->id != _CELL) {	// 仮引数リスト
			return error(IFF);
		}
		if (func->car == (CELLP)lambda) {
			bodies = func->cdr->cdr;
			stackcheck;
			*++sp = bind(func->cdr->car, args, env); ec;
			// implicitなprognの処理
			for (; bodies->id == _CELL; bodies = bodies->cdr) {
				result = eval(bodies->car, *sp);		// prognは最後のS式の評価結果を返すので、ひたすら結果を上書きしている。
				ec;
			}
			sp--;
			return result;
		}
		break;
	default:
		return error(IFF);
	}
	return result;
}

// 環境に対して変数の束縛を行う
CELLP bind(CELLP keys, CELLP values, CELLP env)
{
	if (keys != (CELLP)nil && keys->id == _ATOM) {	// 環境変数が１組だけ
		env = push(keys, values, env); ec;
		return env;
	}
	stackcheck;
	*++sp = env;
	while (keys->id == _CELL) {
		if (values->id != _CELL) {
			return error(NEA);
		}
		*sp = push(keys->car, values->car, *sp); ec;
		keys = keys->cdr;
		values = values->cdr;
	}
	if (keys != (CELLP)nil && keys->id == _ATOM) {	// 仮引数(x . y)　実引数(a b c) => x = a, y = (b c)
		*sp = push(keys, values, *sp); ec;
	}
	return *sp--;
}

// 環境リストに新しい項を付け加える（先頭の(key . value)の形でcons）
static CELLP push(CELLP key, CELLP value, CELLP env)
{
	stackcheck;
	*++sp = newcell(); ec;
	(*sp)->cdr = env;
	env = *sp;
	env->car = newcell(); ec;
	env->car->car = key;
	env->car->cdr = value;
	return *sp--;
}

