#pragma once

// prevent overflow when reading larger files
#define _FILE_OFFSET_BITS 64

// core
#include <cstdint>
#include <cerrno>
#include <cstring>
#include <ctime>

// kernel 
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace mt {
    class File {
    protected:
        int32_t m_fd;                   // file descriptor
        int32_t m_errsv;
        void update_stat();
        
    private:
        const char* m_path;             // absolute path
        const char* m_name;
        int32_t m_oflag;
        struct stat m_statbuf;          // metadata about file from kernel vfs
        mode_t m_mode;
        off_t m_size;
        time_t m_atime;
        time_t m_mtime;

    public:
        // full custom parameters constructor
        File(const char *path, int32_t oflag, int32_t mode);

        // read only constructor
        File(const char *path);

        // release fd destructor
        virtual ~File();
    
        // setters
        bool set_oflag(int32_t new_oflag);

        // getters
        const struct stat get_stat() const;

        const char *get_name() const;

        const char *get_path() const;

        off_t get_size() const;

        time_t get_atime() const;

        char *get_atime_sec() const;
        
        time_t get_mtime() const;
        
        char* get_mtime_sec() const;

        int32_t get_errsv() const;
        
        // operator overloading
        bool operator==(const File& other) const;

        File& operator<<(const char* data);
    };   
}