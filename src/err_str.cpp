#include "err_str.h"
#include "log.h"

void err_str(const char* str,int err)
{
	LOG_ERROR("%s%d\n",str,err);
    perror(str);
//	exit(err);
}
