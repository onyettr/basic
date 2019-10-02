#include <stdio.h>
#include "basic.h"

bool Verbose  = false;    /* This is required as we do not include parsecommandline.c file */

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

   printf("TokenGetNumber_one_digit_success\n");

   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   Pattern[0] = '1';
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetNumber(&Bufferp, Tokenp, false);

   fail_unless(ReturnToken == TOKEN_DIGIT, "TokenGetNumber - wrong token return");
   fail_unless(Tokenp[0] == '1',           "TokenGetNumber - didnt copy token  ");
   fail_unless(strlen(Tokenp) == 1,        "TokenGetNumber - wrong size token  ");

#test TokenGetNumber_one_digit_negative_success
   Token_t ReturnToken;
   char TokenBuffer[10];
   char *Bufferp;
   char *Tokenp;
   char Pattern[3];

   printf("TokenGetNumber_one_digit_negative_success\n");
   
   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   Pattern[0] = '-';
   Pattern[1] = '1';
   Pattern[2] = '0';   
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetNumber(&Bufferp, Tokenp, true);

   fail_unless(ReturnToken == TOKEN_DIGIT, "TokenGetNumber - wrong token return");
   fail_unless(Tokenp[0] == '-',           "TokenGetNumber - didnt copy token[0]");
   fail_unless(Tokenp[1] == '1',           "TokenGetNumber - didnt copy token[1]");
   fail_unless(Tokenp[2] == '0',           "TokenGetNumber - didnt copy token[2]");      

#test TokenGetNumber_one_digit_failure_not_a_digit
   Token_t ReturnToken;
   char TokenBuffer[10];
   char *Bufferp;
   char *Tokenp;
   char Pattern[1];

   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   Pattern[0] = '?';
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetNumber(&Bufferp, Tokenp, false);
   printf("Token = %s\n", TokenGetStringType(ReturnToken));
   
   fail_unless(ReturnToken == TOKEN_DIGIT, "TokenGetNumber - wrong token return");

#test TokenGetNumber_one_floating_point
   Token_t ReturnToken;
   char TokenBuffer[10];
   char *Bufferp;
   char *Tokenp;
   char Pattern[4];

   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   Pattern[0] = '1';
   Pattern[1] = '.';
   Pattern[2] = '2';
   Pattern[3] = '\0';
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetNumber(&Bufferp, Tokenp, false);
   printf("Token = %s\n", TokenGetStringType(ReturnToken));
   
   fail_unless(ReturnToken == TOKEN_DIGIT, "TokenGetNumber - wrong token return");

#test TokenGetWord_word_success
   Token_t ReturnToken;
   char TokenBuffer[10];
   char *Bufferp;
   char *Tokenp;
   char Pattern[10];

   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   strcpy(Pattern, "Word");
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetWord(&Bufferp, Tokenp);
   printf("Token = %s\n", TokenGetStringType(ReturnToken));
   
   fail_unless(ReturnToken == TOKEN_WORD, "TokenGetWord - wrong token return");
   fail_unless(Tokenp[0] == 'W',          "TokenGetWord- didnt copy token  ");
   fail_unless(strlen(Tokenp) == 4,       "TokenGetWord - wrong size token  ");

#test TokenGetWord_word_keyword_success
   Token_t ReturnToken;
   char TokenBuffer[10];
   char *Bufferp;
   char *Tokenp;
   char Pattern[10];

   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   strcpy(Pattern, "REM");
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetWord(&Bufferp, Tokenp);
   printf("Token = %s\n", TokenGetStringType(ReturnToken));
   
   fail_unless(ReturnToken == TOKEN_REM, "TokenGetWord - wrong token return");
   fail_unless(Tokenp[0] == 'R',          "TokenGetWord- didnt copy token  ");
   fail_unless(strlen(Tokenp) == 3,       "TokenGetWord - wrong size token  ");

#test TokenGetString_String_success
   Token_t ReturnToken;
   char TokenBuffer[10];
   char *Bufferp;
   char *Tokenp;
   char Pattern[10];

   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   Pattern[0] = '"';
   Pattern[1] = 'A';
   Pattern[2] = 'B';
   Pattern[3] = '"';
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetString(&Bufferp, Tokenp);
   printf("Token = %s\n", TokenGetStringType(ReturnToken));
   
   fail_unless(ReturnToken == TOKEN_STRING, "TokenGetString - wrong token return");
   fail_unless(Tokenp[0] == '"',            "TokenGetString- didnt copy token  ");

#test TokenGetSpecial_success
   Token_t ReturnToken;
   char TokenBuffer[10];
   char *Bufferp;
   char *Tokenp;
   char Pattern[10];

   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   Pattern[0] = '?';
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetSpecial(&Bufferp, Tokenp);
   printf("Token = %s\n", TokenGetStringType(ReturnToken));
   
   fail_unless(ReturnToken == TOKEN_QUESTION_MARK, "TokenGetSpecial - wrong token return");
   fail_unless(Tokenp[0]   == '?',                 "TokenGetSpecial - didnt copy token  ");

#test TokenGetSpecial_logic_NE_success
   Token_t ReturnToken;
   char TokenBuffer[10];
   char *Bufferp;
   char *Tokenp;
   char Pattern[10];

   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   Pattern[0] = '<';
   Pattern[1] = '>';
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetSpecial(&Bufferp, Tokenp);
   printf("Token = %s\n", TokenGetStringType(ReturnToken));
   
   fail_unless(ReturnToken == TOKEN_NE, "TokenGetSpecial - wrong token return");
   fail_unless(Tokenp[0]   == '<',      "TokenGetSpecial - didnt copy token  ");

#test TokenGetSpecial_logic_LE_success
   Token_t ReturnToken;
   char TokenBuffer[10];
   char *Bufferp;
   char *Tokenp;
   char Pattern[10];

   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   Pattern[0] = '<';
   Pattern[1] = '=';
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetSpecial(&Bufferp, Tokenp);
   printf("Token = %s\n", TokenGetStringType(ReturnToken));
   
   fail_unless(ReturnToken == TOKEN_LE, "TokenGetSpecial - wrong token return");
   fail_unless(Tokenp[0]   == '<',      "TokenGetSpecial - didnt copy token  ");

#test TokenGetSpecial_logic_GE_success
   Token_t ReturnToken;
   char TokenBuffer[10];
   char *Bufferp;
   char *Tokenp;
   char Pattern[10];

   memset(TokenBuffer, '\0',sizeof(TokenBuffer));
   
   Pattern[0] = '>';
   Pattern[1] = '=';
   Bufferp = Pattern;
   Tokenp  = TokenBuffer;
   
   ReturnToken = TokenGetSpecial(&Bufferp, Tokenp);
   printf("Token = %s\n", TokenGetStringType(ReturnToken));
   
   fail_unless(ReturnToken == TOKEN_GE, "TokenGetSpecial - wrong token return");
   fail_unless(Tokenp[0]   == '>',      "TokenGetSpecial - didnt copy token  ");
