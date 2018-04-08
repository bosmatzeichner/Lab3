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
    enum printf_state state;
    int width, array, left_blanks, blanks, placeholders;
} state_args;
struct state_result {
    int printed_chars;
    enum printf_state new_state;
    struct args* state;
};
#define MAX_NUMBER_LENGTH 64
#define is_octal_char(ch) ('0' <= (ch) && (ch) <= '7')
int toy_printf(char *fs, ...);
const char *digit = "0123456789abcdef";
const char *DIGIT = "0123456789ABCDEF";
unsigned int unsigned_decimal;

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
    if (radix < 2 || radix > 16) {
        toy_printf("Radix must be in [2..16]: Not %d\n", radix);
        exit(-1);
    }
    ++num;
    putchar('{');
    int i;
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
    int printed;
    while(*string_value){
        printed++;
        putchar(*string_value);
        string_value++;
    }
    return printed;
}
int print_width(int n, int radix, const char * digit, state_args* state){
    int result;
    int x=n;
    int count=1;
    int ind=0;
    if (n<0){
        x=-n;
        count++;
    }
    while(x>9){
        count ++;
        x= x/10;
    }
    result=count;
    if (state->placeholders && count < state->blanks){
        result=state->blanks;
        if (n<0){
            putchar('-');
            n=-n;
        }
        for(ind=0; ind<state->blanks-count; ind++){
            putchar('0');
        }
    }
    if (state->left_blanks&&!state->placeholders && count<state->blanks){
        result=state->blanks;
        for(ind=0; ind< state->blanks-count; ind++){
            putchar(' ');
        }
    }
    print_int(n, radix, digit);
    if(!state->left_blanks&& !state->placeholders && count<state->blanks){
        result=state->blanks;
        for(ind=0; ind<state->blanks-count; ind++){
            putchar(' ');
        }
        putchar('#');
    }
    state->blanks=0;
    state->left_blanks=0;
    state->placeholders=0;
    state->width=0;
    return result;
}
int handle_default1(va_list args,state_args* state) {
        toy_printf("Unhandled format %%%c...\n", *(state->fs));
        exit(-1);
}
int handle_default2(va_list args,state_args* state) {
    putchar(*state->fs);
    return 1;
}
int handle_precent1(va_list args,state_args* state) {
    putchar('%');
    return 1;

}
int handle_initPrecent(va_list args,state_args* state) {
    return 0;
}
int handle_print_d(va_list args,state_args* state){
    if (state->array){
        state->array=0;
        int *int_array = va_arg(args, int*);
        int input_size = va_arg(args, int);
        return print_array(int_array, input_size, 10, digit);
    }
    else{
        int int_value = va_arg(args, int);
        if (state->width){
            return print_width(int_value, 10, digit, state);
        }
        else{
            return print_int(int_value, 10, digit);
        }
    }
}
int handle_print_u(va_list args,state_args* state) {
    unsigned_decimal = 1;
    if (state->array) {
        state->array=0;
        int_array = va_arg(args, int*);
        int input_size = va_arg(args, int);
        return print_array(int_array, input_size, 10, digit);
    } else {
        int int_value = va_arg(args, int);
        return print_int(int_value, 10, digit);
    }
}
int handle_print_b(va_list args,state_args* state){
    if (state->array){
        state->array=0;
        int_array = va_arg(args, int*);
        int input_size = va_arg(args, int);
        return print_array(int_array, input_size, 10, digit);
    }
    else{
        int int_value = va_arg(args, int);
        return print_int(int_value, 2, digit);
    }
}
int handle_print_o(va_list args,state_args* state){
    if (state->array){
        state->array=0;
        int_array = va_arg(args, int*);
        int input_size = va_arg(args, int);
        return print_array(int_array, input_size, 10, digit);
    }
    else{
        int int_value = va_arg(args, int);
        return print_int(int_value, 8, digit);
    }
}
int handle_print_x(va_list args,state_args* state){
    if (state->array){
        state->array=0;
        int_array = va_arg(args, int*);
        int input_size = va_arg(args, int);
        return print_array(int_array, input_size, 10, digit);
    }
    else{
        int int_value = va_arg(args, int);
        return print_int(int_value, 16, digit);
    }
}
int handle_print_X(va_list args,state_args* state){
    if (state->array){
        state->array=0;
        int_array = va_arg(args, int*);
        int input_size = va_arg(args, int);
        return print_array(int_array, input_size, 10, digit);
    }
    else{
        int int_value = va_arg(args, int);
        return print_int(int_value, 16, DIGIT);
    }
}
int handle_print_s(va_list args,state_args* state){
    int chars_printed=0,count=0;
    if (state->array){
        state->array=0;
        string_array = va_arg(args, char**);
        int input_size = va_arg(args, int);
        putchar('{');
        int i=0;
        for (i = 0; i < input_size-1; i++){
            putchar('"');
            chars_printed += print_string(string_array[i]);
            putchar('"');
            putchar(',');
            putchar(' ');
        }
        putchar('"');
        chars_printed += print_string(string_array[i]);
        putchar('"');
        putchar('}');
    }
    else{
        char* string_value = va_arg(args, char *);
        int ind=0;
        while(string_value[ind]!= '\0'){
            count++;
            ind++;
        }
        chars_printed+=count;

        if (state->blanks&& state->placeholders&& count<state->blanks){
            for(ind=0; ind<state->blanks-count; ind++){
                putchar('0');
            }
            chars_printed+=state->blanks-count;
        }
        if (state->blanks && state->left_blanks&& !state->placeholders && count<state->blanks){
            for(ind=0; ind< state->blanks-count; ind++){
                putchar(' ');
            }
            chars_printed+=state->blanks-count;

        }
        print_string(string_value);

        if (!state->left_blanks && state->blanks && !state->placeholders&& count<state->blanks){
            for(ind=0; ind<state->blanks-count; ind++){
                putchar(' ');
            }
            putchar('#');
            chars_printed+=state->blanks-count+1;
        }
        state->blanks=0;
        state->left_blanks=0;
        state->placeholders=0;
        state->width=0;
    }
    return chars_printed;
}
int handle_print_c(va_list args,state_args* state){
    int chars_printed;
    if (state->array){
        state->array=0;
        char_array = va_arg(args, char*);
        int input_size = va_arg(args, int);
        putchar('{');
        chars_printed++;
        int ind;
        for(ind = 0; ind<input_size-1; ind++){
            putchar('\'');
            putchar(char_array[ind]);
            putchar('\'');
            putchar(',');
            putchar(' ');
            chars_printed+=5;
        }
        putchar('\'');
        putchar(char_array[ind]);
        putchar('\'');
        putchar('}');
        chars_printed+=4;

    }
    else{
        char char_value = (char)va_arg(args, int);
        putchar(char_value);
        ++chars_printed;
    }
    return chars_printed;
}
int handle_print_A(va_list args,state_args* state){
    state->array=1;
    return 0;
}
int handle_print_number(va_list args,state_args* state){
    state->width = 1;
    int number_of_blanks = 10*(state->blanks)+(*(state->fs)- 48);
    state->blanks =number_of_blanks;
    return 0;
}
int handle_print_minus(va_list args,state_args* state){
    state-> left_blanks = 1;
}
int handle_print_zero(va_list args,state_args* state){
    if (state->width==0)
        state->placeholders++;
    else {
        state->blanks = 10*(state->blanks);
    }
}

