10 REM compute the sum of integers from 1 to 10 with FOR loop
20 TEXT : HOME : PR#0 
30 FOR N=1 TO 10
40 LET S = S + N
50 PRINT N,S
70 NEXT N
80 PRINT : REM print empty line
90 PRINT "Final sum:";S
100 END