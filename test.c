#include "jsonify.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOUTPUT 1024

void test_relaxed_to_strict(const char *input, const char *expected);
void test_relaxed_to_strict_with_maxroot(const char *input, int maxroot, const char *expected);

int main()
{
  // one-dimensional
  test_relaxed_to_strict("1", "1");
  test_relaxed_to_strict("[]", "[]");
  test_relaxed_to_strict("[1]", "[1]");
  test_relaxed_to_strict("[1,2]", "[1,2]");

  // two-dimensional
  test_relaxed_to_strict("[[]]", "[[]]");
  test_relaxed_to_strict("[1,[]]", "[1,[]]");
  test_relaxed_to_strict("[[],1]", "[[],1]");
  test_relaxed_to_strict("[1,[2]]", "[1,[2]]");
  test_relaxed_to_strict("[[1],2]", "[[1],2]");
  test_relaxed_to_strict("[1,2,[3,4],5]", "[1,2,[3,4],5]");
  test_relaxed_to_strict("[1,[2,3,4],5,6]", "[1,[2,3,4],5,6]");

  // three-dimensional
  test_relaxed_to_strict("[[],1,[2,3],4,[5,[],[6,7],8]]", "[[],1,[2,3],4,[5,[],[6,7],8]]");

  // four-dimensional
  test_relaxed_to_strict("[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,73],6,7],8]]", "[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,73],6,7],8]]");

  // five-dimensional
  test_relaxed_to_strict("[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]", "[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]");

  // six-dimensional
  test_relaxed_to_strict("[[],[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]]", "[[],[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]]");

  // one-dimensional
  test_relaxed_to_strict("{a:true}", "{\"a\":true}");

  // two-dimensional
  test_relaxed_to_strict("[{a:true,b:false},[{c:true}]]", "[{\"a\":true,\"b\":false},[{\"c\":true}]]");

  // three-dimensional
  test_relaxed_to_strict("[{a:true,b:false},[{c:[2,3,{$some:true}]}]]", "[{\"a\":true,\"b\":false},[{\"c\":[2,3,{\"$some\":true}]}]]");

  test_relaxed_to_strict(
      "[{$project:{by:true,action:true,on:true,sid:true}},{$match:{action:{$in:['socketstart','socketstop']}}},{$group:{_id:{sid:\"$sid\",uid:\"$by\"},login:{$min:\"$on\"},logout:{$max:\"$on\"}}},{$project:{login:true,logout:true,duration:{$subtract:[\"$logout\",\"$login\"]}}},{$project:{uid:\"$_id.uid\",duration:true,yearWeek:{$dateToString:{format:'%Y%U',date:\"$login\"}}}},{$group:{_id:{uid:\"$uid\",yearWeek:\"$yearWeek\"},duration:{$sum:\"$duration\"}}},{$project:{duration:{$divide:[\"$duration\",60*1000]}}}]",
      "[{\"$project\":{\"by\":true,\"action\":true,\"on\":true,\"sid\":true}},{\"$match\":{\"action\":{\"$in\":[\"socketstart\",\"socketstop\"]}}},{\"$group\":{\"_id\":{\"sid\":\"$sid\",\"uid\":\"$by\"},\"login\":{\"$min\":\"$on\"},\"logout\":{\"$max\":\"$on\"}}},{\"$project\":{\"login\":true,\"logout\":true,\"duration\":{\"$subtract\":[\"$logout\",\"$login\"]}}},{\"$project\":{\"uid\":\"$_id.uid\",\"duration\":true,\"yearWeek\":{\"$dateToString\":{\"format\":\"%Y%U\",\"date\":\"$login\"}}}},{\"$group\":{\"_id\":{\"uid\":\"$uid\",\"yearWeek\":\"$yearWeek\"},\"duration\":{\"$sum\":\"$duration\"}}},{\"$project\":{\"duration\":{\"$divide\":[\"$duration\",60*1000]}}}]");

  // multiple docs in one string
  test_relaxed_to_strict("{ a: true } { b: \"false\" }  { some: 1234 }", "{\"a\":true}{\"b\":\"false\"}{\"some\":1234}");

  // multiple docs with nested multiple docs in one string
  test_relaxed_to_strict("{ a: { b: true, c: false }, d: { a: { b: 0 } } } { b: \"false\" }  { some: 1234 }", "{\"a\":{\"b\":true,\"c\":false},\"d\":{\"a\":{\"b\":0}}}{\"b\":\"false\"}{\"some\":1234}");

  //////////////
  // with maxroot

  test_relaxed_to_strict_with_maxroot("{ a: true } { b: \"false\" }  { some: 1234 }", 0, "{\"a\":true}{\"b\":\"false\"}{\"some\":1234}");
  test_relaxed_to_strict_with_maxroot("{ a: true } { b: \"false\" }  { some: 1234 }", 1, "{\"a\":true}");
  test_relaxed_to_strict_with_maxroot("{ a: true } { b: \"false\" }  { some: 1234 }", 2, "{\"a\":true}{\"b\":\"false\"}");
  test_relaxed_to_strict_with_maxroot("{ a: true } { b: \"false\" }  { some: 1234 }", 3, "{\"a\":true}{\"b\":\"false\"}{\"some\":1234}");
  test_relaxed_to_strict_with_maxroot("{ a: true } { b: \"false\" }  { some: 1234 }", 4, "{\"a\":true}{\"b\":\"false\"}{\"some\":1234}");

  // multiple docs with nested multiple docs in one string
  test_relaxed_to_strict_with_maxroot("{ a: { b: true, c: false }, d: { a: { b: 0 } } } { b: \"false\" }  { some: 1234 }", 2, "{\"a\":{\"b\":true,\"c\":false},\"d\":{\"a\":{\"b\":0}}}{\"b\":\"false\"}");

  return 0;
}

void
test_relaxed_to_strict(const char *input, const char *expected)
{
  char output[MAXOUTPUT];

  if (relaxed_to_strict(output, MAXOUTPUT, (char *)input, strlen(input), 0) == -1)
    fatal("jsonify error");

  if (strcmp(expected, output) == 0)
    printf("OK: %s\n", expected);
  else
    fprintf(stderr, "ERROR: %s  !=  %s\n", expected, output);
}

void
test_relaxed_to_strict_with_maxroot(const char *input, int maxroot, const char *expected)
{
  char output[MAXOUTPUT];

  if (relaxed_to_strict(output, MAXOUTPUT, (char *)input, strlen(input), maxroot) == -1)
    fatal("jsonify error");

  if (strcmp(expected, output) == 0)
    printf("OK: %s\n", expected);
  else
    fprintf(stderr, "ERROR: %s  !=  %s\n", expected, output);
}
