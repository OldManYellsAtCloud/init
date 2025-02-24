#ifndef INITLIBRARY_H
#define INITLIBRARY_H

#include "inittask.h"
#include <thread>

class InitLibrary
{
private:
    int maxTasks;
    int curTasks = 0;
    std::string path_;
    std::vector<init::InitTask> tasks;
    std::vector<std::thread> threads;
    void loadTasks();

    void removeTask(const std::string& taskName);
    void updateDependencies(const std::string& doneTaskName);

public:
    void setTaskDone(const std::string& taskName);
    init::InitTask* getNewTask();
    InitLibrary(const std::string& path);
    ~InitLibrary(){}
};
#endif // INITLIBRARY_H
