#include "File.hpp"
#include "Directory.hpp"

// core
#include <iostream>
#include <memory>

int main() {
    using namespace mt;

    std::cout << "=== VFS-lib Hierarchy & Inheritance Demo ===\n" << std::endl;

    // 1. Create a Root Directory tracking the current folder
    auto root = std::make_shared<Directory>(".");

    // 2. Create a Sub-directory (mkdir called internally)
    auto workspace = std::make_shared<Directory>("./workspace");
    
    // 3. Create a regular File and write to it
    auto readme = std::make_shared<File>("./workspace/README.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    
    if (readme->get_errsv() == 0) {
        *readme << "Project: VFS-lib\n";
        *readme << "Status: Functional\n";
    }

    // 4. Build the nested structure using operator+=
    *workspace += readme;    
    *root += workspace;      

    // 5. Demonstrate Polymorphism and Retrieval
    std::cout << "Searching for 'workspace' in root..." << std::endl;
    auto found_entry = (*root)["workspace"]; 

    if (found_entry) {
        std::cout << "Found: " << found_entry->get_name() << " (Path: " << found_entry->get_path() << ")" << std::endl;

        // Cast to Directory to access directory-specific methods
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

            // 6. Demonstrate Removal (operator-=)
            std::cout << "\nRemoving 'README.txt' from tracking..." << std::endl;
            *dir_ptr -= readme; 
            std::cout << "Entries remaining in workspace: " << dir_ptr->entries_count() << std::endl;
        }
    }

    // 7. Equality Operator Demo
    File check("./workspace/README.txt"); 
    if (*readme == check) {
        std::cout << "Comparison: Inode check confirms objects point to the same physical file." << std::endl;
    }

    // 8. Physical Directory Removal Demo
    std::cout << "\nCleaning up: Removing './workspace' from filesystem..." << std::endl;
    // Removes all files in directory
    workspace->remove_dir("./workspace");

    if (workspace->get_errsv() != 0) {
        std::cerr << "Note: remove_dir failed (likely dir not empty or permission denied)." << std::endl;
    } else {
        std::cout << "Success: Directory removed." << std::endl;
    }

    return 0;
}