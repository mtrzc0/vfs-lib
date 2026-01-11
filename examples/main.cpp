#include "File.hpp"
#include "Directory.hpp"

// core
#include <iostream>
#include <memory>

// kernel
#include <fcntl.h>

int main() {
    using namespace mt;

    std::cout << "=== VFS-lib Hierarchy & Inheritance Demo ===\n" << std::endl;

    // Create a Root Directory
    auto root = std::make_shared<Directory>(".");

    // Create a Sub-directory
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
    auto found_entry = (*root)["workspace"]; // Returns shared_ptr<File>

    if (found_entry) {
        std::cout << "Found: " << found_entry->get_name() << " (Path: " << found_entry->get_path() << ")" << std::endl;

        // Attempt to treat the found File as a Directory
        // This requires the virtual destructor in File.hpp to work
        auto dir_ptr = std::dynamic_pointer_cast<Directory>(found_entry);

        if (dir_ptr) {
            std::cout << "Confirmed: 'workspace' is a Directory object." << std::endl;
            std::cout << "Entries inside workspace: " << dir_ptr->entries_count() << std::endl;

            // Access the nested file
            auto nested_file = (*dir_ptr)["README.txt"];
            if (nested_file) {
                std::cout << "Found Nested File: " << nested_file->get_name() 
                          << " | Size: " << nested_file->get_size() << " bytes" << std::endl;
            }
        }
    }

    // Equality Operator Demo
    File check("./workspace/README.txt"); // Re-open the same file
    if (*readme == check) {
        std::cout << "\nComparison: Inode check confirms objects point to the same physical file." << std::endl;
    }

    return 0;
}