#include <QtTest/QTest>
#include <QObject>
#include <Threads/ThreadManager.h>
#include <QDebug>

class ThreadTests: public QObject
{
    Q_OBJECT

private slots:
    void test_thread()
    {
        auto thread = ThreadManager::makeThread();

        auto mainThreadId = QThread::currentThreadId();
        void *execThreadId = nullptr;
        void *resolveThreadId = nullptr;
        bool ready = false;

        auto exec = [&ready, &execThreadId, &resolveThreadId] () {
            execThreadId = QThread::currentThreadId();
            return [&ready, &resolveThreadId] () {
                resolveThreadId = QThread::currentThreadId();
                ready = true;
            };
        };

        auto errorHandler = [&ready] (std::exception_ptr ptr) {
            std::rethrow_exception(ptr);
        };

        thread(exec, errorHandler);

        QTest::qWaitFor([&ready]()->bool { return ready; });

        Q_ASSERT(ready);
        QCOMPARE(mainThreadId, resolveThreadId);
        Q_ASSERT(mainThreadId != execThreadId);
    }

    void test_exception()
    {
        auto thread = ThreadManager::makeThread();

        auto mainThreadId = QThread::currentThreadId();
        void *execThreadId = nullptr;
        void *exceptionThreadId = nullptr;
        bool ready = false;

        auto exec = [&ready, &execThreadId] () {
            execThreadId = QThread::currentThreadId();

            std::vector<int> t;
            t.at(1); // throw out of range
            return [&ready] () {
                ready = true;
            };
        };

        auto errorHandler = [&ready, &exceptionThreadId] (std::exception_ptr ptr) {
            try {
                std::rethrow_exception(ptr);
            } catch (std::out_of_range ex) {
                exceptionThreadId = QThread::currentThreadId();
                ready = true;
            }
        };

        thread(exec, errorHandler);

        QTest::qWaitFor([&ready]()->bool { return ready; });
        Q_ASSERT(ready);
        QCOMPARE(mainThreadId, exceptionThreadId);
        Q_ASSERT(mainThreadId != execThreadId);
    }
};

#include "ThreadTests.moc"
QTEST_MAIN(ThreadTests)
