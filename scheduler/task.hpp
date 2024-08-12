#pragma once

#include <scheduler/functional.hpp>

class GenericFunction {
public:
  template<typename T, typename... Args>
  GenericFunction(T fn, Args... args) 
    : fn_(std::make_shared<CallableHolder<decltype(fn(args...))(void)>>(
    [=]() {
      return fn(args...);
    }
  )) {}; 

  template<typename ReturnType>
  ReturnType Invoke() {
    return static_cast<CallableHolder<ReturnType(void)>*>(fn_.get())
    ->operator()();
  };

private:
  std::shared_ptr<void> fn_;
};


class AbstractTask {
public:
  virtual void Execute() = 0;
  virtual ~AbstractTask() = default;
};

template<typename ReturnType>
class Task : public AbstractTask {
public:
  template<typename Callable, typename... Args>
  Task(Callable c, Args... args): fn_([=](){
    return c(args...);
  }) {};

  void Execute() override {
    EvaluateWithResult();
  }

  ReturnType EvaluateWithResult() {
    if (is_done_) {
      return *static_cast<ReturnType*>(result_.get());
    }

    is_done_ = true;
    result_ = std::make_shared<ReturnType>(fn_.Invoke<ReturnType>());
    return *static_cast<ReturnType*>(result_.get());
  }

private:
  bool is_done_ = false;
  std::shared_ptr<void> result_;

  GenericFunction fn_;
};
