// MyGit Project
//
// add_prefix_all_list.h
// File description:
//    add_prefix_all_list.h

#ifndef ADD_PREFIX_ALL_LIST_H
#define ADD_PREFIX_ALL_LIST_H

#include "libs/list/list.h"

/**
 * @brief Add a prefix to all the elements of the list
 *
 * @param list The list to modify
 * @param prefix The prefix to add
 */
void add_prefix_all_list(list_t *list, const char *prefix);

#endif