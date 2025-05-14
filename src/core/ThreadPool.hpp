#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace RayTracer {

/**
 * @brief A simple thread pool for parallel task execution
 */
class ThreadPool {
 public:
  /**
   * @brief Constructor
   * @param numThreads Number of worker threads to create (default: number of
   * CPU cores - 1)
   */
  explicit ThreadPool(size_t numThreads = 0);

  /**
   * @brief Destructor (joins all threads)
   */
  ~ThreadPool();

  /**
   * @brief Add a task to the queue
   * @param func Function to execute
   * @param args Arguments to pass to the function
   * @return A future for the function's result
   */
  template <class F, class... Args>
  auto enqueue(F&& func, Args&&... args)
      -> std::future<typename std::invoke_result<F, Args...>::type>;

  /**
   * @brief Get the number of worker threads
   * @return Number of threads in the pool
   */
  size_t size() const;

  /**
   * @brief Get the number of tasks waiting in the queue
   * @return Queue size
   */
  size_t queueSize() const;

  /**
   * @brief Set the active flag (to pause/resume processing)
   * @param active Whether the pool should be active
   */
  void setActive(bool active);

  /**
   * @brief Check if the pool is active
   * @return True if active, false otherwise
   */
  bool isActive() const;

 private:
  std::vector<std::thread> _workers;         ///< Worker threads
  std::queue<std::function<void()>> _tasks;  ///< Task queue

  // Synchronization
  mutable std::mutex _queueMutex;      ///< Protects task queue
  std::condition_variable _condition;  ///< For notifying worker threads
  std::atomic<bool> _stop;             ///< Flag to stop threads
  std::atomic<bool> _active;           ///< Flag to pause/resume processing
};

// Template implementation (must be in header)
template <class F, class... Args>
auto ThreadPool::enqueue(F&& func, Args&&... args)
    -> std::future<typename std::invoke_result<F, Args...>::type> {
  using return_type = typename std::invoke_result<F, Args...>::type;

  // Create a packaged task
  auto task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(func), std::forward<Args>(args)...));

  // Get the future to return
  std::future<return_type> result = task->get_future();

  // Add the task to the queue
  {
    std::lock_guard<std::mutex> lock(_queueMutex);

    // Don't allow enqueueing after stopping the pool
    if (_stop) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }

    // Wrap the packaged task into a void function
    _tasks.emplace([task]() { (*task)(); });
  }

  // Notify a worker thread
  _condition.notify_one();
  return result;
}

}  // namespace RayTracer

#endif  // THREADPOOL_HPP_
