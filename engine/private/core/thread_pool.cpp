/*
SDLEngine
Matt Hoyle
*/
#include "thread_pool.h"
#include "kernel/thread.h"
#include "kernel/assert.h"

namespace Core
{
	class ThreadPool::PooledThread
	{
	public:
		PooledThread(const std::string& name, ThreadPool* parent)
			: m_name(name)
			, m_parent(parent)
		{
		}
		~PooledThread()
		{
		}
		void Start()
		{
			SDE_ASSERT(m_parent != nullptr);
			auto threadFnc = [this]()
			{
				while (m_parent->m_stopRequested.Get() == 0)
				{
					m_parent->m_fn();
				}
				return 0;
			};
			m_thread.Create(m_name.c_str(), threadFnc);
		}
		void WaitForFinish()
		{
			m_thread.WaitForFinish();
		}
	private:
		Kernel::Thread m_thread;
		ThreadPool* m_parent;
		std::string m_name;
	};

	ThreadPool::ThreadPool()
		: m_stopRequested(0)
	{
	}

	ThreadPool::~ThreadPool()
	{
		Stop();
	}

	void ThreadPool::Start(const char* poolName, uint32_t threadCount, ThreadPoolFn threadfn)
	{
		SDE_ASSERT(m_threads.size() == 0);
		m_fn = threadfn;
		m_threads.reserve(threadCount);
		for (uint32_t t = 0;t < threadCount;++t)
		{
			char threadNameBuffer[256] = { '\0' };
			sprintf_s(threadNameBuffer, "%s_%d", poolName, t);
			auto thisThread = std::make_unique<PooledThread>(threadNameBuffer, this);
			SDE_ASSERT(thisThread);
			thisThread->Start();
			m_threads.push_back(std::move(thisThread));
		}
	}

	void ThreadPool::Stop()
	{
		m_stopRequested = true;
		for (auto& it : m_threads)
		{
			it->WaitForFinish();
			it = nullptr;
		}
		m_threads.clear();
	}
}