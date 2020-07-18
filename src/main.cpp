#include "mysql.h"
#include "TCPNet.h"
#include "TCPKernel.h"

int main()
{
	TCPKernel tk;
	tk.Open();
	return 0;
}
