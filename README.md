# VFS-lib (Virtual File System)

A lightweight, header-only (or split) **C++ Virtual File System** library designed for embedded systems and memory-constrained applications. This library provides an abstraction layer to manage a hierarchical structure of files and directories in memory.

## 🏗 Architecture & Design

The project follows a strict Object-Oriented approach with a focus on low overhead and clear responsibility separation.

### Class Hierarchy
The library utilizes inheritance (up to 3 levels) to represent different types of filesystem nodes:

1.  **`Node` (Base Class)**: An abstract interface representing any entry in the filesystem.
    * *Responsibilities*: Stores metadata (name, timestamps, parent reference).
    * *Key Methods*: `getName()`, `setName()`, and pure virtual `getSize()`.
2.  **`File<T> : public Node`**: A template class representing a file containing data of type `T`.
    * *Responsibilities*: Buffering data, managing read/write operations.
    * *Note*: Being a template allows storage of diverse data types (e.g., `uint8_t` for binaries, `float` for sensor logs).
3.  **`Directory : public Node`**: A container node that holds pointers to other `Node` objects.
    * *Responsibilities*: Management of children, searching, and path resolution.
4.  **`EncryptedFile<T> : public File<T>`**: (3rd level) A specialized file that encrypts data using a lightweight algorithm before storage.



### Key Technical Features
* **Namespace Protection**: All classes are encapsulated within the `namespace [XY]` (where XY are the author's initials) to prevent naming collisions.
* **Constexpr Optimization**: Constants such as maximum filename lengths or default permissions are evaluated at compile-time.
* **RAII & Memory Management**: Proper use of constructors/destructors to manage the lifecycle of nodes.
* **No Direct I/O**: Following library best practices, the classes do not use `std::cin` or `std::cout`. They return values or status flags, allowing the end-user to decide how to handle the output.

---

## 🛠 Operator Overloading

To make the library intuitive and idiomatic, the following operators are implemented:

| Operator | Target Class | Purpose |
| :--- | :--- | :--- |
| `[]` | `Directory` | Access a child node by its string name (e.g., `dir["config.txt"]`). |
| `+=` | `Directory` | Add a new node (file or subdirectory) to the directory. |
| `<<` | `File<T>` | Append data to the file's internal buffer. |
| `==` | `Node` | Compare two nodes based on their metadata and identity. |
| `<` | `Node` | Compare nodes for alphabetical sorting in directory listings. |
| `bool` | `File<T>` | Returns true if the file contains data (is not empty). |

---

## 🚦 Error Handling (Exceptions)

The library uses a custom exception hierarchy to handle runtime errors gracefully:
* `VFS::FileNotFoundException`: Thrown when a specified path does not exist.
* `VFS::FileAlreadyExistsException`: Thrown when trying to create a node with a duplicate name.
* `VFS::AccessDeniedException`: Thrown when unauthorized operations are attempted.

---

## 📂 Project Structure

```text
.
├── include/           # Header files (.hpp)
│   └── XY_VFS.hpp     # Library headers with Doxygen documentation
├── src/               # Implementation files (.cpp)
│   ├── XY_Node.cpp
│   ├── XY_File.cpp
│   └── XY_Directory.cpp
├── examples/
│   └── main.cpp       # Demonstration of features and usage
├── CMakeLists.txt     # Build configuration
└── README.md          # Project documentationb
