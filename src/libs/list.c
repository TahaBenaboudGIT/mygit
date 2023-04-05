// Sorbonne University, 2022 - 2023
// MyGit Project
// 
// list.c
// File description:
//   This file contains the implementation of the list.h header.

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

List **initList() {
  List **new = malloc(sizeof(List *));
  if (!new)
    return NULL;
  *new = NULL;
  return new;
}

Cell *buildCell(const char *data) {
  Cell *newCell = malloc(sizeof(Cell));
  if (!newCell)
    return NULL;
  newCell->data = strdup(data);
  if (!newCell->data) {
    free(newCell);
    return NULL;
  }
  return newCell;
}

void freeCell(Cell *cell) {
  if (!cell)
    return;
  free(cell->data);
  free(cell);
}

void freeList(List **list) {
  if (!list)
    return;
  Cell *iter = *list, *tmp;
  while (iter) {
    tmp = iter;
    iter = iter->next;
    freeCell(tmp);
  }
  free(list);
}

void insertFirst(List **list, Cell *cell) {
  if (!list || !cell)
    return;
  cell->next = *list;
  *list = cell;
}

Cell *listGet(List **list, int pos) {
  if (!list || !*list)
    return NULL;
  Cell *iter = *list;
  for (int i = 0; i < pos && iter; ++i)
    iter = iter->next;
  if (!iter)
    fprintf(stderr, "[listGet] Index out of range\n");
  return iter;
}

char *ctos(const Cell *cell) {
  if (!cell)
    return NULL;
  return cell->data;
}

char *ltos(List **list) {
  Cell *iter = *list;
  size_t strSize = MAX_FILES * MAX_LEN_DATA;
  char *str = malloc(strSize * sizeof(char));
  if (!str)
    return NULL;
  while (iter) {
    snprintf(str, strSize, "%s%s", str, ctos(iter));
    iter = iter->next;
    if (iter)
      snprintf(str, strSize, "%s%s", str, SEPARATOR_LIST_STRING);
  }
  return str;
}

Cell *searchList(List **list, const char *str) {
  if (!list || !str)
    return NULL;
  Cell *iter = *list;
  while (iter && strcmp(iter->data, str))
    iter = iter->next;
  return iter;
}

int addFirstCell(List **list, const char *data) {
  Cell *myNewCell = NULL;
  myNewCell = buildCell(data);
  if (!myNewCell)
    return 0;
  insertFirst(list, myNewCell);
  return 1;
}

List **stol(const char *str) {
  List **res = initList();
  if (!res)
    return NULL;
  char buffer[MAX_LEN_DATA];
  int bufferSize = 0, bufferOffset = 0, i = 0;
  for (; str[i] != '\0'; ++i) {
    if (str[i] == SEPARATOR_LIST_STRING[0]) {
      memcpy(buffer, str + bufferOffset, bufferSize * sizeof(char));
      buffer[bufferSize] = '\0';
      if (!addFirstCell(res, buffer)) {
        fprintf(stderr, "Error while converting the string into a List, the "
                        "List could be not complete\n");
        return res;
      }
      bufferOffset += bufferSize + 1;
      bufferSize = 0;
      continue;
    }
    ++bufferSize;
  }
  memcpy(buffer, str + bufferOffset, bufferSize * sizeof(char));
  buffer[bufferSize] = '\0';
  if (!addFirstCell(res, buffer))
    fprintf(stderr, "Error while converting the string into a List, the List "
                    "could be not complete\n");
  return res;
}

void ltof(List **list, const char *path) {
  if (!list || !path)
    return;
  FILE *f = fopen(path, "w");
  if (!f) {
    fprintf(stderr, "Error, can't open file %s in write mode ...\n", path);
    return;
  }
  char *strList = ltos(list);
  if (!strList)
    return;
  fprintf(f, "%s\n", strList);
  fclose(f);
  free(strList);
}

List **ftol(const char *path) {
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
  return stol(buffer);
}
