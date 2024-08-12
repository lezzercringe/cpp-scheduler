#pragma once

#include <memory>

template<typename T>
class CallableHolder;

template<typename ReturnType, typename... Args>
class CallableHolder<ReturnType(Args...)> {
public:
    template<typename Fn>
    CallableHolder(Fn fn) 
      : callable_(std::make_unique<Callable<Fn>>(fn)) {}

    template<typename T>
    CallableHolder& operator=(T t) {
        callable_ = std::make_unique<Callable<T>>(t);
        return *this;
    }

    ReturnType operator()(Args... args) const {
        return callable_->Invoke(std::forward<Args>(args)...);
    }

private:
    class ICallable {
    public:
        virtual ~ICallable() = default;
        virtual ReturnType Invoke(Args...) = 0;
    };

    template<typename T>
    class Callable final: public ICallable {
    T fn_;

    public:
        Callable(const T& t)
                : fn_(t) { }

        ReturnType Invoke(Args... args) override {
            return fn_(args...);
        }
    };

    std::unique_ptr<ICallable> callable_;
};


