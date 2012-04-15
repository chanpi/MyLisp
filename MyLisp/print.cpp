#include "print.h"
#include <ctype.h>
#include "read.h"
#include "error.h"

static void putstr(int mode, STR tp);   // シンボルの印字名を表示

void print_s(CELLP cp, int mode)        // S式の表示
{
    if (cp->id != _CELL) {
        print_atom(cp, mode);
    } else {
        fputc('(', cur_fpo);
        for (;;) {
            print_s(cp->car, mode);
            cp = cp->cdr;
            if (cp->id != _CELL) {
                break;
            }
            fputc(' ', cur_fpo);
        }
        if (cp != (CELLP)nil) {
            fprintf(cur_fpo, " . ");
            print_atom(cp, mode);
        }
        fputc(')', cur_fpo);
    }
}
void print_atom(CELLP cp, int mode)     // アトムの表示
{
    switch (cp->id) {
        case _FIX:
            fprintf(cur_fpo, "%ld", ((NUMP)cp)->value.fix);
            break;
        case _FLT:
            fprintf(cur_fpo, "%#.6g", ((NUMP)cp)->value.flt);
            break;
        case _ATOM:
            putstr(mode, ((ATOMP)cp)->name);
            break;
        default:
            error(ULO);
    }
}

static void putstr(int mode, STR tp)
{
    if (mode == ESCOFF) {
        fprintf(cur_fpo, "%s", tp);
    } else if (*tp == '\0') {
        fprintf(cur_fpo, "||");		// ヌル文字列を表す
    } else {
        if (num(tp)) {	// read.h
			fputc('\\', cur_fpo);	// 数値ではなく数字の場合は見分けるために'\\'を接頭辞としてつける
        }
		do {
			if (iskanji(*tp) && iskanji2(*(tp+1)))
			{
				fputc(*tp++, cur_fpo);
				fputc(*tp++, cur_fpo);
			}
			else if (!isprkana(*tp))
			{
				fprintf(cur_fpo, "#\\%03d", *tp++);
			}
			else
			{
				if (isesc(*tp))
				{
					fputc('\\', cur_fpo);
				}
				fputc(*tp++, cur_fpo);
			}
		} while (*tp != '\0');
    }
}
