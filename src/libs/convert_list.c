// Sorbonne University, 2022 - 2023
// MyGit Project
//
// convert_list.c
// File description:
//     This file contains the implementation of the functions to convert a list
//     in another format.

#include "convert_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *list_to_string(List **list) {
  Cell *iter = *list;
  size_t str_size = MAX_FILES * MAX_LEN_DATA;
  char *str = malloc(str_size * sizeof(char));
  if (!str)
    return NULL;
  while (iter) {
    snprintf(str, str_size, "%s%s", str, cell_to_string(iter));
    iter = iter->next;
    if (iter)
      snprintf(str, str_size, "%s%s", str, SEPARATOR_LIST_STRING);
  }
  return str;
}

static void update_and_reset_buffer(const char *str, char *buffer, int *buffer_size,
                                    int *buffer_offset) {
  memcpy(buffer, str + *buffer_offset, *buffer_size * sizeof(char));
  buffer[*buffer_size] = '\0';
  *buffer_offset += *buffer_size + 1;
  *buffer_size = 0;
}

static int create_and_insert_cell_in_list_or_return(List **res, char *buffer) {
  if (!create_and_insert_cell_in_list(res, buffer)) {
    fprintf(stderr, "Error while converting the string into a List, the List "
                    "could be not complete");
    return 0;
  }
  return 1;
}

List **string_to_list(const char *str) {
  List **res = create_init_list();
  char buffer[MAX_LEN_DATA];
  int buffer_size = 0;
  int buffer_offset = 0;

  if (!res)
    return NULL;
  for (int i = 0; str[i] != '\0'; ++i) {
    if (str[i] == SEPARATOR_LIST_STRING[0]) {
      update_and_reset_buffer(str, buffer, &buffer_size, &buffer_offset);
      if (!create_and_insert_cell_in_list_or_return(res, buffer))
        return res;
      continue;
    }
    ++buffer_size;
  }
  update_and_reset_buffer(str, buffer, &buffer_size, &buffer_offset);
  create_and_insert_cell_in_list_or_return(res, buffer);
  return res;
}

void list_to_file(List **list, const char *path) {
  if (!list || !path)
    return;
  FILE *f = fopen(path, "w");
  if (!f) {
    fprintf(stderr, "Error, can't open file %s in write mode ...\n", path);
    return;
  }
  char *str_list = list_to_string(list);
  if (!str_list)
    return;
  fprintf(f, "%s\n", str_list);
  fclose(f);
  free(str_list);
}

List **file_to_list(const char *path) {
  if (!path)
    return NULL;
  FILE *f = fopen(path, "r");
  char buffer[MAX_FILES * MAX_LEN_DATA];
  if (!f) {
    fprintf(stderr, "Error, can't open file %s in read mode ...\n", path);
    return NULL;
  }
  fgets(buffer, MAX_FILES * MAX_LEN_DATA, f);
  fclose(f);
  return string_to_list(buffer);
}
