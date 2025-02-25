#include "inittaskherder.h"
#include <vector>
#include <loglib/loglib.h>

InitTaskHerder::InitTaskHerder(int maxConcurrentTaks, std::vector<init::InitTask> tasks):
    maxConcurrentTasks_{maxConcurrentTaks}, allTasks{tasks}, currentTaskNumber{0} {}

std::optional<init::InitTask> InitTaskHerder::getNextTask()
{
    for (init::InitTask& task: allTasks){
        if (task.getDependencies().empty()){
            init::InitTask ret{task};
            std::erase(allTasks, task);
            LOG_DEBUG_F("Returning new task to run: {}", ret.getName());
            return ret;
        }
    }
    return {};
}

bool InitTaskHerder::isTaskAvailable()
{
    for (init::InitTask& task: allTasks){
        if (task.getDependencies().empty())
            return true;
    }
    return false;
}

void InitTaskHerder::removeDependency(std::string dependencyName)
{
    LOG_DEBUG_F("Removing {} from dependency lists", dependencyName);
    for (init::InitTask& task: allTasks){
        std::erase_if(task.getDependencies(),
                      [&](std::string s){return s == dependencyName;});
    }
}

void InitTaskHerder::removeCurrentTask(std::string &name)
{
    LOG_DEBUG_F("Removing {} from current tasks", name);
    for (auto it = currentTasks.begin(); it != currentTasks.end(); ++it){
        if (it->getTask().getName() == name){
            currentTasks.erase(it);
            return;
        }
    }

    LOG_DEBUG_F("Remaining running tasks:");
    for (auto& ct: currentTasks){
        LOG_DEBUG(ct.getTask().getName());
    }
}

void InitTaskHerder::taskDoneNotification(std::string name, init::TASK_STATUS status)
{
    LOG_INFO_F("Task {} new status: {}", name, int(status));
    if (status == init::TASK_STATUS::FAIL){
        LOG_ERROR_F("Task {} failed! Let's remove it now, but TODO: handle me.", name);
    }

    if (status != init::TASK_STATUS::RUNNING){
        removeDependency(name);
        removeCurrentTask(name);
        runTasks();
    }
}

void InitTaskHerder::runTasks()
{
    while (isTaskAvailable() && currentTasks.size() < maxConcurrentTasks_){
        init::InitTask task = getNextTask().value();
        currentTasks.emplace_back(task);
        currentTasks.back().run();
    }
}

