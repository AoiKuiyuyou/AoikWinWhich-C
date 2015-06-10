//
#include "stdafx.h"
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

//
typedef char const * String;

//
typedef struct StringNode
{
    String str;
    struct StringNode* next;
} StringNode;

//
typedef struct StringList
{
    StringNode* head;
    StringNode* end;
    size_t count;
} StringList;

//
StringNode* stringnode_create(String str) {
    //
    StringNode* str_node = (StringNode*)malloc(sizeof(StringNode));

    //
    str_node->str = str;

    str_node->next = NULL;

    //
    return str_node;
}

//
StringList* stringlist_create() {
    //
    StringList* str_list = (StringList*)malloc(sizeof(StringList));

    //
    str_list->head = NULL;
    str_list->end = NULL;
    str_list->count = 0;

    //
    return str_list;
}

//
void stringlist_add_head(StringList* str_list, String str) {
    //
    StringNode* item = stringnode_create(str);

    //if strlist->end
    if (str_list->count == 0) {
        //
        assert(!str_list->head);

        assert(!str_list->end);

        //
        str_list->head = item;

        str_list->end = item;

        // new end clears "next"
        str_list->end->next = NULL;
    }
    else {
        //
        assert(str_list->head);

        assert(str_list->end);

        // new head links to old head
        item->next = str_list->head;

        // set new head
        str_list->head = item;
    }

    //
    str_list->count += 1;
}

//
void stringlist_add_end(StringList* str_list, String str) {
    //
    StringNode* item = stringnode_create(str);

    //if strlist->end
    if (str_list->count == 0) {
        //
        assert(!str_list->head);

        assert(!str_list->end);

        //
        str_list->head = item;

        str_list->end = item;

        // new end clears "next"
        str_list->end->next = NULL;
    }
    else {
        //
        assert(str_list->head);

        assert(str_list->end);

        // old end links to new end
        str_list->end->next = item;

        // set new end
        str_list->end = item;

        // new end clears "next"
        str_list->end->next = NULL;
    }

    //
    str_list->count += 1;
}

//
BOOL stringlist_contains(StringList* str_list, String str) {
    //
    StringNode* node = str_list->head;

    while (node) {
        // If two strings are equal
        if (!strcmp(node->str, str)) {
            return TRUE;
        };

        //
        node = node->next;
    }

    return FALSE;
}

//
void stringlist_uniq(
    StringList* res_list, // Result
    StringList* str_list)
{
    //
    StringNode* node = str_list->head;

    while (node) {
        //
        if (!stringlist_contains(res_list, node->str)) {
            // String "node->str" is changed ownership
            stringlist_add_end(res_list, node->str);

            // Clear old pointer
            node->str = NULL;
        };

        //
        node = node->next;
    }
}

//
void stringlist_del(StringList* str_list) {
    //
    StringNode* node = str_list->head;

    while (node) {
        //
        StringNode* next = node->next;

        //
        if (node->str) {
            //
            free((void*)node->str);

            //
            node->str = NULL;
        }

        node->next = NULL;

        free(node);

        //
        node = next;
    }

    //
    str_list->head = NULL;

    str_list->end = NULL;

    str_list->count = 0;

    //
    free(str_list);
}

//
void stringlist_del_v2(
    StringList** str_list_pp // "pp" means pointer pointer
    ) {
    //
    assert(str_list_pp);

    assert(*str_list_pp);

    //
    stringlist_del(*str_list_pp);

    // Set the "StringList pointer" to NULL, in order to avoid address violation.
    *str_list_pp = NULL;
}

//
char* string_copy_new(const char* str) {
    //
    size_t str_bytes_cnt = sizeof(char) * (strlen(str) + 1);

    //
    char* str_copy = (char *)malloc(str_bytes_cnt);

    //
    memcpy(str_copy, str, str_bytes_cnt);

    str_copy[str_bytes_cnt - 1] = '\0';

    //
    return str_copy;
}

//
void string_split(StringList* str_s, String str, char sep) {
    //
    char sep_str[2];

    sep_str[0] = sep;
    sep_str[1] = '\0';

    //
    size_t str_len = strlen(str);

    size_t start = 0;

    while (TRUE) {
        //
        String substr_start = str + start;

        // 2a1XF8a
        // Can be 0 when the first char is the sep char.
        size_t substr_cnt = strcspn(substr_start, sep_str);

        // "+ 1" for null terminator.
        //
        // "substr_cnt" at 2a1XF8a being 0 is well handled.
        size_t substr_byte_cnt = sizeof(char) * (substr_cnt + 1);

        //
        char* substr = malloc(substr_byte_cnt);

        // "substr_cnt" at 2a1XF8a being 0 is well handled.
        memcpy(substr, substr_start, substr_cnt);

        substr[substr_cnt] = '\0';

        //
        stringlist_add_end(str_s, substr);

        // "+ 1" for skipping the sep char that stops "strcspn" at 2a1XF8a.
        //
        // If what stops "strcspn" at 2a1XF8a is null char, then "start" should
        // equal (str_len + 1). This is well handled at 7fkzkrk.
        start += (substr_cnt + 1);

        // 7fkzkrk
        if (start > str_len) {
            //
            assert(start == (str_len + 1));

            //
            break;
        }
        // This is the case when the last char is the sep char
        else if (start == str_len) {
            //
            assert(str[str_len - 1] == sep);

            // Allocate an empty string
            char* empty_str = malloc(sizeof(char));

            empty_str[0] = '\0';

            // Add the last empty string.
            // E.g. string_split("a|b|", '|') -> ["a", "b", ""]
            stringlist_add_end(str_s, empty_str);

            //
            break;
        }
    }
}

