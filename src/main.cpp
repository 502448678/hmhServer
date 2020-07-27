#include "mysql.h"
#include "TCPNet.h"
#include "TCPKernel.h"
#include "log.h"
pthread_mutex_t alock;
pthread_mutex_t rlock;

int main()
{
	pthread_mutex_init(&alock,NULL);
	pthread_mutex_init(&rlock,NULL);

	char LogPath[64] = "../log/mylog.log";
	Log::get_instance()->init(LogPath, 300, 20000);

	TCPKernel tk;
	tk.Open();
/*	if(tk.Open()==true)
	{
		while(1)
		{
			cout << "Server is running & write to log .."<<endl;	
			Log::get_instance()->flush();
			sleep(10);
		}

	}
*/
	Log::get_instance()->flush();
	tk.Close();
	return 0;
}

/*void *log(void* args)
{
	for(;;)
	{
		sleep(5);
		Log::get_instance()->flush();
	}
}


void *tk(void* args)
{
	TCPKernel tk;
	tk.Open();

}

int main()
{
	Log::get_instance()->init("../log/mylog.log", 300, 20000);

	pthread_t id_log,id_tk;
	
	pthread_create(&id_log,NULL,log,NULL);
	pthread_join(id_log,NULL);

	pthread_create(&id_tk,NULL,tk,NULL);
	pthread_join(id_tk,NULL);
	

	return 0;

}*/
