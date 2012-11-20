/**
 * \file
 * This file contains the tree representation of (multiparty) session
 * according to the Scribble language specification and provides functions
 * to build and manipulate session type trees.
 * 
 * \headerfile "st_node.h"
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "st_node.h"
#include "st_expr.h"


st_tree *st_tree_init(st_tree *tree)
{
  assert(tree != NULL);
  tree->info = (st_info *)malloc(sizeof(st_info));
  tree->info->nrole   = 0;
  tree->info->nimport = 0;
  tree->root = NULL;

  return tree;
}


void st_tree_free(st_tree *tree)
{
  assert(tree != NULL);
  if (tree->info != NULL)
    free(tree->info);
  if (tree->root == NULL)
    st_node_free(tree->root);
}


void st_node_free(st_node *node)
{
  int i;
  for (i=0; i<node->nchild; ++i) {
    st_node_free(node->children[i]);
  }
  if (node->nchild > 0) {
    free(node->children);
  }
  node->nchild = 0;

  switch (node->type) {
    case ST_NODE_ROOT:
      break;
    case ST_NODE_SENDRECV:
    case ST_NODE_SEND:
    case ST_NODE_RECV:
      free(node->interaction);
      break;
    case ST_NODE_PARALLEL:
      break;
    case ST_NODE_CHOICE:
      free(node->choice);
      break;
    case ST_NODE_RECUR:
      free(node->recur);
      break;
    case ST_NODE_CONTINUE:
      free(node->recur);
      break;
    case ST_NODE_FOR:
      free(node->forloop);
      break;
    default:
      fprintf(stderr, "%s:%d %s Unknown node type: %d\n", __FILE__, __LINE__, __FUNCTION__, node->type);
      break;
  }

  free(node);
}


st_tree *st_tree_set_name(st_tree *tree, const char *name)
{
  assert(tree != NULL);
  tree->info->name = (char *)calloc(sizeof(char), strlen(name)+1);
  tree->info->param = NULL;
  strcpy(tree->info->name, name);

  return tree;
}


st_tree *st_tree_set_name_param(st_tree *tree, const char *name, st_expr_t *param)
{
  assert(tree != NULL);
  tree->info->name = (char *)calloc(sizeof(char), strlen(name)+1);
  tree->info->param = param;
  strcpy(tree->info->name, name);

  return tree;
}


st_tree *st_tree_add_role(st_tree *tree, const char *role)
{
  assert(tree != NULL);
  assert(tree->info != NULL);
  if (tree->info->nrole == 0) {
    // Allocate for 1 element.
    tree->info->roles = (st_role_t **)malloc(sizeof(st_role_t *));
  } else if (tree->info->nrole > 0) {
    // Allocate for n+1 element.
    tree->info->roles = (st_role_t **)realloc(tree->info->roles, sizeof(st_role_t *) * (tree->info->nrole+1));
  }

  tree->info->roles[tree->info->nrole] = (st_role_t *)malloc(sizeof(st_role_t));
  tree->info->roles[tree->info->nrole]->name = strdup(role);
  tree->info->roles[tree->info->nrole]->param = NULL; // Non-parametrised

  tree->info->nrole++;

  return tree;
}


st_tree *st_tree_add_role_param(st_tree *tree, const char *role, st_expr_t *param)
{
  assert(tree != NULL);
  assert(tree->info != NULL);
  assert(param != NULL);
  assert(param->type <= 100);
  if (tree->info->nrole == 0) {
    // Allocate for 1 element.
    tree->info->roles = (st_role_t **)malloc(sizeof(st_role_t *));
  } else if (tree->info->nrole > 0) {
    // Allocate for n+1 element.
    tree->info->roles = (st_role_t **)realloc(tree->info->roles, sizeof(st_role_t *) * (tree->info->nrole+1));
  }

  tree->info->roles[tree->info->nrole] = (st_role_t *)malloc(sizeof(st_role_t));
  tree->info->roles[tree->info->nrole]->name = strdup(role);
  tree->info->roles[tree->info->nrole]->param = param;

  tree->info->nrole++;

  return tree;
}


st_tree *st_tree_add_import(st_tree *tree, st_tree_import_t import)
{
  assert(tree != NULL);
  if (tree->info == NULL) {
    tree->info = malloc(sizeof(st_info));
    tree->info->nrole = 0;
  }

  if (tree->info->nimport == 0) {
    // Allocate for 1 element.
    tree->info->imports = (st_tree_import_t **)malloc(sizeof(st_tree_import_t *));
  } else if (tree->info->nimport > 0) {
    // Allocate for n+1 element.
    tree->info->imports = (st_tree_import_t **)realloc(tree->info->imports, sizeof(st_tree_import_t *) * (tree->info->nimport+1));
  }

  tree->info->imports[tree->info->nimport] = (st_tree_import_t *)malloc(sizeof(st_tree_import_t));
  memcpy(tree->info->imports[tree->info->nimport], &import, sizeof(st_tree_import_t));

  tree->info->nimport++;

  return tree;
}


st_node *st_node_init(st_node *node, int type)
{
  assert(node != NULL);
  node->type = type;
  switch (type) {
    case ST_NODE_ROOT:
      break;
    case ST_NODE_SENDRECV:
    case ST_NODE_SEND:
    case ST_NODE_RECV:
      node->interaction = (st_node_interaction *)malloc(sizeof(st_node_interaction));
      node->interaction->msg_cond = NULL;
      break;
    case ST_NODE_PARALLEL:
      break;
    case ST_NODE_CHOICE:
      node->choice = (st_node_choice *)malloc(sizeof(st_node_choice));
      memset(node->choice, 0, sizeof(st_node_choice));
      break;
    case ST_NODE_RECUR:
      node->recur = (st_node_recur *)malloc(sizeof(st_node_recur));
      memset(node->recur, 0, sizeof(st_node_recur));
      break;
    case ST_NODE_CONTINUE:
      node->cont = (st_node_continue *)malloc(sizeof(st_node_continue));
      memset(node->cont, 0, sizeof(st_node_continue));
      break;
    case ST_NODE_FOR:
      node->forloop = (st_node_for *)malloc(sizeof(st_node_for));
      memset(node->forloop, 0, sizeof(st_node_for));
      break;
    default:
      fprintf(stderr, "%s:%d %s Unknown node type: %d\n", __FILE__, __LINE__, __FUNCTION__, type);
      break;
  }
  node->nchild = 0;
  node->marked = 0;

  return node;
}


st_node *st_node_append(st_node *node, st_node *child)
{
  assert(node != NULL);
  assert(child != NULL);
  if (node->nchild == 0) {
    // Allocate for 1 node.
    node->children = (st_node **)malloc(sizeof(st_node *));
  } else if (node->nchild > 0) {
    // Allocate for n+1 nodes.
    node->children = (st_node **)realloc(node->children, sizeof(st_node *) * (node->nchild+1));
  }

  node->children[node->nchild++] = child;

  return node;
}


void st_tree_print(const st_tree *tree)
{
  int i;

  if (tree == NULL) {
    fprintf(stderr, "%s:%d %s tree is NULL\n", __FILE__, __LINE__, __FUNCTION__);
  }

  printf("\n-------Summary------\n");

  if (tree->info != NULL) {
    printf("Protocol: %s\n", tree->info->name);
    if (ST_TYPE_GLOBAL == tree->info->type) {
      printf("Global");
    } else if (ST_TYPE_LOCAL == tree->info->type) {
      printf("Local");
    } else if (ST_TYPE_PARAMETRISED == tree->info->type) {
      printf("Parametrised local");
    } else assert(1/* unrecognised type */);
    printf(" protocol\n");

    if (ST_TYPE_GLOBAL != tree->info->type) {
      printf("Endpoint role: %s", tree->info->myrole);
    }
    if (tree->info->param != NULL) {
      printf("[");
      st_expr_print(tree->info->param);
      printf("]");
    }
    printf("\n");
    printf("Imports: [\n");
    for (i=0; i<tree->info->nimport; ++i)
      printf("  { name: %s, as: %s, from: %s }\n", tree->info->imports[i]->name, tree->info->imports[i]->as, tree->info->imports[i]->from);
    printf("]\n");
    printf("Roles: [");
    for (i=0; i<tree->info->nrole; ++i) {
      printf(" %s", tree->info->roles[i]->name);
      if (NULL != tree->info->roles[i]->param) {
        printf("[");
        st_expr_print(tree->info->roles[i]->param);
        printf("]");
      }
    }
    printf(" ]\n");
  } else {
    printf("Protocol info not found\n");
  }

  printf("--------------------\n");

  if (tree->root != NULL) {
    st_node_print_r(tree->root, 0);
  } else {
    printf("Protocol tree is empty\n");
  }

  printf("--------------------\n\n");
}


