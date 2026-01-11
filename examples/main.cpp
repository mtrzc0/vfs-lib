#include <iostream>
#include <memory>
#include <fcntl.h>
#include "File.hpp"
#include "Directory.hpp"

int main() {
    using namespace mt;

    std::cout << "=== VFS-lib Hierarchy & Inheritance Demo ===\n" << std::endl;

    // Create a Root Directory
    // In POSIX, a directory is a file. We open the current working directory.
    auto root = std::make_shared<Directory>(".");

    // Create a Sub-directory
    // Because Directory inherits from File, this is perfectly valid.
    auto workspace = std::make_shared<Directory>("./workspace");
    
    // Create a regular File and write to it
    auto readme = std::make_shared<File>("./workspace/README.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    
    if (readme->get_errsv() == 0) {
        *readme << "Project: VFS-lib\n";
        *readme << "Status: Functional\n";
    }

    // Build the nested structure using operator+=
    *workspace += readme;    // Add File to Directory
    *root += workspace;      // Add Directory to Directory (allowed via inheritance)

    // Demonstrate Polymorphism and Retrieval
    std::cout << "Searching for 'workspace' in root..." << std::endl;
    auto foundEntry = (*root)["workspace"]; // Returns shared_ptr<File>

    if (foundEntry) {
        std::cout << "Found: " << foundEntry->get_name() << " (Path: " << foundEntry->get_path() << ")" << std::endl;

        // Attempt to treat the found File as a Directory
        // This requires the virtual destructor in File.hpp to work
        auto dirPtr = std::dynamic_pointer_cast<Directory>(foundEntry);

        if (dirPtr) {
            std::cout << "Confirmed: 'workspace' is a Directory object." << std::endl;
            std::cout << "Entries inside workspace: " << dirPtr->entries_count() << std::endl;

            // Access the nested file
            auto nestedFile = (*dirPtr)["README.txt"];
            if (nestedFile) {
                std::cout << "Found Nested File: " << nestedFile->get_name() 
                          << " | Size: " << nestedFile->get_size() << " bytes" << std::endl;
            }
        }
    }

    // Equality Operator Demo
    File check("./workspace/README.txt"); // Re-open the same file
    if (*readme == check) {
        std::cout << "\nComparison: Inode check confirms objects point to the same physical file." << std::endl;
    }

    std::cout << "\nDemo complete. Resources will be closed by smart pointer destructors." << std::endl;

    return 0;
}