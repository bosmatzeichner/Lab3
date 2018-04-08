/* toy-io.c
 * Limited versions of printf and scanf
 *
 * Programmer: Mayer Goldberg, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* the states in the printf state-machine */
enum printf_state {
    st_printf_init,
    st_printf_percent
};
typedef struct state_args {
    char* fs;
    char currChar;
    int array;
    enum printf_state state;
    int width;
} state_args;
#define MAX_NUMBER_LENGTH 64
#define is_octal_char(ch) ('0' <= (ch) && (ch) <= '7')
int toy_printf(char *fs, ...);
const char *digit = "0123456789abcdef";
const char *DIGIT = "0123456789ABCDEF";
unsigned int unsigned_decimal;
int array,i, array_length, input_size,toAdd, printed, ind, width=0, left_blanks=0, blanks=0, placeholders;
int* int_array;
char* char_array;
char** string_array;

int print_int_helper(int n, int radix, const char *digit) {
    int result;

    if (n < radix) {
        putchar(digit[n]);
        return 1;
    }
    else {
        result = print_int_helper(n / radix, radix, digit);
        putchar(digit[n % radix]);
        return 1 + result;
    }
}
int print_unsigned_int_helper(unsigned int n, int radix, const char *digit) {
    int result;

    if (n < radix) {
        putchar(digit[n]);
        return 1;
    }
    else {
        result = print_int_helper(n / radix, radix, digit);
        putchar(digit[n % radix]);
        return 1 + result;
    }
}
int print_int(int n, int radix, const char * digit) {
    if (radix < 2 || radix > 16) {
        toy_printf("Radix must be in [2..16]: Not %d\n", radix);
        exit(-1);
    }

    if (n > 0) {
        return print_int_helper(n, radix, digit);
    }
    if (n == 0) {
        putchar('0');
        return 1;
    }
    else if(radix != 10){
        return print_unsigned_int_helper(n, radix, digit);
    }
    else if(unsigned_decimal==1){
        unsigned_decimal = (unsigned int) n;
        return print_unsigned_int_helper(unsigned_decimal, radix, digit);
    }
    else {
        putchar('-');
        return 1 + print_int_helper(-n, radix, digit);
    }
}
int print_array(int* n, int size, int radix, const char * digit) {
    int num=0;
    int element;
    array=0;
    if (radix < 2 || radix > 16) {
        toy_printf("Radix must be in [2..16]: Not %d\n", radix);
        exit(-1);
    }
    ++num;
    putchar('{');
    for (i = 0; i < size-1; i++){
        element = n[i];
        num +=print_int(element,radix,digit);
        printf(", ");
        num+=2;
    }
    print_int(n[i],radix,digit);
    putchar('}');
    ++num;
    return num;

}
int print_string(char* string_value){
    while(*string_value){
        printed++;
        putchar(*string_value);
        string_value++;
    }
    return printed;
}
int print_width(int n, int radix, const char * digit){
    int result;
    int x=n;
    int count=1;
    ind=0;


    if (n<0){
        x=-n;
        count++;
    }
    while(x>9){
        count ++;
        x= x/10;
    }
    result=count;


    if (placeholders && count<blanks){
        result=blanks;
        if (n<0){
            putchar('-');
            n=-n;
        }
        for(ind=0; ind<blanks-count; ind++){
            putchar('0');
        }

    }

    if (left_blanks&&!placeholders && count<blanks){
        result=blanks;
        for(ind=0; ind< blanks-count; ind++){
            putchar(' ');
        }
    }
    print_int(n, 10, digit);
    if(!left_blanks&& !placeholders && count<blanks){
        result=blanks;
        for(ind=0; ind<blanks-count; ind++){
            putchar(' ');
        }
        putchar('#');
    }
    blanks=0;
    left_blanks=0;
    placeholders=0;
    count=0;
    width=0;
    return result;
}
int handle_default(va_list args,state_args* state) {
    return 1;
}
int handle_precent(va_list args,state_args* state) {
    if (state->state == st_printf_percent){
        putchar('%');
        state->state = st_printf_init;
        return 1;
    }
    else{
        state->state = st_printf_percent;
        return 0;
    }
}
int handle_print_d(va_list args,state_args* state){
    if (state->array){
        int *int_array = va_arg(args, int*);
        int input_size = va_arg(args, int);
        return print_array(int_array, input_size, 10, digit);
    }
    else{
        int int_value = va_arg(args, int);
        if (state->width){
            return print_width(int_value, 10, digit);
        }
        else{
            return print_int(int_value, 10, digit);
        }
    }
}
int handle_print_u(va_list args,state_args* state) {
    unsigned_decimal = 1;
    if (array) {
        int_array = va_arg(args, int*);
        input_size = va_arg(args, int);
        return print_array(int_array, input_size, 10, digit);
    } else {
        int int_value = va_arg(args, int);
        return print_int(int_value, 10, digit);
    }
 }