void st_node_print_r(const st_node *node, int indent)
{
  int i;

  st_node_print(node, indent);
  for (i=0; i<node->nchild; ++i) {
    st_node_print_r(node->children[i], indent+1);
  }
}


void st_node_print(const st_node *node, int indent)
{
  int i;

  if (node != NULL) {
    if (node->marked) {
      printf("%3d *>", indent);
    } else {
      printf("%3d | ", indent);
    }
    for (i=indent; i>0; --i) printf("  ");
    switch (node->type) {

      case ST_NODE_ROOT: // ---------- ROOT ----------
        printf("Node { type: root }\n");
        break;

      case ST_NODE_SENDRECV: // ---------- SENDRECV ----------

        printf("Node { type: interaction, from: %s", node->interaction->from->name);
        if (NULL != node->interaction->from->param) {
          printf("[");
          st_expr_print(node->interaction->from->param);
          printf("]");
        }

        printf(", to(%d): ", node->interaction->nto);
        printf("[%s", node->interaction->to[0]->name);
        if (NULL != node->interaction->to[0]->param) {
          printf("[");
          st_expr_print(node->interaction->to[0]->param);
          printf("]");
        }
        printf(" ..]");

        printf(", msgsig: { op: %s, payload: %s }", node->interaction->msgsig.op, node->interaction->msgsig.payload);

        printf("}\n");
        break;

      case ST_NODE_SEND: // ---------- SEND ----------
        printf("Node { type: send");
        printf(", to(%d): ", node->interaction->nto);
        printf("[%s", node->interaction->to[0]->name);
        if (NULL != node->interaction->to[0]->param) {
          printf("[");
          st_expr_print(node->interaction->to[0]->param);
          printf("]");
        }
        printf(" ..]");

        printf(", msgsig: { op: %s, payload: %s }", node->interaction->msgsig.op, node->interaction->msgsig.payload);

        if (NULL != node->interaction->msg_cond) { // ST_ROLE_PARAMETRISED, always
          printf(", cond: %s", node->interaction->msg_cond->name);
          assert(NULL!=node->interaction->msg_cond->param);
          printf("[");
          st_expr_print(node->interaction->msg_cond->param);
          printf("]");
        } // if msg_cond

        printf("}\n");
        break;

      case ST_NODE_RECV: // ----------- RECV -----------
        printf("Node { type: recv, from: %s", node->interaction->from->name);
        if (NULL != node->interaction->from->param) {
          printf("[");
          st_expr_print(node->interaction->from->param);
          printf("]");
        }

        printf(", msgsig: { op: %s, payload: %s }", node->interaction->msgsig.op, node->interaction->msgsig.payload);

        if (NULL != node->interaction->msg_cond) {
          printf(", cond: %s", node->interaction->msg_cond->name);
          assert(NULL != node->interaction->msg_cond->param);
          printf("[");
          st_expr_print(node->interaction->msg_cond->param);
          printf("]");
        } // if msg_cond

        printf("}\n");
        break;

      case ST_NODE_CHOICE: // ---------- CHOICE ----------
        printf("Node { type: choice, at: %s } %d children \n", node->choice->at, node->nchild);
        break;

      case ST_NODE_PARALLEL: // ---------- PARALLEL ----------
        printf("Node { type: par }\n");
        break;

      case ST_NODE_RECUR: // ---------- RECUR ----------
        printf("Node { type: recur, label: %s }\n", node->recur->label);
        break;

      case ST_NODE_CONTINUE: // ---------- CONTINUE ----------
        printf("Node { type: continue, label: %s }\n", node->cont->label);
        break;

      case ST_NODE_FOR: // ---------- FOR ----------
        printf("Node { type: forloop, var: %s range: ", node->forloop->var);
        if (node->forloop->range == NULL) { printf("NULL"); }
        else { st_expr_print(node->forloop->range); }
        printf("\n");
        break;

      default:
        fprintf(stderr, "%s:%d %s Unknown node type: %d\n", __FILE__, __LINE__, __FUNCTION__, node->type);
        break;
    }
  }
}


