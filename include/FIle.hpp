#pragma once

// prevent overflow when reading larger files
#define _FILE_OFFSET_BITS 64

// cpp core
#include <cstdint>
#include <cerrno>

// kernel specific
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace mt {
    class File {
    private:
        int32_t m_fd;
        const char* m_path;     // absolute path
        int32_t m_oflag;
        int32_t m_mode;
        uint32_t m_size;
        int32_t m_errsv;
        stat *m_statbuf;        // metadata about file from kernel vfs
    public:
        // set full custom parameters
        File(int32_t fd, const char *path, int32_t oflag, int32_t mode): m_fd(fd), m_path(path), m_oflag(oflag), m_mode(mode)  {
            fd = open(path, oflag, mode);
            if (fd == -1) {
                errsv = errno;  // save error value
            }
            // set size
        }

        // read only
        File(int32_t fd, const char *path): m_fd(fd), m_path(path), m_oflag(O_RDONLY), m_mode(0) {
            fd = open(path, O_RDONLY, 0);
            if (fd == -1) {
                errsv = errno;
            }
            // set size
        }
    
        // setters
        int32_t set_oflag(int32_t fd, int32_t oflag, int32_t new_oflag) {
            fcntl(fd, FSETFL, oflag | new_oflag);
            if (fd == -1)
        }

        // getters
        const stat *get_stat() const {
            return m_statbuf;    
        }

        const char *get_name() const {
            return m_name;
        }

        const char *get_path() const {
            return m_path;
        }

        uint32_t get_size() const {
            return m_size;
        }

        int32_t get_errsv() const {
            return m_errsv;
        }
        
        ~File() {
            close(m_fd);
        }
    };   
}

