#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>

#include <functional>

namespace ThreadManager
{

using ResolveFunctor = std::function<void()>;
using ExecuteFunctor = std::function<ResolveFunctor()>;
using ErrorHandlerFunctor = std::function<void(std::exception_ptr)>;

class Manager;

class Promise
{
private:
    struct State {
        ExecuteFunctor execute;
        ResolveFunctor resolve;
        ErrorHandlerFunctor reject;
        std::exception_ptr exception;
    };
    std::shared_ptr<State> m_state;
public:
    Promise(ExecuteFunctor executor, ErrorHandlerFunctor error);
    Promise() = default;
    Promise(const Promise &other);

    friend Manager;
};


class Manager: public QObject
{
    Q_OBJECT
public:
    Manager();
    ~Manager();
    void addToQueue(Promise p);

signals:
    void execute(Promise p);
    void ready(Promise p);

private:
    QThread m_thread;
};


using Executor = std::function<void(ExecuteFunctor, ErrorHandlerFunctor)>;

Executor makeThread();


};

Q_DECLARE_METATYPE(ThreadManager::Promise)


#endif // THREADMANAGER_H