void st_node_reset_markedflag(st_node *node)
{
  int i = 0;
  assert(node != NULL);
  node->marked = 0;

  for (i=0; i<node->nchild; ++i) {
    st_node_reset_markedflag(node->children[i]);
  }
}


int st_node_is_overlapped(const st_node *node, const st_node *other)
{
  int is_overlapped = 0;
  if (ST_NODE_SEND == node->type) {
    if (NULL == node->interaction->to[0]->param
        && ((other->type == ST_NODE_SEND && NULL == other->interaction->to[0]->param)
            ||(other->type == ST_NODE_RECV && NULL == other->interaction->from->param))) {
      is_overlapped = 1; // Normal role with same name
    } else {
      // Try to match role parameters too
      if (other->type == ST_NODE_RECV) {
        is_overlapped |= st_expr_is_overlapped(node->interaction->to[0]->param, other->interaction->from->param);
      } else if (other->type == ST_NODE_SEND) {
        is_overlapped |= st_expr_is_overlapped(node->interaction->to[0]->param, other->interaction->to[0]->param);
      }
    }
  }
  if (ST_NODE_RECV == node->type) {
    if (NULL == node->interaction->from->param
        && ((other->type == ST_NODE_SEND && NULL == other->interaction->to[0]->param)
            ||(other->type == ST_NODE_RECV && NULL == other->interaction->from->param))) {
        is_overlapped = 1; // Normal role with same name
    } else {
      if (other->type == ST_NODE_RECV) {
        is_overlapped |= st_expr_is_overlapped(node->interaction->from->param, other->interaction->from->param);
      } else if (other->type == ST_NODE_SEND) {
        is_overlapped |= st_expr_is_overlapped(node->interaction->from->param, other->interaction->to[0]->param);
      }
    }
  }
  return is_overlapped;
}



