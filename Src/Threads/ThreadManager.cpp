#include <Threads/ThreadManager.h>
#include <QThread>
#include <QDebug>

ThreadManager::Manager::Manager()
{
    qRegisterMetaType<Promise>();
    m_context = new QObject();
    m_context->moveToThread(&m_thread);
    connect(&m_thread, &QThread::finished, m_context, &QObject::deleteLater);
    connect(this, &Manager::execute, m_context, [this](Promise p) {
        try {
            p.m_state->resolve =  p.m_state->execute();
        } catch (...) {
            p.m_state->exception = std::current_exception();
        }
        emit ready(p);
    });

    connect(this, &Manager::ready, this, [](Promise p) {
        if (p.m_state->exception) {
            if (p.m_state->reject) {
                p.m_state->reject(p.m_state->exception);
            } else {
                std::rethrow_exception(p.m_state->exception);
            }
        } else if (p.m_state->resolve) {
            p.m_state->resolve();
        }
    });
    m_thread.start();


}

ThreadManager::Manager::~Manager()
{
    m_thread.quit();
    m_thread.wait();
}

void ThreadManager::Manager::addToQueue(ThreadManager::Promise p)
{
    emit execute(p);
}

ThreadManager::Promise::Promise(ThreadManager::ExecuteFunctor executor, ThreadManager::ErrorHandlerFunctor error):
    m_state(std::make_shared<State>())
{
    m_state->execute = executor;
    m_state->reject = error;
}

ThreadManager::Promise::Promise(const ThreadManager::Promise &other): m_state(other.m_state)
{

}

ThreadManager::Executor ThreadManager::makeThread()
{
    std::shared_ptr<Manager> manager = std::make_shared<Manager>();
    return [manager](ExecuteFunctor exec, ErrorHandlerFunctor reject) {
        Promise p(exec, reject);
        manager->addToQueue(p);
    };
}
