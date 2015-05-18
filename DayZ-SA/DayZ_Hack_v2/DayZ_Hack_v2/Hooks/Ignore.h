#ifndef IGNORE_H
#define IGNORE_H

#define MAX_PROTECTED_PIDS 32

void add_protected_pid(unsigned long pid);
int is_protected_pid(unsigned long pid);

int is_ignored_file_ascii(const char *fname, int length);
int is_ignored_file_unicode(const wchar_t *fname, int length);
int is_ignored_file_objattr(const OBJECT_ATTRIBUTES *obj);

void ignore_file_prepend_stuff(const OBJECT_ATTRIBUTES *obj,
        wchar_t **str, unsigned int *length);

int is_ignored_process();

int is_ignored_retaddr(unsigned int addr);

#endif
