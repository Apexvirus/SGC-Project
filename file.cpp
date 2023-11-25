#include "file.h"

    

File::File(const std::string &n){
    
}
std::string File::getContent() const{
    return content;
}
void File::setContent(const std::string &newContent){

}
std::string File::getName() const{
    return name;
}
time_t  File::getCreationDate() const{
    return creationDate;
}