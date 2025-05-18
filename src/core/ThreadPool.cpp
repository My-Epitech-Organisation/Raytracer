/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** ThreadPool
*/

/**
 * @file ThreadPool.cpp
 * @brief Implementation of a thread pool for parallel task execution to
 * optimize rendering performance
 * @author @paul-antoine
 * @date 2025-05-16
 * @version 1.0
 */

#include "ThreadPool.hpp"
#include <algorithm>
#include <thread>

namespace RayTracer {

ThreadPool::ThreadPool(size_t numThreads) : _stop(false), _active(true) {
  // If numThreads is 0, use hardware concurrency minus 1 (leave one for OS)
  if (numThreads == 0) {
    numThreads = std::max(1u, std::thread::hardware_concurrency() - 1);
  }

  // Create worker threads
  for (size_t i = 0; i < numThreads; ++i) {
    _workers.emplace_back([this] {
      while (true) {
        std::function<void()> task;

        {
          // Wait for a task or stop signal
          std::unique_lock<std::mutex> lock(this->_queueMutex);
          this->_condition.wait(lock, [this] {
            return this->_stop || (!this->_tasks.empty() && this->_active);
          });

          // Exit if stopping and no more tasks
          if (this->_stop && this->_tasks.empty()) {
            return;
          }

          // If not active, keep waiting
          if (!this->_active) {
            continue;
          }

          // Get next task
          task = std::move(this->_tasks.front());
          this->_tasks.pop();
        }

        // Execute the task
        task();
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  {
    std::lock_guard<std::mutex> lock(_queueMutex);
    _stop = true;
  }

  // Wake up all threads
  _condition.notify_all();

  // Join all threads
  for (std::thread& worker : _workers) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

size_t ThreadPool::size() const {
  return _workers.size();
}

size_t ThreadPool::queueSize() const {
  std::lock_guard<std::mutex> lock(_queueMutex);
  return _tasks.size();
}

void ThreadPool::setActive(bool active) {
  _active = active;
  if (active) {
    _condition.notify_all();
  }
}

bool ThreadPool::isActive() const {
  return _active;
}

}  // namespace RayTracer
