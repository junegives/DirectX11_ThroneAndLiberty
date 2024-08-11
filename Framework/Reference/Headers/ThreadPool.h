#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CThreadPool
{
	DECLARE_SINGLETON(CThreadPool)

public:
	CThreadPool();
	~CThreadPool();

public:
	/*������ ������ �޾Ƽ� �׸�ŭ �̸� �����صδ� Init �Լ�*/
	HRESULT Initialize(_uint _iThreadsNum);

	/*tasks�� �ؾ��� �Լ��� �߰��ϴ� Funtion*/

	/*TestCase 1 */
	//template<class F, class... Args>
	//auto ThreadEnqueue(F&& f, Args&&... args)
	//	-> std::future<typename std::invoke_result<F,Args...>::type>;


	template<class F, class... Args>
	auto ThreadEnqueue(F&& f, Args&&... args)
		-> std::future<typename std ::invoke_result<F, Args...>::type>;

private:
	/*�̸� �����س��� �Ŀ� ����� ������ ����*/
	vector<thread> m_workers;

	/*��������� ó���ؾ� �� task��*/
	std::queue<function<void()>> m_tasks;

	std::mutex m_mutex;
	std::condition_variable m_condition;
	_bool m_isStop = false;

};

END

//
//template<class F, class ...Args>
//inline std::future<typename std::invoke_result_t<F(Args...)>::type> CThreadPool::ThreadEnqueue(F&& f, Args && ...args)
//{
//	using return_type = typename std::invoke_result_t<F(Args...)>::type;
//	auto job = std::make_shared<std::packaged_task<return_type()>>(
//		std::bind(std::forward<F>(f), std::forward<Args>(args)...));
//	std::future<return_type> job_result_future = job->get_future();
//	{
//		std::lock_guard<std::mutex> lock(m_mutex);
//		m_tasks.push([job]() { (*job)(); });
//	}
//	m_condition.notify_one();
//
//	return job_result_future;
//}

#endif

