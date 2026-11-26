# Basic Process Injection – Windows API Research

## Overview

This project explores several Windows API concepts involved in process management, memory allocation, and thread execution.

The objective was to understand how Windows applications interact with running processes, how process information can be enumerated, and how Windows manages memory and threads.

The practical exercise used Notepad as the target process to examine these concepts in a controlled Windows 11 virtual machine.

## Process Enumeration

The first stage involved identifying a running process and retrieving its Process Identifier (PID).

Windows provides the Tool Help Library, which allows applications to take snapshots of currently running processes. These snapshots can then be enumerated to retrieve information about individual processes.

The main functions explored were:

- **CreateToolhelp32Snapshot:** Creates a snapshot of running processes.
- **Process32First:** Retrieves the first process entry from the snapshot.
- **Process32Next:** Iterates through the remaining process entries.

Each process has a unique PID, which Windows uses to identify it during process-management operations.

## Process Handles

Windows uses handles to provide access to system resources, including processes, threads, and files.

A process handle represents an open reference to a process. The access rights requested when obtaining a handle determine which operations can be performed against that process.

The exercise explored how Windows controls access to processes and how handles are used when interacting with them.

## Memory Management

Windows provides APIs that allow applications to reserve, commit, protect, and release virtual memory.

Each process operates within its own virtual address space. This provides isolation between processes and allows Windows to manage memory independently for each application.

The exercise introduced the concepts of:

- Virtual address spaces.
- Memory reservation and commitment.
- Memory protection permissions.
- Read, write, and execute permissions.
- Differences between local and remote process memory.

Memory protection is particularly important because it determines how a region of memory can be accessed or executed.

## Data Encoding and XOR

The project also explored basic XOR encryption and decryption.

XOR is a bitwise operation that compares two binary values. Applying the same key to the resulting data reverses the operation.

Although XOR can be used for simple data obfuscation, it is not considered secure encryption. Basic XOR transformations do not inherently prevent antivirus products from identifying suspicious data or behaviour.

An important distinction is that obfuscating data and preventing its detection are two different things.

## Thread Management

Windows applications use threads to execute instructions.

A thread is an independent execution path within a process. Windows provides APIs for creating, managing, waiting on, and retrieving information about threads.

The exercise introduced the following concepts:

- Thread creation and lifecycle.
- Thread handles.
- Waiting for thread completion.
- Thread exit codes.
- Synchronisation between execution paths.

Waiting for a thread to finish does not necessarily mean that its operation completed successfully. It only indicates that the thread has terminated. Its exit status must be considered separately.

## Error Handling and Resource Management

Windows API functions frequently return handles, Boolean values, or status codes to indicate whether an operation succeeded.

Checking these return values is essential when developing reliable Windows applications.

The project also highlighted the importance of resource management. Handles and dynamically allocated memory should be released appropriately to avoid resource leaks.

## Key Takeaways

This exercise provided practical exposure to several important Windows programming concepts:

- Enumerating running processes using the Tool Help API.
- Understanding PIDs and process handles.
- Learning how Windows manages virtual memory.
- Understanding memory protection permissions.
- Exploring basic XOR encoding and decoding.
- Understanding thread creation and synchronisation.
- Recognising the importance of API return values and error handling.
- Understanding the distinction between obfuscation and security.

## Environment

- **Operating System:** Windows 11 
- **Language:** C++
- **Platform:** Windows API
- **Testing Environment:** Isolated Windows virtual machine

## Conclusion

This project served as an introduction to Windows internals from a C++ development perspective.

Understanding process management, virtual memory, and thread execution provides a useful foundation for further research into Windows internals, application security, malware analysis, and defensive security engineering.
