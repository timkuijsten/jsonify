#!/bin/sh

function run() {
  for i in $2; do
    if [ $(echo "$i" | ./a.out 2>/dev/null | fgrep "$i") ]; then
      echo OK "$1": "$i"
    else
      echo ERROR "$1": $(echo "$i" | ./a.out 2>/dev/null | fgrep -v "$i") \\t != "$i" 1>&2
    fi
  done
}

# one-dimensional
run 1 '1  []  [1]  [1,2]'

# two-dimensional
run 2 '[[]]  [1,[]]  [[],1]  [1,[2]]  [[1],2]  [1,2,[3,4],5]  [1,[2,3,4],5,6]'

# three-dimensional
run 3 '[[],1,[2,3],4,[5,[],[6,7],8]]'

# four-dimensional
run 4 '[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,73],6,7],8]]'

# five-dimensional
run 5 '[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]'

# six-dimensional
run 6 '[[],[[[[],[],2],[]],1,[2,3],4,[5,[],[[23,5],[23],[23,[23]],6,7],8]]]'

# one-dimensional
run 1 '{a:true}'

# two-dimensional
run 2 '[{a:true,b:false},[{c:true}]]'

# three-dimensional
run 3 '[{a:true,b:false},[{c:[2,3,{$some:true}]}]]'

# three-dimensional
run 3 '[{a:true,b:false},[{c:[2,3,{$some:true}]}]]'

run x '[{$project:{by:true,action:true,on:true,sid:true}},{$match:{action:{$in:[''socketstart'',''socketstop'']}}},{$group:{_id:{sid:"$sid",uid:"$by"},login:{$min:"$on"},logout:{$max:"$on"}}},{$project:{login:true,logout:true,duration:{$subtract:["$logout","$login"]}}},{$project:{uid:"$_id.uid",duration:true,yearWeek:{$dateToString:{format:''%Y%U'',date:"$login"}}}},{$group:{_id:{uid:"$uid",yearWeek:"$yearWeek"},duration:{$sum:"$duration"}}},{$project:{duration:{$divide:["$duration",60*1000]}}}]'