//
void string_strip(char *str) {
    //
    char *end;

    // Remove heading spaces.
    //
    // Null char is well handled by "isspace".
    while (isspace(*str)) {
        str++;
    }

    //
    if (*str == '\0') {
        return;
    }

    // Remove ending spaces
    end = str + strlen(str) - 1;

    while (end > str && isspace(*end)) {
        end--;
    }

    // Set null
    *(end + 1) = '\0';
}

//
void string_tolower(char* str) {
    //
    size_t i = 0;

    for (i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

//
char* string_concat_new(const char* str, const char* end) {
    //
    int str_len = strlen(str);

    int end_len = strlen(end);

    //
    size_t bytes_cnt = sizeof(char) * (str_len + end_len + 1);

    //
    char* str_new = (char *)malloc(bytes_cnt);

    //
    memcpy(str_new, str, str_len);

    memcpy(str_new + str_len, end, end_len);

    // Set last bytes to NUL
    str_new[str_len + end_len] = '\0';

    //
    return str_new;
}

//
BOOL string_endswith(const char* str, const char* end) {
    //
    int str_len = strlen(str);

    int end_len = strlen(end);

    //
    return
        // "str" is not shorter then "end"
        (str_len >= end_len) &&
        // Two strings' ends are equal
        (!strcmp(str + str_len - end_len, end));
}

//
BOOL prog_has_ext_in(String prog, StringList* ext_s) {
    //
    StringNode* node = ext_s->head;

    while (node) {
        // If two strings are equal
        if (string_endswith(prog, node->str)) {
            return TRUE;
        };

        //
        node = node->next;
    }

    return FALSE;
}

//
BOOL file_exists(LPCTSTR szPath) {
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

//
StringList* find_exe_paths(String prog)
{
    // An environment variable has a maximum size limit of 32,767 characters,
    // including the null-terminating character.
    char buf[32767];

    // 8f1kRCu
    DWORD res = GetEnvironmentVariable(TEXT("PATHEXT"), buf, 32767);

    // 4fpQ2RB
    // If "res" indicates error
    if (res == 0) {
        return NULL;
    }

    // 6qhHTHF
    // Split into a list of extensions
    //
    // Freed at 9nopWXf
    StringList* ext_s = stringlist_create();

    string_split(ext_s, buf, ';');

    //
    StringNode* node;

    // 2pGJrMW
    // Strip
    node = ext_s->head;

    while (node) {
        // String "node->str" is changed in-place
        string_strip((char *)node->str);

        //
        node = node->next;
    }

    // 2gqeHHl
    // Remove empty.
    // Must be done after the stripping at 2pGJrMW.
    //
    // Freed at 3vUw4d9
    StringList* ext_s_2 = stringlist_create();

    node = ext_s->head;

    while (node) {
        // If is not empty string
        if (node->str[0]) {
            // String "node->str" is changed ownership
            stringlist_add_end(ext_s_2, node->str);

            // Clear old pointer
            node->str = NULL;
        };

        //
        node = node->next;
    }

    // 9nopWXf
    stringlist_del_v2(&ext_s);

    assert(!ext_s);

    // 2zdGM8W
    // Convert to lowercase
    node = ext_s_2->head;

    while (node) {
        // String "node->str" is changed in-place
        string_tolower((char *)node->str);

        //
        node = node->next;
    }

    // 2fT8aRB
    // Uniquify
    //
    // Freed at 6b8UxoC
    StringList* ext_s_3 = stringlist_create();

    stringlist_uniq(ext_s_3, ext_s_2);

    // 3vUw4d9
    stringlist_del_v2(&ext_s_2);

    assert(!ext_s_2);

    //
    StringList* dir_path_s = stringlist_create();

    // 4ysaQVN
    res = GetEnvironmentVariable(TEXT("PATH"), buf, sizeof buf);

    // 5gGwKZL
    // If "res" indicates error
    if (res == 0) {
        // 7bVmOKe
        // Go ahead with "dir_path_s" being empty
        ;
    }
    else {
        // 6mPI0lg
        // Split into a list of dir paths
        string_split(dir_path_s, buf, ';');
    }

    // 5rT49zI
    // Insert empty dir path to the beginning.
    //
    // Empty dir handles the case that "prog" is a path, either relative or
    // absolute. See code 7rO7NIN.
    stringlist_add_head(dir_path_s, string_copy_new(""));

    // 2klTv20
    // Uniquify
    //
    // Freed at 6f2j5cZ
    StringList* dir_path_s_2 = stringlist_create();

    stringlist_uniq(dir_path_s_2, dir_path_s);

    // 6f2j5cZ
    stringlist_del_v2(&dir_path_s);

    assert(!dir_path_s);

    // 9gTU1rI
    // Check if "prog" ends with one of the file extension in "ext_s".
    //
    // "ext_s_3" are all in lowercase, ensured at 2zdGM8W.
    //
    // Freed at 2aR7zCp
    char* prog_lc = string_copy_new(prog);

    string_tolower(prog_lc);

    BOOL prog_has_ext = prog_has_ext_in(prog_lc, ext_s_3);

    // 2aR7zCp
    free(prog_lc);

    // 6bFwhbv
    //
    // Freed at 8swW6Av
    StringList* exe_path_s = stringlist_create();

    node = dir_path_s_2->head;

    while (node) {
        // 7rO7NIN
        // Synthesize a path
        String dir_path = node->str;

        String path = NULL;

        // If dir path is empty string
        if (!dir_path[0]) {
            path = prog;
        }
        else {
            // "PathCombine" will not writes more than MAX_PATH chars to "buf"
            assert(sizeof buf >= MAX_PATH);

            LPTSTR res = PathCombine(buf, dir_path, prog);

            // If "res" indicates error
            if (res == NULL) {
                // Ignore
                continue;
            }

            //
            path = buf;
        }

        // 6kZa5cq
        // If "prog" ends with executable file extension
        if (prog_has_ext) {
            // 3whKebE
            if (file_exists(path)) {
                // 2ffmxRF
                stringlist_add_end(exe_path_s, string_copy_new(path));
            }
        }

        // 2sJhhEV
        // Assume user has omitted the file extension
        StringNode* ext_node = ext_s_3->head;

        while (ext_node) {
            // 6k9X6GP
            // Synthesize a path with one of the file extensions in PATHEXT
            //
            // Freed at 4vT1o9M, or changed ownership at 7dui4cD
            char* path_2 = string_concat_new(path, ext_node->str);

            // 6kabzQg
            if (file_exists(path_2)) {
                // 7dui4cD
                // "path_2" is changed ownership
                stringlist_add_end(exe_path_s, path_2);
            }
            else {
                // 4vT1o9M
                free(path_2);
            }

            //
            ext_node = ext_node->next;
        }

        //
        node = node->next;
    }

    // 6b8UxoC
    stringlist_del_v2(&ext_s_3);

    assert(!ext_s_3);

    // 8swW6Av
    // Uniquify
    StringList* exe_path_s_2 = stringlist_create();

    stringlist_uniq(exe_path_s_2, exe_path_s);

    stringlist_del_v2(&exe_path_s);

    assert(!exe_path_s);

    // 7y3JlnS
    return exe_path_s_2;
};

// 4zKrqsC
// Program entry
int main(int argc, char* argv[])
{
    //
    int exit_code = 0;

    // 9mlJlKg
    // If not exactly one command argument is given
    if (argc != 2) {
        // 7rOUXFo
        // Print program usage
        char const * const usage_txt = "Usage: aoikwinwhich PROG\n"
            "\n"
            "#/ PROG can be either name or path\n"
            "aoikwinwhich notepad.exe\n"
            "aoikwinwhich C:\\Windows\\notepad.exe\n"
            "\n"
            "#/ PROG can be either absolute or relative\n"
            "aoikwinwhich C:\\Windows\\notepad.exe\n"
            "aoikwinwhich Windows\\notepad.exe\n"
            "\n"
            "#/ PROG can be either with or without extension\n"
            "aoikwinwhich notepad.exe\n"
            "aoikwinwhich notepad\n"
            "aoikwinwhich C:\\Windows\\notepad.exe\n"
            "aoikwinwhich C:\\Windows\\notepad";

        printf("%s", usage_txt);

        // 3nqHnP7
        exit_code = 1;

        return exit_code;
    }

    //
    assert(argc == 2);

    // 9m5B08H
    // Get executable name or path
    String prog = argv[1];

    // 8ulvPXM
    // Find executable paths
    //
    // Freed at 2jUVFP0
    StringList* exe_path_s = find_exe_paths(prog);

    // 5fWrcaF
    // If has found none
    if (!exe_path_s || !exe_path_s->count) {
        // 3uswpx0
        exit_code = 2;
    }
    // If has found some
    else {
        // 9xPCWuS
        // Print result
        StringNode* node = exe_path_s->head;

        while (node) {
            //
            printf("%s\n", node->str);

            //
            node = node->next;
        }

        // 4s1yY1b
        exit_code = 0;
    }

    // 2jUVFP0
    if (exe_path_s) {
        stringlist_del_v2(&exe_path_s);

        assert(!exe_path_s);
    }

    //
    return exit_code;
}
