#include "Directory.h"
class FileSystem
{

public:
    Directory *root;

    Directory *currentDirectory;

    FileSystem();

    void addDirectory(Directory *parent, const std::string &directoryName);

    void addFile(Directory *parent, const std::string &fileName);

    void deleteDirectory(Directory *directory);

    void deleteFile(File *file);

    void changeDirectory(Directory *newDirectory);

    void listContents(Directory *directory);

    void moveUp();

    void renameDirectory(Directory *directory, const std::string &newName);

    void renameFile(File *file, const std::string &newName);

    void editFileContent(File *file, const std::string &newContent);

    void displayProperties(Directory *directory);

    void displayProperties(File *file);
};
