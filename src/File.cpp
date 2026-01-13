#include "File.hpp"

static constexpr int32_t DEFAULT_OFLAG = O_RDONLY;

namespace mt {
    // Set full custom parameters
    File::File(const char *path, int32_t oflag, int32_t mode)
    : m_path(path), m_oflag(oflag), m_mode(mode), m_name(""), m_fd(-1), m_size(0), m_errsv(0) {
        m_fd = open(path, oflag, mode);
        if (m_fd == -1) {
            m_errsv = errno;  // Save error value
        } else {
            update_stat();
        }
    }

    // Set read only
    File::File(const char *path)
        : m_path(path), m_oflag(DEFAULT_OFLAG), m_name(""), m_fd(-1), m_size(0), m_errsv(0) {
        m_fd = open(path, DEFAULT_OFLAG, S_IFREG);
        if (m_fd == -1) {
            m_errsv = errno;  // Save error value
        } else {
            update_stat();
        }        
    }

    File::~File() {
        close(m_fd);
    }
    
    void File::update_stat() {
        // Strrchr finds the pointer to the last '/'
        const char* last_slash = strrchr(m_path, '/');
        
        if (last_slash == nullptr) {
            m_name = m_path; // No slash found, path is the name
        } else {
            m_name = last_slash + 1; // Name starts after the last slash
        }

        // Update all parameters
        if (fstat(m_fd, &m_statbuf) == -1) {
            m_errsv = errno;
        } else {
            m_size = m_statbuf.st_size;
            m_atime = m_statbuf.st_atime;
            m_mtime = m_statbuf.st_mtime;
        }
    }
    
    // Setters
    bool File::set_oflag(int32_t new_oflag) {
        if (m_fd == -1) {
            return false;
        } 

        if (fcntl(m_fd, F_SETFL, new_oflag) == -1) {
            m_errsv = errno;
            return false;
        }

        m_oflag = new_oflag;
        return true;
    }

    // Getters
    const struct stat File::get_stat() const {
        return m_statbuf;    
    }

    const char *File::get_name() const {
        return m_name;
    }

    const char *File::get_path() const {
        return m_path;
    }

    off_t File::get_size() const {
        return m_size;
    }
    
    time_t File::get_atime() const {
        return m_atime;
    }
    
    char *File::get_atime_sec() const {
        return std::ctime(&m_atime);
    }
    
    time_t File::get_mtime() const {
        return m_mtime;
    }
    
    char *File::get_mtime_sec() const {
        return std::ctime(&m_mtime);
    }

    int32_t File::get_errsv() const {
        return m_errsv;
    }
    
    // Compare files by their unique Inode number and Device ID
    bool File::operator==(const File& other) const {
        if (m_fd == -1 || other.m_fd == -1) return false;
        return (m_statbuf.st_ino == other.m_statbuf.st_ino) && 
               (m_statbuf.st_dev == other.m_statbuf.st_dev);
    }

    // Write data to file using the stream operator
    File& File::operator<<(const char* data) {
        if (m_fd != -1 && data != nullptr) {
            ssize_t bytes_written = write(m_fd, data, strlen(data));
            if (bytes_written == -1) {
                m_errsv = errno;
            } else {
                update_stat(); // Refresh size and mtime after writing
            }
        }
        return *this;
    }
}

