#pragma once

#include <scheduler/task.hpp>
#include <vector>
#include <utility>


using TaskId = int;


class Scheduler {
public:
  template<typename T>
  class FutureResult {
  public:
    FutureResult(TaskId id, Scheduler& parent) : parent_(parent), id_(id){};

    operator T() const {
      return parent_.evaluateFuture(*this);
    }

    int Id() const {
      return id_;
    }

  private:
    Scheduler& parent_;
    int id_;
  };

  template<typename Callable, typename... Args>
  int add(Callable fn, Args... args) {
    using ReturnType = decltype(fn(args...));

    tasks_.emplace_back(
      std::make_unique<Task<ReturnType>>(std::forward<Callable>(fn), std::forward<Args>(args)...)
    );

    return tasks_.size() - 1;
  }

  template<typename T>
  FutureResult<T> getFutureResult(TaskId task_id) {
    return FutureResult<T>{task_id, *this};
  };

  template<typename T>
  T evaluateFuture(const FutureResult<T> future) {
    return static_cast<Task<T>*>(tasks_.at(future.Id()).get())->EvaluateWithResult();
  };

  template<typename ReturnType>
  ReturnType getResult(TaskId id) {
    return static_cast<Task<ReturnType>*>(tasks_.at(id).get())->EvaluateWithResult();
  }

  void executeAll() {
    for (auto& task: tasks_) {
      task->Execute();
    }
  }


private:
  std::vector<std::unique_ptr<AbstractTask>> tasks_;
};
