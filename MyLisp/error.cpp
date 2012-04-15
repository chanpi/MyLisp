#include "lisp.h"
#include "print.h"
#include "error.h"

static STR error_message[] = {
	(STR)"I don't know what happend",
	(STR)"String area used up",
	(STR)"Number area used up",
	(STR)"Atom area used up",
	(STR)"Cell area used up",
	(STR)"Unidentified List Object",
	(STR)"Pseudo S expression",			// Pseudo = ‹U‚Ì
	(STR)"Ctrl character sneaks in",
	(STR)"Undefined function",
	(STR)"Illegal function form",
	(STR)"Not enough arguments",
	(STR)"Illegal argument -- Atom required",
	(STR)"Illegal argument--Number required",
	(STR)"Illegal argument--List required",
	(STR)"Illegal argument--Atom or List required",
	(STR)"Illegal argument--Atom or Number required",
	(STR)"Illegal argument--List or Number required",
	(STR)"Illegal argument--Fix num required",
	(STR)"Illegal argument--Float num required",
	(STR)"Illegal structure (Illegal form)",
	(STR)"Illegal A-list",
	(STR)"Illegal property list",
	(STR)"Environment is an Atom",
	(STR)"Environment has an Atom",
	(STR)"Condition Clause must be a List",
	(STR)"Unexpected EOF",
	(STR)"Cannot Change Constant value",
	(STR)"Error undefined",
	(STR)"Softwear stack used up",
	(STR)"No such go-label",
	(STR)"Return Without Prog",
	(STR)"Throw Tag must be an Atom",
	(STR)"Throw Without Catch",
	(STR)"Illegal Local Variable List",
	(STR)"File Not Available",
	(STR)"Illegal File Descriptor",
	(STR)"File is not opend",
	(STR)"Too many Files",
	(STR)"File is Read Only",
	(STR)"File is Write Only",
	(STR)"Cannot Close File",
};

CELLP error(int code)
{
	err = ERR;
	err_no = code;
	return NULL;
}

void print_error(CELLP form)
{
	fprintf(stderr, "\nOops!\n");;
	fprintf(stderr, "Error No.%d : %s\n", err_no, error_message[err_no]);
	fprintf(stderr, "At ");
	if ((ATOMP)form == nil) {
		fprintf(stderr, "toplevel");
	} else {
		cur_fpo = stderr;
		print_s(form, ESCON);
	}
	fprintf(stderr, "\n\n");
	err = ERROK;
}
