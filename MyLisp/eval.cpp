#include "eval.h"
#include "error.h"
#include "gbc.h"

static int eval_arg_p(ATOMP func);				// �֐��̈�����]�����邩�ۂ������肷��
static CELLP evallist(CELLP args, CELLP env);	// �֐��̈����̃��X�g�����
static CELLP atomvalue(ATOMP ap, CELLP env);	// �V���{���̒l�����o��
static CELLP apply(CELLP func, CELLP args, CELLP env);	// �֐��̕]��
static CELLP push(CELLP key, CELLP value, CELLP env);	// �����X�g�ɐV��������t��������

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
	case _CELL:		// �]�����ɑ��(FUNC x)�݂����Ȋ����ł���Bcar��func���Acdr�͈����B
		stackcheck;
		++sp;

		func = (ATOMP)form->car;
		if (eval_arg_p(func)) {	// �������ɕ]�����Ă����K�v�����邩
			*sp = evallist(form->cdr, env);	// ��ɕ]�����ꂽ���X�g�����B�������X�g�̓X�^�b�N�ɑޔ�
			if (err) {	// �������s�\���Ƃ��H
				break;
			}
		} else {
			*sp = form->cdr;	// �������X�g�̓X�^�b�N�ɑޔ�
		}

		// �]��
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

// �֐��̈�����]�����邩�ۂ������肷��
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

// �֐��̈����̃��X�g�����i�����ŗ^����ꂽ������args��]�������X�g�ɂ��ĕԂ��j
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

// �V���{���̒l�����o��
static CELLP atomvalue(ATOMP ap, CELLP env)
{
	// env ((a . 1) (b . 2)) �݂����Ȃ���
	while (env->id == _CELL) {
		if (env->car->id != _CELL) {
			return error(EHA);
		}
		if (env->car->car == (CELLP)ap) {
			return env->car->cdr;
		}
		env = env->cdr;
	}
	return ap->value;	// �����X�g�Ɍ�����Ȃ��������߁AATOM���g��value�����o�̒l���߂�l�ƂȂ�
}

// �֐��̕]��
static CELLP apply(CELLP func, CELLP args, CELLP env)
{
	CELLP bodies, result = (CELLP)nil;
	CELLP (*funcp1)(CELLP args), (*funcp2)(CELLP args, CELLP env);
	char functype;

	switch (func->id) {
	case _ATOM:
		functype = ((ATOMP)func)->ftype;
		// func��ATOM���Ăǂ�ȂƂ����H ->C�Ŏ�������Ă���fptr�ɂ��̊֐���`�ւ̃|�C���^���i�[����Ă���
		// func��ATOM���Ăǂ�ȂƂ����H ->Lisp�Ŏ�������Ă���fptr�ɂ��̊֐���`(lambda��)�ւ̃|�C���^���i�[����Ă���
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
	case _CELL:		// lambda��
		if (func->cdr->id != _CELL) {	// ���������X�g
			return error(IFF);
		}
		if (func->car == (CELLP)lambda) {
			bodies = func->cdr->cdr;
			stackcheck;
			*++sp = bind(func->cdr->car, args, env); ec;
			// implicit��progn�̏���
			for (; bodies->id == _CELL; bodies = bodies->cdr) {
				result = eval(bodies->car, *sp);		// progn�͍Ō��S���̕]�����ʂ�Ԃ��̂ŁA�Ђ����猋�ʂ��㏑�����Ă���B
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

// ���ɑ΂��ĕϐ��̑������s��
CELLP bind(CELLP keys, CELLP values, CELLP env)
{
	if (keys != (CELLP)nil && keys->id == _ATOM) {	// ���ϐ����P�g����
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
	if (keys != (CELLP)nil && keys->id == _ATOM) {	// ������(x . y)�@������(a b c) => x = a, y = (b c)
		*sp = push(keys, values, *sp); ec;
	}
	return *sp--;
}

// �����X�g�ɐV��������t��������i�擪��(key . value)�̌`��cons�j
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

