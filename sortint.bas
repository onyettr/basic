10 REM BUBBLESORT
80 HOME : TEXT
90 DIM A(8) : REM ARRAY WITH MAX 8 ELEMENTS 
100 REM ASK FOR 8 NUMBERS 
110 FOR I = 1 TO 8 
120 PRINT "TYPE A NUMBER: "; 
130 INPUT A (I) 
140 NEXT I 
150 REM PASS THROUGH 8 NUMBERS, TESTING BY PAIRS 
160 F = 0 : REM RESET THE ORDER INDICATOR
170 FOR I = 1 TO 7 : REM NOTE THAT ENDING INDEX IS 8 MINUS 1
180 IF A(I) <= A(I+1) THEN GOTO 240 
190 REM SWAP A (I) AND A (I+1) 
200 T = A(I) 
210 A(I) = A(I+1) 
220 A(I+1) = T 
230 F = 1 : REM ORDER WAS NOT PERFECT 
240 NEXT I 
250 REM F = 0 MEANS ORDER IS PERFECT 
260 IF F = 1 THEN GOTO 160 : REM TRY AGAIN 
270 PRINT : REM PRINT EMPTY LINE 
280 REM PRINT ORDERED NUMBERS 
290 FOR I = 1 TO 8 
300 PRINT A (I) 
310 NEXT I
320 END 