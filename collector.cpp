#include <iostream>
#include <string>
#include <filesystem>
#include <queue>
#include <sstream>
#include <fstream>
#include <iterator>

std::queue<std::string> *filenames(const std::string &path);
void read_and_write_to_file(const std::string &filename);
std::string filter_line(const std::string &line);

using std::filesystem::directory_iterator;

int main(int argc, char *argv[])
{
    if (argc == 1) {
        std::cerr << "Too few arguments;"
                  << "program requires path to folder to run" << std::endl;
        return 1;
    } else if (argc > 2) {
        std::cerr << "Too many arguments;"
                  << "program requires path to folder to run" << std::endl;
        return 1;
    }

    // truncate the file
    std::ofstream whole_text_txt_file;
    whole_text_txt_file.open("whole_text.txt", std::ios::trunc);
    whole_text_txt_file.close();

    std::string path = argv[1];

    auto *names = filenames(path);

    while (!names->empty()) {
        std::cout << std::endl << names->front() << std::endl;
        read_and_write_to_file(names->front());
        names->pop();
    }

    delete names;

    return 0;
}

std::queue<std::string> *filenames(const std::string &path)
{
    std::queue<std::string> *names = new std::queue<std::string>;

    std::stringstream stream;

    std::ofstream paths_write;
    paths_write.open("paths.txt", std::ios::out);

    try {
        for (const auto &file : directory_iterator(path))
            paths_write << file << std::endl;
    } catch (...) {
        std::cerr << "Terminated; path is incorrect" << std::endl;
    }

    paths_write.close();

    std::ifstream paths_read;
    paths_read.open("paths.txt", std::ios::in);

    if (!paths_read)
        return NULL;

    std::string line{};
    while (std::getline(paths_read, line))
        names->push(filter_line(line));

    return names;
}

void read_and_write_to_file(const std::string &filename)
{
    std::string txt {};

    for (size_t i {filename.length() - 1}; i > filename.length() - 4; i--)
        txt += filename[i];
    std::cout << std::endl << txt << std::endl;
    if (txt != "txt")
        return;

    std::ofstream file_write;
    file_write.open("whole_text.txt", std::ios::app);

    if (!file_write)
        return;

    std::ifstream file_read;
    file_read.open(filename, std::ios::in);

    if (!file_read)
        return;

    std::string content{std::istreambuf_iterator<char>{file_read}, {}};

    file_write << content << std::endl;

    file_read.close();
    file_write.close();
}

std::string filter_line(const std::string &line)
{
    std::string filtered{};

    for (size_t i{1}; i < line.length() - 1; i++) {
        filtered += line[i];
    }

    return filtered;
}