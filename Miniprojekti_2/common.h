#ifndef COMMON_H
#define COMMON_H

#define file_open_err(program_name) do {fprintf(stderr, "%s: cannot open file\n", program_name); exit(1);} while(0);
#define file_close_err(program_name) do {fprintf(stderr, "%s: failed to close file\n", program_name); exit(1);} while(0);
#define file_write_err(program_name) do {fprintf(stderr, "%s: failed to write to file\n", program_name); exit(1);} while(0);
#define malloc_err(program_name) do {fprintf(stderr, "%s: memory allocation failed\n", program_name); exit(1);} while(0);

#endif