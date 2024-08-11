#include "ThreadPool.h"

IMPLEMENT_SINGLETON(CThreadPool)

CThreadPool::CThreadPool()
{
}

CThreadPool::~CThreadPool()
{
}

HRESULT CThreadPool::Initialize(_uint _iThreadsNum)
{

    for (_uint i = 0; i < _iThreadsNum; ++i) {
        m_workers.emplace_back([&]() {
            /*�۾��� �߰��Ǳ���� ������ ����ߴٰ� �۾��� �߰��Ǹ� �����Ѵ�*/
            while (true) {

                std::function<void()> task;
                {
                    /*�Ӱ豸���� �ٸ� �����尡 �������� ���ϵ��� lock�� �Ǵ� -> task queue�� ����*/
                    unique_lock<std::mutex> lock(m_mutex);
                    /*Stop�̰ų� empty�� �ƴҶ����� ���*/
                    m_condition.wait(lock,
                        [&] { return m_isStop || !m_tasks.empty(); });
                    if (m_isStop && m_tasks.empty())
                        return;
                    /*�� �տ� �ִ� task�� pop�ؼ� ���´�*/
                    task = std::move(m_tasks.front());
                    m_tasks.pop();
                }
                /*task ����*/
                task();
            }

            }
        );
    }

    return S_OK;
}

//template<class F, class ...Args>
//auto CThreadPool::ThreadEnqueue(F&& f, Args && ...args) -> std::future<typename std::invoke_result<F,Args ...>::type>
//{
//    using return_type = typename std::result_of<F(Args...)>::type;
//
//    /*thread�� ó���ؾ� �� task*/
//    auto task = std::make_shared< std::packaged_task<return_type()> >(
//        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
//    );
//
//
//    std::future<return_type> res = task->get_future();
//    {
//        std::unique_lock<std::mutex> lock(m_mutex);
//
//        /*stop �϶��� �߰����� �۾��� �ϸ� �ȵǱ� ������ ����ó��*/
//        if (m_isStop)
//            throw std::runtime_error("enqueue on stopped ThreadPool");
//
//        m_tasks.emplace([task]() { (*task)(); });
//    }
//
//    m_condition.notify_one();
//
//    return res;
//}