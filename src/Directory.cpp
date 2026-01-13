#include "Directory.hpp"

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
    
    Directory& Directory::operator-=(std::shared_ptr<File> entry) {
        if (entry) {
            for (size_t i = 0; i < m_entries.size(); i++) {
                if (*m_entries[i] == *entry) {
                    if (unlink(entry->get_path()) == -1) {
                        m_errsv = errno;
                    }
                    m_entries.erase(m_entries.begin()+i);
                    return *this;
                }
            }
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

    void Directory::remove_dir(const char* path) {
        if (m_entries.size() != 0) {
            m_entries.clear();
        } else {
            if (rmdir(path) != 0) {
                m_errsv = errno;
            }
        }
    }
}