struct state_result* init_precentState_handler(struct state_result* sr, va_list args ) {
    int (*handler_printed_chars) (va_list ,struct state_args*);

    switch (*(((state_args*)(sr->state))->fs)) {
        case '%':
            handler_printed_chars = &handle_precent1;
            sr->new_state = st_printf_init;
            break;

        case 'd':
            handler_printed_chars = &handle_print_d;
            sr->new_state = st_printf_init;
            break;

        case 'u':
            handler_printed_chars = &handle_print_u;
            sr->new_state = st_printf_init;
            break;

        case 'b':
            handler_printed_chars = &handle_print_b;
            sr->new_state = st_printf_init;
            break;

        case 'o':
            handler_printed_chars = &handle_print_o;
            sr->new_state = st_printf_init;
            break;

        case 'x':
            handler_printed_chars = &handle_print_x;
            sr->new_state = st_printf_init;
            break;

        case 'X':
            handler_printed_chars = &handle_print_X;
            sr->new_state = st_printf_init;
            break;

        case 's':
            handler_printed_chars = &handle_print_s;
            sr->new_state = st_printf_init;
            break;

        case 'c':
            handler_printed_chars = &handle_print_c;
            sr->new_state = st_printf_init;
            break;

        case 'A':
            handler_printed_chars = &handle_print_A;
            sr->new_state = st_printf_percent;
            break;

        case 49 ... 57:
            handler_printed_chars = &handle_print_number;
            sr->new_state = st_printf_percent;
            break;

        case '-':
            handler_printed_chars = &handle_print_minus;
            sr->new_state = st_printf_percent;
            break;

        case '0':
            handler_printed_chars = &handle_print_zero;
            sr->new_state = st_printf_percent;
            break;
        default:
            handler_printed_chars = &handle_default1;
            sr->new_state = st_printf_init;

    }
    struct state_args* sa = sr->state;
    sr->printed_chars += handler_printed_chars(args,sa);
    return sr;
}
struct state_result* init_printfState_handler(struct state_result* sr, va_list args) {
    int (*handler_printed_chars) (va_list ,struct state_args*);
    switch (*(((state_args*)(sr->state))->fs)) {
        case '%':
            handler_printed_chars = &handle_initPrecent;
            sr->new_state = st_printf_percent;
            break;
        default:
            handler_printed_chars = &handle_default2;
            sr->new_state = st_printf_init;
    }
    struct state_args* sa = sr->state;
    sr->printed_chars += handler_printed_chars(args,sa);
    return sr;
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
    struct state_result *sr= malloc(sizeof(*sr));
    sr->state = malloc(sizeof(struct state_args));
    struct state_args *sa = sr->state;
    sa->fs = fs;
    va_start(args, fs);
    sa->state = st_printf_init;;
    for (;  *sa->fs != '\0'; ++(sa->fs)) {
        switch (sr->new_state) {
            case st_printf_init:
                sr = init_printfState_handler(sr, args);
                break;
            case st_printf_percent:
                sr = init_precentState_handler(sr, args);
                break;
            default:
                toy_printf("toy_printf: Unknown state -- %d\n", (int)sa->state);
                exit(-1);
        }
    }
    va_end(args);
    return  sr->printed_chars;
}