int handle_print_b(va_list args,state_args* state){
    if (state->array){
        int_array = va_arg(args, int*);
        input_size = va_arg(args, int);
        return print_array(int_array, input_size, 10, digit);
    }
    else{
        int int_value = va_arg(args, int);
        return print_int(int_value, 2, digit);
    }
}
int handle_print_o(va_list args,state_args* state){}
int handle_print_x(va_list args,state_args* state){}
int handle_print_X(va_list args,state_args* state){}
int handle_print_c(va_list args,state_args* state){}
int handle_print_s(va_list args,state_args* state){}
int handle_print_A(va_list args,state_args* state){}
int handle_print_w(va_list args,state_args* state){}
enum printf_state init_precentState_handler(va_list args, int* out_printed_chars, state_args* state) {
  switch (*(state->fs)) {
            case '%':
                out_printed_chars = &handle_precent;
                return st_printf_init;
            case 'd':
                out_printed_chars = &handle_print_d;
                return st_printf_init;
            case 'u':
                out_printed_chars = &handle_print_u;
                return st_printf_init;
            case 'b':
                out_printed_chars = &handle_print_b;
                return st_printf_init;
            case 'o':
                out_printed_chars = &handle_print_o;
                return st_printf_init;
            case 'x':
                out_printed_chars = &handle_print_x;
                return st_printf_init;
            case 'X':
                out_printed_chars = &handle_print_X;
                return st_printf_init;
            case 's':
                out_printed_chars = &handle_print_s;
                return st_printf_init;
            case 'c':
                out_printed_chars = &handle_print_c;
                return st_printf_init;
            case 'A':
                out_printed_chars = &handle_print_A;
                return st_printf_percent;
            case 49 ... 57:
                out_printed_chars = &handle_print_w;
                return st_printf_percent;
            case '-':
                out_printed_chars = &handle_print_w;
                return st_printf_percent;
            case '0':
                out_printed_chars = &handle_print_w;
                return st_printf_percent;
        }
    }
enum printf_state init_printfState_handler(va_list args, int* out_printed_chars, state_args* state) {
        switch (*(state->fs)) {
            case '%':
                out_printed_chars = &handle_precent;
                return st_printf_percent;
            default:
                out_printed_chars = &handle_default;
                return st_printf_init;

}

/* SUPPORTED:
 *   %b, %d, %o, %x, %X
 *   and nowww also %u--
 *     integers in binary, decimal, octal, hex, and HEX
 *   %s -- strings
 */

int toy_printf(char *fs, ...) {
    int chars_printed= 0;
    va_list args;
    enum printf_state state;
    //int (*handler_printed_chars)(int);
    //int (*handler_printed_chars)(va_list args,state_args* state);
    int handler_printed_chars;
    struct state_args * sa = malloc(sizeof(*sa));
    sa->fs = fs;
    va_start(args, fs);
    state = st_printf_init;
    sa->state = state;
    for (;  (sa->currChar=*fs) != '\0'; ++fs) {
        switch (sa->state) {
            case st_printf_init:
                state = init_printfState_handler(args, &handler_printed_chars, sa);
                chars_printed += handler_printed_chars;
                break;
            case st_printf_percent:
                state = init_precentState_handler(args, &handler_printed_chars, sa);
                chars_printed += handler_printed_chars;
                break;
            default:
                toy_printf("toy_printf: Unknown state -- %d\n", (int)state);
                exit(-1);
        }
    }
    va_end(args);
    return chars_printed;
}



