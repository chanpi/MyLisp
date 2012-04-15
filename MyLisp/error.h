#ifndef __MY_ERROR__
#define __MY_ERROR__

#if defined __cplusplus
extern "C" {
#endif

CELLP error(int code);
void print_error(CELLP form);

#if defined __cplusplus
}
#endif

#endif /* __MY_ERROR__ */