int st_node_compare_async(const st_node *node, const st_node *other)
{
  int identical = 1;
  int search_from, search_to;
  int visited[node->nchild];

  int i, j;

  // We currently only support async optimisation inside recursion blocks
  // and only in the toplevel child of a recursion block
  //
  if ((node->type != ST_NODE_FOR && node->type != ST_NODE_RECUR) || (node->type != ST_NODE_FOR && other->type != ST_NODE_RECUR))
    return 0;

  if (node->nchild != other->nchild)
    return 0;

  for (i=0; i<node->nchild; ++i) {
    visited[i] = 0;
  }

  // If first node is not send/recv, used ordinary compare
  if (node->nchild > 0 && node->children[0]->type != ST_NODE_SEND && node->children[0]->type != ST_NODE_RECV) {

    for (i=0; i<node->nchild; ++i) {
      identical &= st_node_compare_r(node->children[i], other->children[i]);
    }

  } else {

    // Mark range of consecutive send/recv for async optimisation (search_from - search_to).
    search_from = 0;
    search_to   = node->nchild;

    for (i=0; i<node->nchild; ++i) {
      if (ST_NODE_SEND == node->children[i]->type || ST_NODE_RECV == node->children[i]->type) {
        search_from = i;
        for (j=i+1; j<node->nchild; ++j) {
          if (ST_NODE_SEND != node->children[j]->type && ST_NODE_RECV != node->children[j]->type) {
            search_to = j;
            break;
          }
        }
        // If nothing is found, search_to is node->nchild;
        break; // CHANGEME: We consider the first segment only
      }
    }

    printf("%s: Matching range %d - %d\n", __FUNCTION__, search_from, search_to-1);
    st_node_print(node->children[search_from], 0);
    st_node_print(node->children[search_to-1], 0);

    // The actual matching.
    for (i=search_from; i<search_to; ++i) {
      //assert(ST_NODE_SEND == node->children[i]->type || ST_NODE_RECV == node->children[i]->type);

      if (ST_NODE_RECV == node->children[i]->type) {
        // - Look for matching receive
        // - Allow send in same channel (to overtake)
        // - Stop at non-matching receive in same channel or end of search range
        for (j=search_from; j<search_to; ++j) {

          // Case 0: This node has been matched previously.
          if (visited[j] == 1) {
            continue;
          }

          // Case 1: RECV in the same channel.
          if (visited[j] == 0
              && ST_NODE_RECV == other->children[j]->type) { // Recv node

            if (st_node_compare_interaction(node->children[i], other->children[j])) {
              // Matching RECV node
              printf("RECV matched a RECV node: %d-%d: Done\n", i, j);
              st_node_print(node->children[i], 0);
              st_node_print(other->children[j], 0);
              visited[j] = 1;
              identical &= 1;
              break;
            } else if (st_node_is_overlapped(node->children[i], other->children[j])) {
              // Don't allow RECV-RECV overtake in same channel
              node->children[i]->marked = 1;
              other->children[j]->marked = 1;
              identical = 0;
              break;
            } else { // Not the same and not overlapped
              continue;
            }
          }

          // Case 2: SEND in same channel.
          if (visited[j] == 0
              && ST_NODE_SEND == other->children[j]->type
              && st_node_is_overlapped(node->children[i], other->children[j])) { // Send node
            // Same channel SEND is allowed.
            printf("RECV matched a SEND node: %d-%d: Skip\n", i, j);
            st_node_print(node->children[i], 0);
            st_node_print(other->children[j], 0);
            continue;
          }

        } // for nodes matching RECV

        // No matching node found
        if (j == search_to) {
          node->children[i]->marked = 1;
          identical = 0;
        }

      } else if (ST_NODE_SEND == node->children[i]->type) {
        // - Look for matching send
        // - Allow send in the same channel (to overtake)
        // - Stop at receive in the same channel or end of search range
        for (j=search_from; j<search_to; ++j) {

          // Case 0: This node has been matched previously.
          if (visited[j] == 1) {
            continue;
          }

          // Case 1: SEND in the same channel.
          if (visited[j] == 0
              && ST_NODE_SEND == other->children[j]->type) {
            if (st_node_compare_interaction(node->children[i], other->children[j])) {
              // Matching SEND node
              printf("SEND matched a SEND node: %d-%d: Done\n", i, j);
              st_node_print(node->children[i], 0);
              st_node_print(other->children[j], 0);
              visited[j] = 1;
              identical &= 1;
              break;
            } else if (st_node_is_overlapped(node->children[i], other->children[j])) {
              // Don't allow SEND-SEND overtake in same channel
              printf("SEND unmatched a SEND node: %d-%d: Wrong\n", i, j);
              st_node_print(node->children[i], 0);
              st_node_print(other->children[j], 0);
              node->children[i]->marked = 1;
              other->children[j]->marked = 1;
              identical = 0;
              break;
            } else { // Not the same and not overlapped
              continue;
            }
          }

          // Case 2: RECV node in same channel
          if (visited[j] == 0
              && ST_NODE_RECV == other->children[j]->type
              && st_node_is_overlapped(node->children[i], other->children[j])) {
            // Don't allow RECV-SEND overtake in the same channel
            printf("SEND matched a RECV node: %d-%d: Wrong\n", i, j);
            st_node_print(node->children[i], 0);
            st_node_print(other->children[j], 0);
            node->children[i]->marked = 1;
            other->children[j]->marked = 1;
            identical = 0;
            break;
          }

        } // for nodes matching SEND

        // No matching node found
        if (j == search_to) {
          node->children[i]->marked = 1;
          identical = 0;
        }

      }

    } // Checking done for search_from - search_to

    printf("%s: Matching range (non async) %d - %d\n", __FUNCTION__, search_to, node->nchild-1);
    st_node_print(node->children[search_from], 0);
    st_node_print(other->children[search_to-1], 0);


    for (i=search_to; i<node->nchild; ++i) {
      identical &= st_node_compare_r(node->children[i], other->children[i]);
    }
  }

  return identical;
}


