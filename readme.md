This is a simple init system, to be ran as PID 1.

Task file format:
```
Name: string
Command: string
StopCommand: string
RunOnce: boolean (T/True/F/False)
Enabled: boolean
Type: enum (FOREGROUND, BACKGROUND, ONESHOT, SHUTDOWN)
Dependencies: comma separated string list
```

Name: name of the task. Should be unique amongst the tasks.

Command: the full command to start the task, including arguments.

StopCommand: the full command to stop the task.

RunOnce: currently ignored.

Enabled: run the task or not.

Type: FOREGROUND - the task doesn't fork, it stays in the foreground when running, indefinitely. BACKGROUND - the task is expected to fork. ONESHOT - The task is expected to finish on its own shortly after starting. SHUTDOWN - the task should be executed as part of the shutdown procedure.

Dependencies: names of other tasks that should be executed before this one.
