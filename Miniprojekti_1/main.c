#include <stdio.h>
#include <stdlib.h>

#include "reverse.h"

int main(int argc, char *argv[])
{
    FILE *in = NULL, *out = NULL;
    
    assign_fds(&in, &out, argc, argv);

    struct line* head = read_to_list(in);
    write_list(head, out);
    free_list(head);

    if (fclose(in) != 0)
        file_close_err("input");
       
    if (fclose(out) != 0)
        file_close_err("output");
    
    return 0;
}
