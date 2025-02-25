#include <thread>
#include <filesystem>
#include <loglib/loglib.h>

#include "inittaskherder.h"
#include "inittaskparser.h"

using namespace std;

#define DEFAULT_TASK_LOCATION "/etc/init_tasks"

std::string taskLocation = DEFAULT_TASK_LOCATION;

void parseArgs(int argc, char* argv[]){
    int i = 1;
    while (i < argc){
        if (strcmp(argv[i], "--location") == 0) {
            if (++i == argc){
                LOG_ERROR("--location argument is present, but actual location is missing");
                exit(1);
            }
            taskLocation = argv[i++];
            continue;
        }
    }
}

int getCPUCoreNumber(){
    return std::thread::hardware_concurrency();
}

std::vector<init::InitTask> parseTasksInFolder(std::string folder){
    std::vector<init::InitTask> tasks;
    for (const auto& directoryEntry: std::filesystem::directory_iterator(folder)){
        if (directoryEntry.is_regular_file()){
            LOG_DEBUG_F("Found task file: {}", directoryEntry.path().string());
            auto [success, task] = taskparser_parseTask(directoryEntry.path());
            if (success){
                tasks.push_back(task);
            } else {
                LOG_ERROR_F("Could not parse {}", directoryEntry.path().string());
            }
        }
    }
    return tasks;
}

int main(int argc, char* argv[])
{
    loglib::logger().setDefaultName("init");
    loglib::logger().registerLogger(logging::LOGGER_FILE);
    loglib::logger().registerLogger(logging::LOGGER_CONSOLE);

    parseArgs(argc, argv);
    int maxConcurrentJobs = getCPUCoreNumber();

    std::vector<init::InitTask> tasks = parseTasksInFolder(taskLocation);
    InitTaskHerder initTaskHerder {maxConcurrentJobs, tasks};
    return 0;
}