int st_node_compare_r(st_node *node, st_node *other)
{
  int identical = 1;
  int i;

  if (node != NULL && other != NULL) {
    identical &= st_node_compare(node, other);

    if (node->type == ST_NODE_RECUR || node->type == ST_NODE_FOR) {
      identical &= st_node_compare_async(node, other);
    } else {
      for (i=0; i<node->nchild; ++i) {
        identical &= st_node_compare_r(node->children[i], other->children[i]);
      }
    }
  }

  return identical;
}


int st_node_compare_msgsig(const st_node_msgsig_t msgsig, const st_node_msgsig_t other)
{
  return ( !((msgsig.op == NULL && other.op != NULL) || (msgsig.op != NULL && other.op == NULL))
           && ((msgsig.op == NULL && other.op == NULL) || 0 == strcmp(msgsig.op, other.op))
           && (0 == strcmp(msgsig.payload, other.payload)) );
}


// Helper function to compare interaction nodes or ranges of interaction nodes
int st_node_compare_interaction(st_node *node, st_node *other)
{
  int identical = 1;
  int i = 0;
  assert(ST_NODE_SENDRECV == node->type || ST_NODE_SEND == node->type || ST_NODE_RECV == node->type);
  if (node->interaction->msg_cond != NULL && other->interaction->msg_cond != NULL) {
    identical &= st_expr_is_identical(node->interaction->msg_cond->param, other->interaction->msg_cond->param);
  }

  switch (node->type) {
    case ST_NODE_SENDRECV:
      // Send
      for (i=0; i<node->interaction->nto; ++i) {
        identical &= (0 == strcmp(node->interaction->to[i]->name, other->interaction->to[i]->name) || 0 == strcmp(other->interaction->to[i]->name, "__ROLE__"));
        identical &= (node->interaction->to[i]->param == NULL && other->interaction->to[i]->param == NULL)
                      || st_expr_is_identical(node->interaction->to[i]->param, other->interaction->to[i]->param);
      }
      // Receive
      identical &= (0 == strcmp(node->interaction->from->name, other->interaction->from->name) || 0 == strcmp(other->interaction->from->name, "__ROLE__"));
      identical &= (node->interaction->from->param == NULL && other->interaction->from->param == NULL)
                    || st_expr_is_identical(node->interaction->from->param, other->interaction->from->param);
      break;

    case ST_NODE_SEND:
      for (i=0; i<node->interaction->nto; ++i) {
        identical &= (0 == strcmp(node->interaction->to[i]->name, other->interaction->to[i]->name) || 0 == strcmp(other->interaction->to[i]->name, "__ROLE__"));
        identical &= (node->interaction->to[i]->param == NULL && other->interaction->to[i]->param == NULL)
                      || st_expr_is_identical(node->interaction->to[i]->param, other->interaction->to[i]->param);
      }
      break;

    case ST_NODE_RECV:
      identical &= (0 == strcmp(node->interaction->from->name, other->interaction->from->name) || 0 == strcmp(other->interaction->from->name, "__ROLE__"));
      identical &= (node->interaction->from->param == NULL && other->interaction->from->param == NULL)
                    || st_expr_is_identical(node->interaction->from->param, other->interaction->from->param);
      break;

    default:
      fprintf(stderr, "%s:%d %s Unknown node type: %d\n", __FILE__, __LINE__, __FUNCTION__, node->type);
  }

  return identical;
}


