HOMEWORK 6 CONTEST: INVERSE WORD SEARCH


NAME:  < Canshi Wei >


OPTIMIZATIONS:
What did you do?
1. check if the delete_word immediatly after one puzzle generated
2. add the puzzle to the 'collection' immerdiatly after generated and check word, reduce the cases needed to measure 
3. check if the delete_word immediatly after a vacant point be filled with alphabelt
4. use set to avoid repeated puzzle, rather using for loop in for loop to compare each other




TEST CASE SUMMARY:
How did your program perform on the different test cases?  Summarize
the running times.  (It's ok if it didn't finish the harder examples.)
What new test cases did you create and how did it perform on those
tests?
 
                running time
puzzle1.txt     0.01627
puzzle2.txt     0.0159
puzzle3.txt     0.01967
puzzle4.txt     0.01551
puzzle5.txt     0.01572
puzzle6.txt     0.01657
puzzle7.txt     0.05769
puzzle8.txt     0.5787

weic2_1.txt
3 3
+ xxx
+ xxx
+ oo
+ oo
- xoo
- oxx
- ooo
- a
- b
- c
- d
- e
- f
- g
- h
- i
- j
- k
- l
- m
- n
- p
- q
- r
- s
- t
- u
- v
- w
- y
- z

expected out put:
running time: 0.4535s
1 solution(s)
Board:
  xox
  oxo
  xox
  
weic2_2.txt
13 3
+ abcdefghijklm
+ nopqrstuvwxyz
+ xxxxxxxxxxxxx

expected output:
running time: 0.0134s
24 solution(s)
Board:
  abcdefghijklm
  nopqrstuvwxyz
  xxxxxxxxxxxxx
Board:
  abcdefghijklm
  xxxxxxxxxxxxx
  nopqrstuvwxyz
Board:
  abcdefghijklm
  xxxxxxxxxxxxx
  zyxwvutsrqpon
Board:
  abcdefghijklm
  zyxwvutsrqpon
  xxxxxxxxxxxxx
Board:
  mlkjihgfedcba
  nopqrstuvwxyz
  xxxxxxxxxxxxx
Board:
  mlkjihgfedcba
  xxxxxxxxxxxxx
  nopqrstuvwxyz
Board:
  mlkjihgfedcba
  xxxxxxxxxxxxx
  zyxwvutsrqpon
Board:
  mlkjihgfedcba
  zyxwvutsrqpon
  xxxxxxxxxxxxx
Board:
  nopqrstuvwxyz
  abcdefghijklm
  xxxxxxxxxxxxx
Board:
  nopqrstuvwxyz
  mlkjihgfedcba
  xxxxxxxxxxxxx
Board:
  nopqrstuvwxyz
  xxxxxxxxxxxxx
  abcdefghijklm
Board:
  nopqrstuvwxyz
  xxxxxxxxxxxxx
  mlkjihgfedcba
Board:
  xxxxxxxxxxxxx
  abcdefghijklm
  nopqrstuvwxyz
Board:
  xxxxxxxxxxxxx
  abcdefghijklm
  zyxwvutsrqpon
Board:
  xxxxxxxxxxxxx
  mlkjihgfedcba
  nopqrstuvwxyz
Board:
  xxxxxxxxxxxxx
  mlkjihgfedcba
  zyxwvutsrqpon
Board:
  xxxxxxxxxxxxx
  nopqrstuvwxyz
  abcdefghijklm
Board:
  xxxxxxxxxxxxx
  nopqrstuvwxyz
  mlkjihgfedcba
Board:
  xxxxxxxxxxxxx
  zyxwvutsrqpon
  abcdefghijklm
Board:
  xxxxxxxxxxxxx
  zyxwvutsrqpon
  mlkjihgfedcba
Board:
  zyxwvutsrqpon
  abcdefghijklm
  xxxxxxxxxxxxx
Board:
  zyxwvutsrqpon
  mlkjihgfedcba
  xxxxxxxxxxxxx
Board:
  zyxwvutsrqpon
  xxxxxxxxxxxxx
  abcdefghijklm
Board:
  zyxwvutsrqpon
  xxxxxxxxxxxxx
  mlkjihgfedcba








