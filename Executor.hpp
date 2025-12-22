#pragma once

#include <iostream>
#include <array>
#include <sys/wait.h>
#include "ERROR.hpp"

using namespace std;

class Executor {
public:
    Executor() {}
    virtual ~Executor() {}

    static int execute(const string& command, string* outputs = nullptr, string* errors = nullptr, bool throws = true) {
        // Create pipes for stdout and stderr
        int stdout_pipe[2];
        int stderr_pipe[2];
        string errstr;

        if (pipe(stdout_pipe) == -1 || pipe(stderr_pipe) == -1) {
            errstr = "Failed to create pipe";
            if (errors) *errors = errstr;
            else cerr << errstr << endl;
            if (throws) throw ERROR("execute failed: " + command + "\n" + errstr);
            return -1;
        }

        pid_t pid = fork();
        if (pid == -1) {
            errstr = "Failed to fork process";
            if (errors) *errors = errstr;
            else cerr << errstr << endl;
            if (throws) throw ERROR("execute failed: " + command + "\n" + errstr);
            return -1;
        }

        if (pid == 0) { // Child process
            // Redirect stdout and stderr to pipes
            dup2(stdout_pipe[1], STDOUT_FILENO);
            dup2(stderr_pipe[1], STDERR_FILENO);
            
            // Close unused pipe ends
            close(stdout_pipe[0]);
            close(stdout_pipe[1]);
            close(stderr_pipe[0]);
            close(stderr_pipe[1]);

            // Execute the command
            execl("/bin/sh", "sh", "-c", command.c_str(), (char*)nullptr);
            
            // If execl fails
            if (throws)
                throw ERROR("execl failed");
            _exit(EXIT_FAILURE);
        } else { // Parent process
            // Close unused pipe ends
            close(stdout_pipe[1]);
            close(stderr_pipe[1]);

            // Read output and error from pipes
            array<char, 256> buffer;
            ssize_t count;
            
            if (outputs) outputs->clear();
            if (errors) errors->clear();
            
            // Read stdout
            while ((count = read(stdout_pipe[0], buffer.data(), buffer.size())) > 0) {
                if (outputs) {
                    outputs->append(buffer.data(), count);
                } else {
                    cout.write(buffer.data(), count);
                }
            }
            
            // Read stderr
            while ((count = read(stderr_pipe[0], buffer.data(), buffer.size())) > 0) {
                if (errors) {
                    errors->append(buffer.data(), count);
                } else {
                    cerr.write(buffer.data(), count);
                }
            }

            // Close pipe ends
            close(stdout_pipe[0]);
            close(stderr_pipe[0]);

            // Wait for child process to finish
            int status;
            waitpid(pid, &status, 0);
            
            int ret = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
            if (throws && ret != 0)
                throw ERROR("Execute failed (" + to_string(ret) + "): " + command + "\n" + (errors ? *errors : "<cerr>"));
            return ret;
        }
    } 
};
