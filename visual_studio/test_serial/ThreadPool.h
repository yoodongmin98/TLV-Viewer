#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>


class ThreadPool
{
public:
	static ThreadPool* TP;
	ThreadPool();
	ThreadPool(size_t numThreads);

	~ThreadPool();


	void AddWork(std::function<void()> _function);
	const size_t GetTaskSize()
	{
		return tasks.empty() ? 0 : tasks.size();
	}
	std::queue<std::function<void()>> GetTasks()
	{
		return tasks;
	}
	std::vector<std::thread>& GetWorkers()
	{
		return Worker;
	}
	void Resize(size_t numThreads);
protected:
	void WorkerThread();
private:
	//���ư� �������
	std::vector<std::thread> Worker;
	std::mutex QueueMutex;
	std::queue<std::function<void()>> tasks;
	std::condition_variable condition;
	std::atomic<bool> stop;
};