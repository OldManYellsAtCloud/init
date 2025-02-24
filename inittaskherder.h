#ifndef INITTASKHERDER_H
#define INITTASKHERDER_H

#include <string>
#include <optional>
#include "inittaskcontainer.h"

class InitTaskHerder
{
    int maxConcurrentTasks_;
    int currentTaskNumber;
    std::optional<init::InitTask> getNextTask();
    bool isTaskAvailable();
    std::vector<init::InitTask> allTasks;
    std::vector<InitTaskContainer> currentTasks;
    void removeDependency(std::string dependencyName);
public:
    void taskDoneNotification(std::string name, init::TASK_STATUS status);
    InitTaskHerder(int maxConcurrentTasks, std::vector<init::InitTask> tasks);
    void runTasks();
};

#endif // INITTASKHERDER_H
