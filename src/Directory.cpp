#include "Directory.hpp"
#include <sys/stat.h> // Required for mkdir()
#include <algorithm>
#include <cstring>

namespace mt {

    Directory::Directory(const char* path) : File(path, O_RDONLY, 0) {
        // If the directory doesn't exist (File ctor set m_fd to -1)
        if (get_errsv() == ENOENT) {
            if (mkdir(path, 0755) == 0) {
                m_fd = open(path, O_RDONLY); 
                if (m_fd != -1) {
                    m_errsv = 0;        // Clear the previous error
                    update_stat();      // This will now correctly set m_name and m_statbuf
                }
            } else {
                m_errsv = errno;
            }
        }
    }

    Directory& Directory::operator+=(std::shared_ptr<File> entry) {
        if (entry) {
            m_entries.push_back(entry);
        }
        return *this;
    }

    std::shared_ptr<File> Directory::operator[](const char* name) const {
        auto it = std::find_if(m_entries.begin(), m_entries.end(), [name](const auto& f) {
            return std::strcmp(f->get_name(), name) == 0;
        });
        
        return (it != m_entries.end()) ? *it : nullptr;
    }

    size_t Directory::entries_count() const {
        return m_entries.size();
    }
}