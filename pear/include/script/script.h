#ifndef PEAR_SCRIPT_H_
#define PEAR_SCRIPT_H_

typedef struct script_t script_t;

script_t* script_new(const char* script_str);
script_t* script_new_from_file(const char* filename);
void script_delete(script_t* script);

#endif
