#include "file.h"
#include <vector>
class Directory
{
public:
    std::string name;

    std::vector<Directory *> directories;

    std::vector<File *> files;

    time_t creationDate;

    Directory(const std::string &n);

    void addDirectory(Directory *subdirectory);

    void removeDirectory(Directory *subdirectory);

    std::vector<Directory *> getDirectories() const;

    void addFile(File *file);

    void removeFile(File *file);

    std::vector<File *> getFiles() const;

    std::string getName() const;

    time_t getCreationDate() const;

    Directory *getParent() const;

    void moveUp();

    void rename(const std::string &newName);
};
