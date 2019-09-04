#include <stdio.h>
#include "basic.h"

#test listersource_fail
   int32_t ErrorCode = SUCCESS;

   ErrorCode = ListerSource(NULL);
   fail_unless(ErrorCode == ERROR_FILE_OPEN_FAILURE, "negative open failed");   


