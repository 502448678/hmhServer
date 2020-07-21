#include "mysql.h"
#include "TCPNet.h"
#include "TCPKernel.h"
#include "log.h"
int main()
{
	Log::get_instance()->init("../log/mylog.log", 300, 20000);

	TCPKernel tk;
	tk.Open();
	return 0;
}
