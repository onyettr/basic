10 REM BUBBLESORT FOR STRINGS
80 HOME : TEXT
90 DIM A$(8) : REM ARRAY WITH 8 STRINGS 
100 REM ASK FOR 8 STRINGS 
110 FOR I = 1 TO 8 
120 PRINT "TYPE A STRING: "; 
130 INPUT A$ (I) 
140 NEXT I 
150 REM PASS THROUGH 8 STRINGS, TESTING BY PAIRS 
160 F = 0 : REM RESET THE ORDER INDICATOR
170 FOR I = 1 TO 7 
180 IF A$(I) <= A$(I+1) THEN GOTO 240 
190 REM SWAP A$(I) AND A$(I+1) 
200 T$ = A$(I) 
210 A$(I) = A$(I+1) 
220 A$(I+1) = T$ 
230 F = 1 : REM ORDER WAS NOT PERFECT 
240 NEXT I 
250 REM F = 0 MEANS ORDER IS PERFECT 
260 IF F = 1 THEN GOTO 160 : REM TRY AGAIN 
270 PRINT : REM PRINT EMPTY LINE 
280 REM PRINT ORDERED STRINGS 
290 FOR I = 1 TO 8 
300 PRINT A$ (I) 
310 NEXT I
320 END 