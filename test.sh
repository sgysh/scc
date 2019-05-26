##!/bin/bash
try() {
  expected="$1"
  input="$2"

  ./scc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$expected expected, but got $actual"
    exit 1
  fi
}

try 0 'return 0;'
try 42 'return 42;'
try 21 'return 5+20-4;'
try 41 'return  12 + 34 - 5 ;'
try 47 'return 5+6*7;'
try 15 'return 5*(9-6);'
try 4 'return (3+5)/2;'
try 2 'return -3+5;'
try 2 'return -(3-5);'
try 15 'return 3*+5;'
try 1 'return 1+2==2+1;'
try 0 'return 1+2==4;'
try 1 'return 1+2!=4;'
try 0 'return 1+2!=2+1;'
try 0 'return 1+1!=2+1==0;'
try 0 'return (1==2)+1!=(1==1);'
try 1 'return 1>=0;'
try 0 'return 1<=0;'
try 1 'return 0>=0;'
try 1 'return 0>=1==1<=-1;'
try 1 'return 1>0;'
try 0 'return 1<0;'
try 1 'return 0>1==1<-1;'
try 2 'a=2;return a;'
try 10 'a=2;b=3+2;return a*b;'
try 4 'a=b=2;return a*b;'
try 5 'return 5; return 8;'
try 6 'foo = 1; bar = 2 + 3; return foo + bar;'
try 5 '_n_0 = 1; _n_1 = 2 + 3; return _n_0 * _n_1;'

echo OK
