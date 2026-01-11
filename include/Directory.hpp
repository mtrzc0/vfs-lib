#pragma once

#include "File.hpp"

// core
#include <vector>
#include <memory>

/**
 * @file Directory.hpp
 * @brief Header file for the Directory class, which acts as a container in the VFS.
 */

namespace mt {
    /**
     * @class Directory
     * @brief A specialized File that acts as a container for other File objects.
     * * Inherits from mt::File to provide a composite structure, allowing directories 
     * to be treated as files while managing a collection of entries.
     */
    class Directory : public File {
    private:
        std::vector<std::shared_ptr<File>> m_entries; ///< Internal storage for directory entries

    public:
        /**
         * @brief Construct a new Directory object.
         * @param path The filesystem path where the directory is located or should be created.
         */
        Directory(const char* path);
        
        /**
         * @brief Returns the number of files and sub-directories currently stored.
         * @return size_t The count of entries in the m_entries vector.
         */
        size_t entries_count() const;

        /**
         * @brief Adds a shared pointer of a File or Directory to this container.
         * @param entry The File object to be added.
         * @return Directory& Reference to this for operation chaining.
         */
        Directory& operator+=(std::shared_ptr<File> entry);

        /**
         * @brief Searches for an entry by its filename.
         * @param name The name string to match against entry names.
         * @return std::shared_ptr<File> The found File object, or nullptr if not found.
         */
        std::shared_ptr<File> operator[](const char* name) const;

    };
}