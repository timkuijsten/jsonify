#include "jsmn.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKENS 100
#define MAXSTACK 100

int pop();
int push(int val);

ssize_t from_non_strict_json(jsmn_parser *p, char *line, ssize_t linelen, jsmntok_t *tokens, ssize_t nrtokens);
void to_strict_json(const char *json, jsmntok_t *tokens, int nrtokens);

int main()
{
  char *line = NULL;
  size_t linesize = 0;
  ssize_t linelen;
  ssize_t nrtokens;
  jsmntype_t jt;
  jsmn_parser parser;
  jsmntok_t tokens[TOKENS];

  jsmn_init(&parser);

  while ((linelen = getline(&line, &linesize, stdin)) != -1) {
    nrtokens = from_non_strict_json(&parser, line, linelen - 1, tokens, TOKENS);
    fprintf(stderr, "len: %lu, tokens: %lu\n%s\n", linelen, nrtokens, line);
    to_strict_json(line, tokens, nrtokens);
  }
  free(line);

  return 0;
}

ssize_t
from_non_strict_json(jsmn_parser *p, char *line, ssize_t linelen, jsmntok_t *tokens, ssize_t nrtokens)
{
  return jsmn_parse(p, line, linelen, tokens, nrtokens);
}

void
to_strict_json(const char *json, jsmntok_t *tokens, int nrtokens)
{
  char *key, c;
  jsmntok_t *tok;
  int type, i, j;

  for (i = 0; i < nrtokens; i++) {
    tok = &tokens[i];
    type = tok->type;
    key = strndup(json + tok->start, tok->end - tok->start);

    // 1.
    switch (type) {
    case JSMN_OBJECT:
      printf("%c", '{');
      push('}');
      break;
    case JSMN_ARRAY:
      printf("%c", '[');
      push(']');
      break;
    case JSMN_UNDEFINED:
      printf("%s", "undefined");
      if (tok->size)
        printf("%c", ':');
      break;
    case JSMN_STRING:
      printf("\"%s\"", key);
      if (tok->size)
        printf("%c", ':');
      break;
    case JSMN_PRIMITIVE:
      printf("%s", key);
      if (tok->size)
        printf("%c", ':');
      break;
    default:
      fatal("unknown json token type");
    }

    // 2.
    for (j = 0; j < tok->size - 1; j++)
      switch (type) {
      case JSMN_OBJECT:
        if (push(',') == -1)
          fatal("stack push error");
        break;
      case JSMN_ARRAY:
        if (push(',') == -1)
          fatal("stack push error");
        break;
      default:
        fatal("unknown json token type");
      }

    // 3.
    if (!tok->size) {
      if ((c = pop()) != -1)
        printf("%c", c);
      while (c == ']' || c == '}')
        if ((c = pop()) != -1)
          printf("%c", c);
    }
  }
  // flush stack
  //if ((c = pop()) != -1)
    //printf("%c", c);

  if (nrtokens)
    printf("\n");
}

// natural number stack
static int sp = 0;
static int stack[MAXSTACK];

// pop item from the stack
// return item on the stack on success, -1 on error
int pop()
{
  if (sp == 0)
    return -1;
  return stack[--sp];
}

// push new item on the stack
// return 0 on success, -1 on error
int push(int val)
{
  if (val == -1) // don't support -1 values, reserved for errors
    return -1;
  if (sp == MAXSTACK)
    return -1;
  stack[sp++] = val;
  return 0;
}
