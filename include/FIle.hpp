#pragma once

/**
 * @file File.hpp
 * @brief Base class providing a POSIX wrapper for file operations in the VFS.
 */

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
    /**
     * @class File
     * @brief Encapsulates a POSIX file descriptor and its associated metadata.
     * * This class handles the lifecycle of a file (open/close) and provides
     * access to kernel-level statistics and status flags.
     */
    class File {
    protected:
        int32_t m_fd;                   ///< Low-level file descriptor.
        int32_t m_errsv;                ///< Last recorded errno value.
        
        /** @brief Synchronizes internal attributes with the physical file status. */
        void update_stat();
        
    private:
        const char* m_path;             ///< Absolute or relative path to the file.
        const char* m_name;             ///< Filename extracted from the path.
        int32_t m_oflag;                ///< Flags used to open the file.
        struct stat m_statbuf;          ///< Raw kernel metadata structure.
        mode_t m_mode;                  ///< Permissions mode (if created).
        off_t m_size;                   ///< Size of file in bytes.
        time_t m_atime;                 ///< Last access timestamp.
        time_t m_mtime;                 ///< Last modification timestamp.

    public:
        /** @brief Constructor for full control over open flags and permissions. */
        File(const char *path, int32_t oflag, int32_t mode);

        /** @brief Convenience constructor for opening existing files in read-only mode. */
        explicit File(const char *path);

        /** @brief Virtual destructor ensuring the file descriptor is closed. */
        virtual ~File();
    
        /** @brief Changes the file status flags using fcntl. */
        bool set_oflag(int32_t new_oflag);

        /** @brief Returns a copy of the raw POSIX stat structure. */
        const struct stat get_stat() const;

        /** @brief Returns the extracted name of the file. */
        const char *get_name() const;

        /** @brief Returns the full path string. */
        const char *get_path() const;

        /** @brief Returns the file size as off_t. */
        off_t get_size() const;

        /** @brief Returns the raw access timestamp. */
        time_t get_atime() const;

        /** @brief Returns the access time as a human-readable C-string. */
        char *get_atime_sec() const;
        
        /** @brief Returns the raw modification timestamp. */
        time_t get_mtime() const;
        
        /** @brief Returns the modification time as a human-readable C-string. */
        char* get_mtime_sec() const;

        /** @brief Returns the saved errno from the last failed operation. */
        int32_t get_errsv() const;
        
        /** @brief Compares two files based on their unique Device and Inode IDs. */
        bool operator==(const File& other) const;

        /** @brief Writes the provided string data directly to the file. */
        File& operator<<(const char* data);
    };   
}