# Technical Project Summary: VFS-lib

## 1. Project Overview
This project implements a C++17 Virtual File System (VFS) wrapper for POSIX systems. It provides a high-level abstraction for low-level system calls, ensuring safer memory management and a more intuitive API for file and directory manipulation.

## 2. Key Architecture Decisions

### The Composite Pattern
We implemented `Directory` as a class inherited from `File`. 
- **Reasoning**: In Unix-like systems, "Everything is a File." By inheriting, a `Directory` object can be treated as a `File` pointer, allowing us to build recursive tree structures.
- **Polymorphism**: The `File` class uses a `virtual` destructor to ensure that when a `Directory` is deleted via a `File` pointer, the `std::vector` of entries is properly cleaned up.



### Memory Management
- **Smart Pointers**: We used `std::shared_ptr<File>` for all directory entries. This prevents "Dangling File Descriptors" and ensures that a file is only closed when no more objects are referencing it.
- **RAII**: The constructor opens the file, and the destructor closes it, ensuring no resource leaks.

## 3. POSIX System Calls Used

| Function | Purpose in Project |
| :--- | :--- |
| `open()` | Used in constructors to initialize file descriptors. |
| `close()` | Used in the `File` destructor to release resources. |
| `fstat()` | Used in `update_stat()` to sync size and timestamps. |
| `mkdir()` | Used in `Directory` constructor to create physical folders. |
| `fcntl()` | Used in `set_oflag()` to modify file status flags at runtime. |
| `write()` | Used in `operator<<` to commit data to disk. |



## 4. Operator Overloading Logic

- **`operator==`**: Instead of comparing filenames (which can be misleading due to symlinks), we compare `st_dev` and `st_ino`. This confirms if two objects point to the same physical location on the disk.
- **`operator<<`**: Provides a "stream-like" interface for the POSIX `write` call.
- **`operator[]`**: Provides $O(n)$ lookup for files within a directory by name.

## 5. Large File Support
We defined `#define _FILE_OFFSET_BITS 64` to ensure that `off_t` and `stat` structures use 64-bit offsets, allowing the library to handle files larger than 2GB on 32-bit systems.



## 6. Documentation
The project is fully documented using **Doxygen**. 
- Run `doxygen Doxyfile` to generate the documentation.
- All public and protected members include `@brief` tags for clear API reference.