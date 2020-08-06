#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <numeric>

class AbstractFile
{
public:
    AbstractFile(std::string name) : m_name { std::move(name) } {}
    std::string GetName() const { return m_name; }
    virtual size_t GetSize() const = 0;

    virtual ~AbstractFile() = default;
private:
    std::string m_name;
};

class File : public AbstractFile
{
public:
    File(const File&) = default;
    File(File&&) = default;

    File& operator=(const File&) = default;
    File& operator=(File&&) = default;

    File(std::string name, size_t size, std::string type) : 
        AbstractFile(std::move(name)), m_size { size }, m_type { std::move(type) } {}
    
    size_t GetSize() const { return m_size; }
    std::string GetType() const { return m_type; }
private:
    size_t m_size;
    std::string m_type;
};

class Directory : public AbstractFile
{
public:
    Directory(const Directory&) = default;
    Directory(Directory&&) = default;

    Directory& operator=(const Directory&) = default;
    Directory& operator=(Directory&&) = default;

    Directory(std::string name) :
        AbstractFile(name), m_files() {}

    Directory& Add(std::shared_ptr<AbstractFile> file) 
    {
        m_files.push_back(file);
        return *this;
    }

    size_t GetSize() const override
    {
        return std::accumulate(m_files.cbegin(), m_files.cend(), static_cast<size_t>(0), 
        [](size_t a, std::shared_ptr<AbstractFile> b)
        {
            return a + b->GetSize();
        });
    }
private:
    std::vector<std::shared_ptr<AbstractFile>> m_files;
};

int main(int argc, char** argv)
{
    //beurk
    Directory dir {std::move(Directory("/")
        .Add(std::make_shared<File>("note.txt", 1025, "text"))
        .Add(std::make_shared<File>("a.exe", 5469, "executable"))
        .Add(std::make_shared<Directory>(std::move(Directory("C++")
            .Add(std::make_shared<File>("main.cpp", 825, "Source")))))
    )};

    std::cout << dir.GetSize() << " bytes" << std::endl;
    return EXIT_SUCCESS;
}