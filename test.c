#include "jsonify.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOUTPUT 1024

int run_test(const char *input, const char *expected);

int main()
{
  // one-dimensional
  run_test("1", "1");
  run_test("[]", "[]");
  run_test("[1]", "[1]");
  run_test("[1,2]", "[1,2]");

  // two-dimensional
  run_test("[[]]", "[[]]");
  run_test("[1,[]]", "[1,[]]");
  run_test("[[],1]", "[[],1]");
  run_test("[1,[2]]", "[1,[2]]");
  run_test("[[1],2]", "[[1],2]");
  run_test("[1,2,[3,4],5]", "[1,2,[3,4],5]");
  run_test("[1,[2,3,4],5,6]", "[1,[2,3,4],5,6]");

  // three-dimensional
  run_test("[[],1,[2,3],4,[5,[],[6,7],8]]", "[[],1,[2,3],4,[5,[],[6,7],8]]");

  // four-dimensional
  run_test("[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,73],6,7],8]]", "[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,73],6,7],8]]");

  // five-dimensional
  run_test("[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]", "[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]");

  // six-dimensional
  run_test("[[],[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]]", "[[],[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]]");

  // one-dimensional
  run_test("{a:true}", "{\"a\":true}");

  // two-dimensional
  run_test("[{a:true,b:false},[{c:true}]]", "[{\"a\":true,\"b\":false},[{\"c\":true}]]");

  // three-dimensional
  run_test("[{a:true,b:false},[{c:[2,3,{$some:true}]}]]", "[{\"a\":true,\"b\":false},[{\"c\":[2,3,{\"$some\":true}]}]]");

  run_test(
      "[{$project:{by:true,action:true,on:true,sid:true}},{$match:{action:{$in:['socketstart','socketstop']}}},{$group:{_id:{sid:\"$sid\",uid:\"$by\"},login:{$min:\"$on\"},logout:{$max:\"$on\"}}},{$project:{login:true,logout:true,duration:{$subtract:[\"$logout\",\"$login\"]}}},{$project:{uid:\"$_id.uid\",duration:true,yearWeek:{$dateToString:{format:'%Y%U',date:\"$login\"}}}},{$group:{_id:{uid:\"$uid\",yearWeek:\"$yearWeek\"},duration:{$sum:\"$duration\"}}},{$project:{duration:{$divide:[\"$duration\",60*1000]}}}]",
      "[{\"$project\":{\"by\":true,\"action\":true,\"on\":true,\"sid\":true}},{\"$match\":{\"action\":{\"$in\":[\"socketstart\",\"socketstop\"]}}},{\"$group\":{\"_id\":{\"sid\":\"$sid\",\"uid\":\"$by\"},\"login\":{\"$min\":\"$on\"},\"logout\":{\"$max\":\"$on\"}}},{\"$project\":{\"login\":true,\"logout\":true,\"duration\":{\"$subtract\":[\"$logout\",\"$login\"]}}},{\"$project\":{\"uid\":\"$_id.uid\",\"duration\":true,\"yearWeek\":{\"$dateToString\":{\"format\":\"%Y%U\",\"date\":\"$login\"}}}},{\"$group\":{\"_id\":{\"uid\":\"$uid\",\"yearWeek\":\"$yearWeek\"},\"duration\":{\"$sum\":\"$duration\"}}},{\"$project\":{\"duration\":{\"$divide\":[\"$duration\",60*1000]}}}]");

  return 0;
}

int
run_test(const char *input, const char *expected)
{
  char output[MAXOUTPUT];

  if (from_loose_to_strict(output, MAXOUTPUT, (char *)input, strlen(input)) == -1)
    fatal("jsonify error");

  if (strcmp(expected, output) == 0)
    printf("OK: %s\n", expected);
  else
    fprintf(stderr, "ERROR: %s  !=  %s\n", expected, output);
}
