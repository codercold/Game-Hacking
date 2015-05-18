#ifndef MISC_H
#define MISC_H

#include "stdafx.h"

ULONG_PTR parent_process_id(); // By Napalm @ NetCore2K (rohitab.com)
DWORD pid_from_process_handle(HANDLE process_handle);
DWORD pid_from_thread_handle(HANDLE thread_handle);
DWORD random();
DWORD randint(DWORD min, DWORD max);
BOOL is_directory_objattr(const OBJECT_ATTRIBUTES *obj);
void hide_module_from_peb(HMODULE module_handle);

uint32_t path_from_handle(HANDLE handle,
    wchar_t *path, uint32_t path_buffer_len);

uint32_t path_from_object_attributes(const OBJECT_ATTRIBUTES *obj,
    wchar_t *path, uint32_t buffer_length);

int ensure_absolute_path(wchar_t *out, const wchar_t *in, int length);

// imported but for some doesn't show up when #including string.h etc
//int wcsnicmp(const wchar_t *a, const wchar_t *b, int len);
//int wcsicmp(const wchar_t *a, const wchar_t *b);

// Define MAX_PATH plus tolerance for windows "tolerance"
#define MAX_PATH_PLUS_TOLERANCE MAX_PATH + 64

#endif