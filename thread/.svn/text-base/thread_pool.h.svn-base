#include <vector>

class CTask
{
protected:
	string m_strTaskName;  //任务的名称
	void* m_ptrData;       //要执行的任务的具体数据
public:
	CTask(){}
	CTask(string taskName)
	{
		this->m_strTaskName = taskName;
		m_ptrData = NULL;
	}
	virtual int Run()= 0;
	void SetData(void* data);    //设置任务数据
};

class CThreadPool
{
private:
	vector<CTask*> m_vecTaskList;         //任务列表
	int m_iThreadNum;                            //线程池中启动的线程数
	static vector<pthread_t> m_vecIdleThread;   //当前空闲的线程集合
	static vector<pthread_t> m_vecBusyThread;   //当前正在执行的线程集合
	static pthread_mutex_t m_pthreadMutex;    //线程同步锁
	static pthread_cond_t m_pthreadCond;    //线程同步的条件变量
protected:
	static void* ThreadFunc(void * threadData); //新线程的线程函数
	static int MoveToIdle(pthread_t tid);   //线程执行结束后，把自己放入到空闲线程中
	static int MoveToBusy(pthread_t tid);   //移入到忙碌线程中去
	int Create();          //创建所有的线程
public:
	CThreadPool(int threadNum);
	int AddTask(CTask *task);      //把任务添加到线程池中
	int StopAll();
};

