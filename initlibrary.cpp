#include "initlibrary.h"
#include "inittaskparser.h"

#include <loglib/loglib.h>
#include <filesystem>

void InitLibrary::loadTasks()
{
    for (const auto& directoryEntry: std::filesystem::directory_iterator(path_)){
        if (directoryEntry.is_regular_file()){
            std::cout << "gonna parse: " << directoryEntry.path() << std::endl;
            auto [success, task] = taskparser_parseTask(directoryEntry.path());
            if (success){
                tasks.push_back(task);
            } else {
                loglib::logger().error("Could not read task from file: " + directoryEntry.path().string());
            }
        }
    }
}

init::InitTask* InitLibrary::getNewTask()
{
    std::cout << "get new task" << std::endl;
    for (init::InitTask& task: tasks){
        //if (task.readyToRun()){
            std::cout << "returning task" << std::endl;
            return &task;
        //}
    }
    std::cout << "no new task" << std::endl;
    return nullptr;
}

void InitLibrary::removeTask(const std::string &taskName)
{
    auto it = std::remove_if(tasks.begin(), tasks.end(), [taskName](const init::InitTask& task){
        return task.getName() == taskName;
    });
    tasks.erase(it, tasks.end());
}

void InitLibrary::updateDependencies(const std::string &doneTaskName)
{
    /*for (init::InitTask& task: tasks){
        task.completeDependency(doneTaskName);
    }*/
}

void InitLibrary::setTaskDone(const std::string &taskName)
{
    removeTask(taskName);
    updateDependencies(taskName);
}

InitLibrary::InitLibrary(const std::string& path): path_{path} {
    // maxTasks equals to the number of CPUs in the system
    maxTasks = std::thread::hardware_concurrency();
    loadTasks();
}