int st_node_compare(st_node *node, st_node *other)
{
  int identical = 1;
  if (node != NULL && other != NULL) {
    identical = (node->type == other->type && node->nchild == other->nchild);

    if (identical) {

      switch (node->type) {
        case ST_NODE_ROOT:
          break;

        case ST_NODE_SENDRECV:
          identical &= st_node_compare_msgsig(node->interaction->msgsig, other->interaction->msgsig);
          identical &= st_node_compare_interaction(node, other);
          break;

        case ST_NODE_SEND:
          identical &= st_node_compare_msgsig(node->interaction->msgsig, other->interaction->msgsig);
          identical &= st_node_compare_interaction(node, other);
          break;

        case ST_NODE_RECV:
          identical &= st_node_compare_msgsig(node->interaction->msgsig, other->interaction->msgsig);
          identical &= st_node_compare_interaction(node, other);
          break;

        case ST_NODE_CHOICE:
          identical &= (0 == strcmp(node->choice->at, other->choice->at));
          break;

        case ST_NODE_PARALLEL:
          break;

        case ST_NODE_RECUR:
          // The label might be different: source code recursion label are generated
          // identical &= (0 == strcmp(node->recur->label, other->recur->label));
          break;

        case ST_NODE_CONTINUE:
          // The label might be different: source code continue label are generated
          // identical &= (0 == strcmp(node->cont->label, other->cont->label));
          break;

        case ST_NODE_FOR:
          identical &= (0 == strcmp(node->forloop->var, other->forloop->var));
          identical &= st_expr_is_identical(node->forloop->range, other->forloop->range);
          break;

        default:
          fprintf(stderr, "%s:%d %s Unknown node type: %d\n", __FILE__, __LINE__, __FUNCTION__, node->type);
          break;
      }

    } // if identical

    if (!identical) {
      node->marked = 1;
      other->marked = 1;
    }

  }
  return identical;
}

