#pragma once

#include "File.hpp"
#include <vector>
#include <memory>

namespace mt {
    class Directory : public File {
    private:
        std::vector<std::shared_ptr<File>> m_entries;

    public:
        explicit Directory(const char* path);
        
        size_t entries_count() const;

        // Operator overloading
        Directory& operator+=(std::shared_ptr<File> entry);
        std::shared_ptr<File> operator[](const char* name) const;

    };
}