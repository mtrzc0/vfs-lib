# VFS-lib: Virtual File System Library

A lightweight, POSIX-compliant C++17 wrapper for file and directory management.

### Features

- **Object-Oriented POSIX**: Wraps low-level system calls (`open`, `fstat`, `mkdir`, `fcntl`) into clean C++ classes.
- **Hierarchical Storage**: `Directory` inherits from `File`, allowing nested directory trees.
- **Smart Metadata**: Automatic extraction of file size, access/modification times, and filenames.
- **Operator Overloading**: 
  - `file1 == file2`: Compare physical files via Inode/Device ID.
  - `file << "data"`: Stream-style writing to files.
  - `dir += file`: Add entries to a directory.
  - `dir["name"]`: Retrieve entries via array-style lookup.
- **Memory Safety**: Uses `std::shared_ptr` to manage file lifecycles and ensure descriptors are closed correctly.

### Project Structure
```text
.
├── include/
│   ├── File.hpp
│   └── Directory.hpp
├── src/
│   ├── File.cpp
│   └── Directory.cpp
├── examples/
│   └── main.cpp
├── Makefile
├── Doxyfile
└── README.md
```


## 🛠 Installation & Build

### Prerequisites
- GCC/Clang with C++17 support.
- Doxygen (optional, for documentation).
- Graphviz (optional, for architecture diagrams).

### Compilation
Use the provided `Makefile`:
```bash
make        # Compiles the library and demo
./build/main # Runs the demonstration
```

### Doxygen
To generate the full technical API documentation:

Ensure Doxyfile is in the root directory.

Run `doxygen Doxyfile`.

Open `doc/html/index.html` in your browser.

### Example Usage
```cpp
using namespace mt;

// Create a directory and a file
auto workDir = std::make_shared<Directory>("./work");
auto logFile = std::make_shared<File>("./work/log.txt", O_RDWR | O_CREAT, 0644);

// Write data and organize
*logFile << "System initialized\n";
*workDir += logFile;

// Retrieve by name
auto found = (*workDir)["log.txt"];
```
