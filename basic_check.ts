#include <stdio.h>
#include "basic.h"

/**
 * Tokenizer API test harness
 *  TokenGetNumber
 */
#test TokenGetNumber_one_digit_success
   Token_t ReturnToken;
   char TokenBuffer[10];
   char *Bufferp;
   char *Tokenp;
   char Pattern[1];

   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   Pattern[0] = '1';
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetNumber(&Bufferp, Tokenp);

   fail_unless(ReturnToken == TOKEN_DIGIT, "TokenGetNumber - wrong token return");
   fail_unless(Tokenp[0] == '1',           "TokenGetNumber - didnt copy token  ");
   fail_unless(strlen(Tokenp) == 1,        "TokenGetNumber - wrong size token  ");

#test TokenGetNumber_one_digit_failure
   Token_t ReturnToken;
   char TokenBuffer[10];
   char *Bufferp;
   char *Tokenp;
   char Pattern[1];

   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   Pattern[0] = '?';
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetNumber(&Bufferp, Tokenp);

   fail_unless(ReturnToken == TOKEN_DIGIT, "TokenGetNumber - wrong token return");
//   fail_unless(Tokenp[0] == '1',           "TokenGetNumber - didnt copy token  ");
//   fail_unless(strlen(Tokenp) == 1,        "TokenGetNumber - wrong size token  ");      