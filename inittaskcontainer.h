#ifndef INITTASKCONTAINER_H
#define INITTASKCONTAINER_H
#include <future>
#include <thread>
#include <functional>
#include "inittask.h"

class InitTaskContainer
{
    init::InitTask task_;
    std::function<void(std::string, init::TASK_STATUS)> taskStatusCallback;
    std::thread tRun, tStop;
    std::future<init::TASK_STATUS> runResult;
    init::TASK_STATUS runCmd(std::string cmd);
public:
    InitTaskContainer(init::InitTask& task);
    InitTaskContainer(const InitTaskContainer& other);
    void run();
    void stop();
    void restart();
    void setCb(std::function<void(std::string, init::TASK_STATUS)> cb);
    init::TASK_STATUS getState();

    init::InitTask& getTask();
};

#endif // INITTASKCONTAINER_H
