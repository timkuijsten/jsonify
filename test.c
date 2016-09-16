#include "jsonify.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTR 1024

int test_relaxed_to_strict(const char *input, const char *exp, const int exp_exit);
int test_relaxed_to_strict_firstroot(const char *input, const char *exp, const int exp_exit);

int main()
{
  int failed = 0;

  printf("test relaxed_to_strict:\n");

  // one-dimensional
  failed += test_relaxed_to_strict("1", "1", 1);
  failed += test_relaxed_to_strict("[]", "[]", 2);
  failed += test_relaxed_to_strict("[1]", "[1]", 3);
  failed += test_relaxed_to_strict("[1,2]", "[1,2]", 5);

  // two-dimensional
  failed += test_relaxed_to_strict("[[]]", "[[]]", 4);
  failed += test_relaxed_to_strict("[1,[]]", "[1,[]]", 6);
  failed += test_relaxed_to_strict("[[],1]", "[[],1]", 6);
  failed += test_relaxed_to_strict("[1,[2]]", "[1,[2]]", 7);
  failed += test_relaxed_to_strict("[[1],2]", "[[1],2]", 7);
  failed += test_relaxed_to_strict("[1,2,[3,4],5]", "[1,2,[3,4],5]", 13);
  failed += test_relaxed_to_strict("[1,[2,3,4],5,6]", "[1,[2,3,4],5,6]", 15);

  // three-dimensional
  failed += test_relaxed_to_strict("[[],1,[2,3],4,[5,[],[6,7],8]]", "[[],1,[2,3],4,[5,[],[6,7],8]]", 29);

  // four-dimensional
  failed += test_relaxed_to_strict("[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,73],6,7],8]]", "[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,73],6,7],8]]", 61);

  // five-dimensional
  failed += test_relaxed_to_strict("[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]", "[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]", 63);

  // six-dimensional
  failed += test_relaxed_to_strict("[[],[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]]", "[[],[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]]", 68);

  // one-dimensional
  failed += test_relaxed_to_strict("{a:true}", "{\"a\":true}", 8);

  // two-dimensional
  failed += test_relaxed_to_strict("[{a:true,b:false},[{c:true}]]", "[{\"a\":true,\"b\":false},[{\"c\":true}]]", 29);

  // three-dimensional
  failed += test_relaxed_to_strict("[{a:true,b:false},[{c:[2,3,{$some:true}]}]]", "[{\"a\":true,\"b\":false},[{\"c\":[2,3,{\"$some\":true}]}]]", 43);

  failed += test_relaxed_to_strict(
      "[{$project:{by:true,action:true,on:true,sid:true}},{$match:{action:{$in:['socketstart','socketstop']}}},{$group:{_id:{sid:\"$sid\",uid:\"$by\"},login:{$min:\"$on\"},logout:{$max:\"$on\"}}},{$project:{login:true,logout:true,duration:{$subtract:[\"$logout\",\"$login\"]}}},{$project:{uid:\"$_id.uid\",duration:true,yearWeek:{$dateToString:{format:'%Y%U',date:\"$login\"}}}},{$group:{_id:{uid:\"$uid\",yearWeek:\"$yearWeek\"},duration:{$sum:\"$duration\"}}},{$project:{duration:{$divide:[\"$duration\",60*1000]}}}]",
      "[{\"$project\":{\"by\":true,\"action\":true,\"on\":true,\"sid\":true}},{\"$match\":{\"action\":{\"$in\":[\"socketstart\",\"socketstop\"]}}},{\"$group\":{\"_id\":{\"sid\":\"$sid\",\"uid\":\"$by\"},\"login\":{\"$min\":\"$on\"},\"logout\":{\"$max\":\"$on\"}}},{\"$project\":{\"login\":true,\"logout\":true,\"duration\":{\"$subtract\":[\"$logout\",\"$login\"]}}},{\"$project\":{\"uid\":\"$_id.uid\",\"duration\":true,\"yearWeek\":{\"$dateToString\":{\"format\":\"%Y%U\",\"date\":\"$login\"}}}},{\"$group\":{\"_id\":{\"uid\":\"$uid\",\"yearWeek\":\"$yearWeek\"},\"duration\":{\"$sum\":\"$duration\"}}},{\"$project\":{\"duration\":{\"$divide\":[\"$duration\",60*1000]}}}]", 487);

  // multiple docs in one string
  failed += test_relaxed_to_strict("{ a: true } { b: \"false\" }  { some: 1234 }", "{\"a\":true}{\"b\":\"false\"}{\"some\":1234}", 42);

  // multiple docs with nested multiple docs in one string
  failed += test_relaxed_to_strict("{ a: { b: true, c: false }, d: { a: { b: 0 } } } { b: \"false\" }  { some: 1234 }", "{\"a\":{\"b\":true,\"c\":false},\"d\":{\"a\":{\"b\":0}}}{\"b\":\"false\"}{\"some\":1234}", 79);

  //////////////
  // with firstroot

  printf("test relaxed_to_strict with firstroot:\n");

  failed += test_relaxed_to_strict_firstroot("{ a: true } { b: \"false\" }  { some: 1234 }", "{\"a\":true}", 11);
  failed += test_relaxed_to_strict_firstroot("   { a: true } { b: \"false\" }  { some: 1234 }", "{\"a\":true}", 14);
  failed += test_relaxed_to_strict_firstroot("{a:true } { b: \"false\" }  { some: 1234 }", "{\"a\":true}", 9);
  failed += test_relaxed_to_strict_firstroot("{a:true}{b:\"false\"}{ some: 1234 }", "{\"a\":true}", 8);

  // multiple docs with nested multiple docs in one string
  failed += test_relaxed_to_strict_firstroot("{ a: { b: true, c: false }, d: { a: { b: 0 } } } { b: \"false\" }  { some: 1234 }", "{\"a\":{\"b\":true,\"c\":false},\"d\":{\"a\":{\"b\":0}}}", 48);

  // test incomplete documents
  failed += test_relaxed_to_strict_firstroot("{ a: true } { b:", "{\"a\":true}", 11);
  failed += test_relaxed_to_strict_firstroot("{ a: true  { b:", "{ a: true  { b:", -1);

  return failed;
}

int
test_relaxed_to_strict(const char *input, const char *exp, const int exp_exit)
{
  int exit;
  char str[MAXSTR];


  strcpy(str, input);
  if ((exit = relaxed_to_strict(str, MAXSTR, (char *)input, strlen(input), 0)) != exp_exit) {
    fprintf(stderr, "FAIL: %s = exit: %d, expected: %d\n", input, exit, exp_exit);
    return 1;
  }

  if (strcmp(str, exp) == 0) {
    printf("PASS: %s = \"%s\"\n", input, str);
    return 0;
  } else {
    fprintf(stderr, "FAIL: %s = \"%s\" instead of \"%s\"\n", input, str, exp);
    return 1;
  }

  return -1;
}

int
test_relaxed_to_strict_firstroot(const char *input, const char *exp, const int exp_exit)
{
  int exit;
  char str[MAXSTR];


  strcpy(str, input);
  if ((exit = relaxed_to_strict(str, MAXSTR, (char *)input, strlen(input), 1)) != exp_exit) {
    fprintf(stderr, "FAIL: %s = exit: %d, expected: %d\n", input, exit, exp_exit);
    return 1;
  }

  if (strcmp(str, exp) == 0) {
    printf("PASS: %s = \"%s\"\n", input, str);
    return 0;
  } else {
    fprintf(stderr, "FAIL: %s = \"%s\" instead of \"%s\"\n", input, str, exp);
    return 1;
  }

  return -1;
}
