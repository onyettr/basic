10 REM a Test
310 IF (A > 0) THEN GOTO 210
200 LET HEX$ = "" : REM HEX$ will return hex string of variable A
210 LET B = A - INT (A/16) * 16
220 IF B < 10 THEN H$ = STR$(B)
230 IF B = 10 THEN H$ = "A"
240 IF B = 11 THEN H$ = "B"
250 IF B = 12 THEN H$ = "C"
260 IF B = 13 THEN H$ = "D"
270 IF B = 14 THEN H$ = "E"
320 RETURN
330 REM 
