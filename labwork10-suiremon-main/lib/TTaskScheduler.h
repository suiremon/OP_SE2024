#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <list>

class any {
    struct Base {
        virtual Base *clone() { 
            return nullptr; 
        };
        virtual ~Base() = default;
    };

    template<typename T>
    struct Derived : public Base {
        explicit Derived(const T &object) : object(object) {}
        
        Base *clone() override {
            return new Derived<T>(object);
        }

        T object;
    };

    Base* ptr = nullptr;
public:
    any() = default;
    template<typename T>
    explicit any(const T &other): ptr(new Derived<T>(other)) {}
    any(const any &other) : ptr(ptr->clone()) {}
    ~any() {
        delete ptr;
    };

    template<typename T>
    any &operator=(const T &other) {
        delete ptr;
        ptr = new Derived<T>(other);
        return *this;
    }

    template<typename T>
    T get() {
        return static_cast<Derived<T> *>(ptr)->object;
    }

    bool hasValue() {
        return ptr != nullptr;
    }
};

class Task {
private:
public:
    Task() = default;
    virtual ~Task() = default;
    virtual void getValue() {};

    template<typename T>
    T getTaskValue() {
        if (_value.hasValue()) {
            return _value.get<T>();
        }
        getValue();
        return _value.get<T>();
    }

    any _value;
};

template<typename T>
struct FutureResult {
    explicit FutureResult(Task *task) : _task(task) {}

    Task *_task;
};

template<typename T>
class TaskArg {
private:
    T _argument;
public:
    TaskArg() = default;
    explicit TaskArg(const T &argument) : _argument(argument) {}
    TaskArg<T> &operator=(const TaskArg<T> &other) {
        _argument = other._argument;
        return *this;
    }

    T getArgument() {
        return _argument;
    }
};

template<typename T>
class TaskArg<FutureResult<T>> {
private:
    FutureResult<T> _argument;
public:
    TaskArg() = default;
    explicit TaskArg(const FutureResult<T> &argument) : _argument(argument) {}
    TaskArg<FutureResult<T>> &operator=(const TaskArg<FutureResult<T>> &other) {
        _argument = other._argument;
        return *this;
    }

    T getArgument() {
        return _argument._task->template getTaskValue<T>();
    }
};

template<typename Function, typename T, typename U>
class TwoArgumentTask : public Task {
public:
    TwoArgumentTask(const Function &func, const T &firstArg, const U &secondArg) : _func(func), _firstArg(firstArg),
                                                                                   _secondArg(secondArg) {};

    void getValue() override {
        _value = _func(_firstArg.getArgument(), _secondArg.getArgument());
    }

    Function _func;
    TaskArg<T> _firstArg;
    TaskArg<U> _secondArg;
};

template<typename Function, typename T>
class OneArgumentTask : public Task {
public:
    OneArgumentTask(const Function &func, const T &arg) : _func(func) {
        _arg = TaskArg<T>(arg);
    }

    void getValue() override {
        _value = _func(_arg.getArgument());
    }

    Function _func;
    TaskArg<T> _arg;
};

template<typename Function>
class ZeroArgumentTask : public Task {
public:
    explicit ZeroArgumentTask(const Function &func) : _func(func) {};
    Function _func;

    void getValue() override {
        _value = _func();
    }
};

struct TaskOrder {
public:
    explicit TaskOrder(size_t id) : _num(id) {};
    ~TaskOrder() = default;
    TaskOrder& operator=(const TaskOrder& other) = default;

    size_t _num;
};

class TTaskScheduler {
private:
    std::vector<std::unique_ptr<Task>> _tasks;

public:
    TTaskScheduler() = default;
    ~TTaskScheduler() = default;
    TTaskScheduler &operator=(const TTaskScheduler &other) = default;

    template<typename T>
    TaskOrder add(const T &func) {
        _tasks.emplace_back(new ZeroArgumentTask<T>(func));

        return TaskOrder(_tasks.size() - 1);
    };
    
    template<typename T, typename U>
    TaskOrder add(const T &func, const U &firstArg) {
        _tasks.emplace_back(new OneArgumentTask<T, U>(func, firstArg));

        return TaskOrder(_tasks.size() - 1);
    };

    template<typename T, typename U, typename K>
    TaskOrder add(const T &func, const U &firstArg, const K &secondArg) {
        _tasks.emplace_back(new TwoArgumentTask<T, U, K>(func, firstArg, secondArg));

        return TaskOrder(_tasks.size() - 1);
    };

    template<typename T>
    FutureResult<T> getFutureResult(const TaskOrder &task) {
        return FutureResult<T>(_tasks[task._num].get());
    };

    template<typename T>
    T getResult(const TaskOrder &id) {
        return _tasks[id._num]->template getTaskValue<T>();
    };

    void executeAll() {
        for (auto &task: _tasks) {
            task->getValue();
        }
    };
};