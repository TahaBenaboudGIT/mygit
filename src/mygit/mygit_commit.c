// MyGit Project
//
// mygit_commit.c
// File description:
//    mygit_commit.c

#include "mygit/mygit_commit.h"
#include "commit/commit.h"
#include "commit/create_blob_of_commit.h"
#include "commit/insert_key_val_in_commit.h"
#include "file/list_files.h"
#include "file/read_write_file.h"
#include "mygit/mygit_commit_pre_checks.h"
#include "refs/refs_utils.h"
#include "utils/constants.h"
#include "work_tree/save_content_and_work_tree.h"
#include "work_tree/save_get_file_work_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static work_tree_t *get_added_work_tree_and_delete(void)
{
  work_tree_t *work_tree = NULL;

  if (!does_file_exists(MYGIT_PATH_ADD)) {
    fprintf(stderr, "Error: no files added\n");
    return NULL;
  }
  work_tree = file_to_work_tree(MYGIT_PATH_ADD);
  remove(MYGIT_PATH_ADD);
  return work_tree;
}

static commit_t *get_commit_from_added(void)
{
  commit_t *commit = NULL;
  work_tree_t *work_tree = NULL;
  char *hash_work_tree = NULL;

  work_tree = get_added_work_tree_and_delete();
  if (!work_tree)
    return NULL;
  hash_work_tree = save_content_and_work_tree(work_tree, PREFIX_PATH);
  if (!hash_work_tree) {
    free_work_tree(work_tree);
    return NULL;
  }
  commit = create_commit_with_tree_key_val(hash_work_tree);
  free(hash_work_tree);
  free_work_tree(work_tree);
  return commit;
}

static char *fill_new_commit_create_blob_and_get_hash(commit_t *new_commit,
                                                      const char *message,
                                                      const char *branch_name)
{
  char *hash_last_commit = NULL;

  hash_last_commit = get_hash_from_last_commit_in_branch(branch_name);
  if (!hash_last_commit)
    return NULL;
  if (strcmp(hash_last_commit, "") != 0)
    insert_key_val_in_commit(new_commit, COMMIT_PARENT_KEY, hash_last_commit);
  free(hash_last_commit);
  if (message)
    insert_key_val_in_commit(new_commit, COMMIT_MESSAGE_KEY, message);
  return create_blob_of_commit(new_commit);
}

void mygit_commit(const char *branch_name, const char *message)
{
  int pre_checks = 0;
  commit_t *new_commit = NULL;
  char *hash_new_commit = NULL;

  if (!branch_name)
    return;
  pre_checks = check_if_refs_exists() && check_if_branch_exists(branch_name) &&
               check_if_head_is_in_branch(branch_name);
  if (!pre_checks)
    return;
  new_commit = get_commit_from_added();
  if (!new_commit)
    return;
  hash_new_commit = fill_new_commit_create_blob_and_get_hash(
      new_commit, message, branch_name);
  free_commit(new_commit);
  if (!hash_new_commit)
    return;
  set_head_and_branch_hash_last_commit(branch_name, hash_new_commit);
}
