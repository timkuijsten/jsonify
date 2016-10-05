#ifndef JSONIFY_H
#define JSONIFY_H

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

#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKENS 100
#define MAXSTACK 100
#define MAXOUTPUT 1024 * 16

int pop();
int push(int val);

long relaxed_to_strict(char *dst, size_t dstsize, const char *src, size_t srcsize, int firstonly);
int iterate(const char *src, jsmntok_t *tokens, int nrtokens, void (*iterator)(jsmntok_t *, char *, int, int, char *));
void strict_writer(jsmntok_t *tok, char *key, int depth, int ndepth, char *closesym);

#endif
