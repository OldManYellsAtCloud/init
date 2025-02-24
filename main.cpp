#include <iostream>
#include <thread>
#include <chrono>
#include "initlibrary.h"

#include "inittaskcontainer.h"

#include <loglib/loglib.h>

using namespace std;

int main()
{
    loglib::logger().setDefaultName("notinit");
    loglib::logger().registerLogger(logging::LOGGER_FILE);
    loglib::logger().registerLogger(logging::LOGGER_CONSOLE);

    auto cb = [](std::string s, init::TASK_STATUS ts){
        std::cout << "Task finished: " << s << ", result: " << ts << std::endl;
    };

    auto it = init::InitTask{"aaa"};
    it.setStartCmd("echo hello1 && sleep 5 && echo hello2");
    InitTaskContainer itc {it};
    itc.setCb(cb);
    itc.run();

    std::this_thread::sleep_for(std::chrono::seconds(7));
    itc.stop();
    std::cout << "done" << std::endl;

    /*InitLibrary initLib{"/home/root/init_test"};

    std::vector<init::InitTask*> tasks;

    init::InitTask* task;
    for (int i = 0; i < 15; ++i){
        while ((task = initLib.getNewTask()) != nullptr){
            std::cout << "Got task: " << task->getName() << std::endl;
            tasks.push_back(task);
        }

        for (init::InitTask* task: tasks){
            std::cout << "Finishing task: " << task->getName() << std::endl;
            initLib.setTaskDone(task->getName());
        }
        tasks.clear();
    }*/

    return 0;
}
