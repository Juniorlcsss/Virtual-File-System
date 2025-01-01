#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <fstream>

//interface
class IFile{

public:
    virtual ~IFile() {}
    virtual void open(const std::string &path)=0;
    virtual void read()=0;
    virtual void write(const std::string &data)= 0;
    virtual void close() =0;
};

//LocalFile class
class LocalFile : public IFile {

private:
    std::fstream fileStream;
    std::string filePath;

public:
    void open(const std::string &path) override{
        filePath = path;
        fileStream.open(filePath, std::ios::in | std::ios::out | std::ios::app);
        if (!fileStream.is_open()){
            std::cerr << "Failed to open file: " << filePath << std::endl;
        }
    }

    void read() override{
        std::string line;
        while(std::getline(fileStream, line)){
            std::cout << line << std::endl;
        }
    }

    void write(const std::string &data) override{
        if(fileStream.is_open()){
            fileStream << data << std::endl;
        }
    }

    void close() override{
        if(fileStream.is_open()){
            fileStream.close();
        }
    }
};

//VirtualFileSystem class
class VirtualFileSystem{
private:
    std::map<std::string, std::shared_ptr<IFile>> files;

public:
    void registerFile(const std::string &name, std::shared_ptr<IFile> file){
        files[name] = file;
    }

    std::shared_ptr<IFile> getFile(const std::string &name){
        return files[name];
    }
};

int main() {
    VirtualFileSystem vfs;

    //register
    auto localFile = std::make_shared<LocalFile>();
    std::string name;
    std::cout << "Type file name: " << std::endl;
    std::cin >> name;
    std::string fileName = name + ".txt";
    vfs.registerFile(fileName , localFile);

    //clear newline character

    //get user input
    auto file = vfs.getFile(fileName);
    std::string input;
    file->open(fileName); // Open the file once

    while (true) {
        std::getline(std::cin, input);
        if (input == "/exit") {
            break;
        }
        //write
        file->open(fileName);
        file->write(input);
        file->close();
        std::cout << "Enter text to write (or type '/exit' to quit): " << std::endl;
    }

    //read
    file->open(fileName);
    std::cout << "Your file reads:" << std::endl;
    file->read();
    file->close();

    return 0;
}