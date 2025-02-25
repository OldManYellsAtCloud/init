#include <stdio.h>
#include <loglib/loglib.h>
#include "inittaskcontainer.h"
#include "utils.h"

#include <iostream>

init::TASK_STATUS InitTaskContainer::runCmd(std::string cmd)
{
    loglib::logger().registerLogger(logging::LOGGER_FILE, task_.getName());
    LOG_INFO_NF(task_.getName(), "Running CMD: {}", cmd);

    // The problem with unique_ptr here is that it also calls the destructor
    // of the object when it goes out of scope, since that's the point of it.
    // However to get the result code of the execution, pclose need to be called,
    // which is the destructor of the FILE object - so there is a double free.
    // However since this naked pointer is created and destroyed only a
    // few lines from each other, I don't really mind it...
    FILE* pipe = popen(cmd.c_str(), "r");
    std::array<char, 256> outputBuffer;
    while (fgets(outputBuffer.data(), static_cast<int>(outputBuffer.size()), pipe) != nullptr){
        removeTrailingNewlines(outputBuffer.begin(), outputBuffer.end());
        LOG_INFO_N(task_.getName(), outputBuffer.data());
    }

    int ret = pclose(pipe);
    LOG_INFO_NF(task_.getName(), "CMD: {}, Result code: {}", cmd, ret);

    init::TASK_STATUS result = ret == 0 ? init::TASK_STATUS::DONE : init::TASK_STATUS::FAIL;
    task_.setStatus(result);
    taskStatusCallback(task_.getName(), result);

    return result;
}

InitTaskContainer::InitTaskContainer(init::InitTask &task): task_{task} {
    LOG_INFO_NF(task_.getName(), "Task Container for {} initialized", task_.getName());
}

InitTaskContainer::InitTaskContainer(const InitTaskContainer &other): task_{other.task_}
{
    taskStatusCallback = other.taskStatusCallback;
}

void InitTaskContainer::run()
{
    auto runTask = std::packaged_task<init::TASK_STATUS()>(std::bind(&InitTaskContainer::runCmd, this, task_.getRunCommand()));
    runResult = runTask.get_future();
    tRun = std::thread(std::move(runTask));
    task_.setStatus(init::TASK_STATUS::RUNNING);
    taskStatusCallback(task_.getName(), init::TASK_STATUS::RUNNING);
}

void InitTaskContainer::stop()
{
    if (task_.getStatus() == init::TASK_STATUS::RUNNING){
        tStop = std::thread{&InitTaskContainer::runCmd, this, task_.getStopCommand()};
        tStop.join();
        task_.setStatus(init::TASK_STATUS::DONE);
        taskStatusCallback(task_.getName(), init::TASK_STATUS::DONE);
    }

    tRun.join();
}

void InitTaskContainer::restart()
{
    if (task_.getStatus() == init::TASK_STATUS::RUNNING)
        stop();
    run();
}

void InitTaskContainer::setCb(std::function<void (std::string, init::TASK_STATUS)> cb)
{
    taskStatusCallback = cb;
}

init::TASK_STATUS InitTaskContainer::getState(){
    return runResult.get();
}

init::InitTask &InitTaskContainer::getTask()
{
    return task_;
}

InitTaskContainer &InitTaskContainer::operator=(InitTaskContainer other)
{
    if (this != &other){
        task_ = other.task_;
        taskStatusCallback = other.taskStatusCallback;
    }
    return *this;
}
