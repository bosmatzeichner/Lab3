#include "toy_stdio.h"


int main(int argc, char *argv[]) {
    toy_printf("Non-padded string: %s\n", "str");
    toy_printf("Right-padded string: %6s\n", "str");
    toy_printf("Left-added string: %-6s\n", "str");
    toy_printf("With numeric placeholders: %05d\n", -1);




/*toy_printf("With numeric placeholders: %05s\n", "str");
toy_printf("Left-added string: %-10s\n", "str");
toy_printf("Right-padded string: %10s\n", "str");
toy_printf("With numeric placeholders: %03s\n", "str");
toy_printf("Right-padded num: %-3d\n", 20);
*/
}
