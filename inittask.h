#ifndef INITTASK_H
#define INITTASK_H

#include <string>
#include <vector>

namespace init {

enum TASK_STATUS {
    TODO = 0, RUNNING, DONE, FAIL
};

enum TASK_TYPE {
    FOREGROUND = 0, BACKGROUND, ONESHOT, SHUTDOWN
};

class InitTask
{
    std::string name_;
    TASK_STATUS status = TASK_STATUS::TODO;
    TASK_TYPE taskType;
    std::string configPath;
    std::string runCommand;
    std::string stopCommand;
    std::vector<std::string> dependencyNames;

    bool enabled;

public:
    InitTask(std::string name);
    InitTask(const InitTask& other);
    ~InitTask(){}

    bool done();

    void setConfigPath(std::string configPath){this->configPath = configPath;}
    void setStartCmd(std::string cmd){runCommand = cmd;}
    void setStopCmd(std::string cmd){stopCommand = cmd;}
    void setTaskType(TASK_TYPE tt){taskType = tt;}
    void setEnabled(bool enabled){this->enabled = enabled;}
    void setDependencyNames(std::vector<std::string> dependencyNames){this->dependencyNames = dependencyNames;}
    std::vector<std::string>& getDependencies();

    std::string getRunCommand(){return runCommand;}
    std::string getStopCommand(){return stopCommand;}

    TASK_TYPE getType(){return taskType;}
    TASK_STATUS getStatus(){return status;}
    void setStatus(TASK_STATUS taskStatus){status = taskStatus;}

    std::string getName() const;
    bool operator==(const InitTask& other);
};

}
#endif // INITTASK_H
