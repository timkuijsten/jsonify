/**
 * Copyright (c) 2016 Tim Kuijsten
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "jsmn.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKENS 100
#define MAXSTACK 100

int pop();
int push(int val);

int relaxed_to_strict(char *output, size_t outputsize, const char *input, ssize_t inputlen, int maxroot);
ssize_t from_relaxed(jsmn_parser *p, const char *line, ssize_t linelen, jsmntok_t *tokens, ssize_t nrtokens);
int iterate(const char *input, jsmntok_t *tokens, int nrtokens, void (*iterator)(jsmntok_t *, char *, int, int, char *));
int to_strict(char *output, size_t outputsize, const char *input, jsmntok_t *tokens, int nrtokens, int maxobj);
