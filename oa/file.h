#include<string>
class File
{
public:
    std::string name;

    std::string content;

    time_t creationDate;

    File(const std::string &n);

    std::string getContent() const;

    void setContent(const std::string &newContent);

    std::string getName() const;

    time_t getCreationDate() const;
};
