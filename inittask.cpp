#include "inittask.h"

init::InitTask::InitTask(std::string name): name_{name} {}

init::InitTask::InitTask(const InitTask &other): name_{other.getName()} {
    enabled = other.enabled;
    status = other.status;
    taskType = other.taskType;
    configPath = other.configPath;
    runCommand = other.runCommand;
    stopCommand = other.stopCommand;
    dependencyNames = std::vector<std::string>(other.dependencyNames);
}


bool init::InitTask::done()
{
    return status == DONE;
}

std::vector<std::string> &init::InitTask::getDependencies()
{
    return dependencyNames;
}

std::string init::InitTask::getName() const
{
    return name_;
}

bool init::InitTask::operator==(const InitTask &other)
{
    return other.getName() == name_ && other.runCommand == runCommand;
}
