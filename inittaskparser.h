#ifndef INITTASKPARSER_H
#define INITTASKPARSER_H
#include "inittask.h"
#include <fstream>
#include <iostream>

static std::string taskparser_extractInfo(const std::vector<std::string>& lines, const std::string& prefix){
    for (const std::string& s: lines){
        if (s.starts_with(prefix)){
            return s.substr(prefix.length());
        }
    }
    return "";
}

static std::vector<std::string> taskparser_readAllLines(const std::string& path){
    std::fstream taskFile{path};
    std::string line;
    std::vector<std::string> allLines;

    while (!taskFile.eof()){
        std::getline(taskFile, line);
        allLines.push_back(line);
    }
    return std::move(allLines);
}

static bool taskparser_validateTaskType(const std::string& taskType){
    return taskType ==  "FOREGROUND" || taskType == "BACKGROUND"
           || taskType == "ONESHOT" || taskType == "SHUTDOWN";
}

static init::TASK_TYPE taskparser_parseTaskType(const std::string& taskType){
    if (taskType == "FOREGROUND")
        return init::TASK_TYPE::FOREGROUND;
    if (taskType == "BACKGROUND")
        return init::TASK_TYPE::BACKGROUND;
    if (taskType == "ONESHOT")
        return init::TASK_TYPE::ONESHOT;
    return init::TASK_TYPE::SHUTDOWN;
}

static bool parseBool(const std::string& str){
    return str == "T" || str == "True";
}

// A function that splits a string into a vector of strings. The delimiter is comma.
static std::vector<std::string> taskparser_splitDependencies(std::string s){
    std::vector<std::string> ret;
    std::string token = ",";
    size_t pos = 0;
    while ((pos = s.find(token)) != std::string::npos) {
        ret.push_back(s.substr(0, pos));
        s.erase(0, pos + token.length());
    }
    if (!s.empty())
        ret.push_back(s);
    return ret;
}


static std::pair<bool, init::InitTask> taskparser_parseTask(std::string path){
    std::vector<std::string> allLines = taskparser_readAllLines(path);

    bool success = true;

    std::string name = taskparser_extractInfo(allLines, "Name: ");
    std::cout << "name: " << name << std::endl;
    std::string startCmd = taskparser_extractInfo(allLines, "Command: ");
    std::cout << "start cmd: " << startCmd << std::endl;
    std::string stopCmd = taskparser_extractInfo(allLines, "StopCommand: ");
    std::string runOnce = taskparser_extractInfo(allLines, "RunOnce: ");
    std::string enabled = taskparser_extractInfo(allLines, "Enabled: ");
    std::string type = taskparser_extractInfo(allLines, "Type: ");
    std::cout << "type: " << type << std::endl;
    std::string dependencies = taskparser_extractInfo(allLines, "Dependencies: ");

    if (name.empty()){
        std::cerr << "Missing name in " << path << std::endl;
        success = false;
    }
    init::InitTask it{name};

    if (!taskparser_validateTaskType(type)){
        std::cerr << "Invalid task: " << type << " in " << path << std::endl;
        success = false;
    } else {
        it.setTaskType(taskparser_parseTaskType(type));
    }

    it.setConfigPath(path);
    it.setStartCmd(startCmd);
    it.setStopCmd(stopCmd);
    it.setEnabled(parseBool(enabled));
    it.setDependencyNames(taskparser_splitDependencies(dependencies));

    std::pair<bool, init::InitTask> ret {success, it};

    return ret;
}

#endif // INITTASKPARSER_H
