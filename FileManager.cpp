#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream> 

namespace fs = std::filesystem;
using namespace std;

void list_directory(const fs::path& path) {
    cout << "Contents of " << path << ":\n";
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            cout << (entry.is_directory() ? "[DIR] " : "      ")
                << entry.path().filename().string() << "\n";
        }
    }
    catch (fs::filesystem_error& e) {
        cerr << "Error accessing directory: " << e.what() << "\n";
    }
}

bool change_directory(fs::path& current_path, const string& dir) {
    fs::path new_path;
    if (dir == "..") {
        new_path = current_path.parent_path();
    }
    else {
        new_path = current_path / dir;
    }

    if (fs::exists(new_path) && fs::is_directory(new_path)) {
        current_path = fs::canonical(new_path);
        return true;
    }
    else {
        cerr << "Directory does not exist.\n";
        return false;
    }
}

void view_file(const fs::path& current_path, const string& filename) {
    fs::path file_path = current_path / filename;
    if (fs::exists(file_path) && fs::is_regular_file(file_path)) {
        ifstream file(file_path);
        if (file.is_open()) {
            cout << "----- " << filename << " -----\n";
            string line;
            while (getline(file, line)) {
                cout << line << "\n";
            }
            cout << "----- End of " << filename << " -----\n";
            file.close();
        }
        else {
            cerr << "Unable to open file.\n";
        }
    }
    else {
        cerr << "File does not exist.\n";
    }
}

void create_directory(const fs::path& current_path, const string& dirname) {
    fs::path new_dir = current_path / dirname;
    try {
        if (fs::create_directory(new_dir)) {
            cout << "Directory created: " << new_dir << "\n";
        }
        else {
            cerr << "Failed to create directory. It may already exist.\n";
        }
    }
    catch (fs::filesystem_error& e) {
        cerr << "Error creating directory: " << e.what() << "\n";
    }
}

void copy_file(const fs::path& current_path, const string& source, const string& destination) {
    fs::path src = current_path / source;
    fs::path dest = current_path / destination;
    try {
        fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
        cout << "File copied from " << src << " to " << dest << "\n";
    }
    catch (fs::filesystem_error& e) {
        cerr << "Error copying file: " << e.what() << "\n";
    }
}

void move_file(const fs::path& current_path, const string& source, const string& destination) {
    fs::path src = current_path / source;
    fs::path dest = current_path / destination;
    try {
        fs::rename(src, dest);
        cout << "File moved from " << src << " to " << dest << "\n";
    }
    catch (fs::filesystem_error& e) {
        cerr << "Error moving file: " << e.what() << "\n";
    }
}

void display_help() {
    cout << "Available commands:\n";
    cout << "  ls                     - List directory contents\n";
    cout << "  cd <directory>         - Change directory\n";
    cout << "  view <filename>        - View file contents\n";
    cout << "  mkdir <directory>      - Create a new directory\n";
    cout << "  copy <source> <dest>   - Copy a file\n";
    cout << "  move <source> <dest>   - Move a file\n";
    cout << "  help                   - Show this help message\n";
    cout << "  exit                   - Exit the file manager\n";
}

int main() {
    fs::path current_path = fs::current_path();
    string input;

    cout << "Simple C++ File Manager\n";
    cout << "Type 'help' to see available commands.\n";

    while (true) {
        cout << current_path << " > ";
        getline(cin, input);

        istringstream iss(input);
        string command;
        iss >> command;

        transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (command == "ls") {
            list_directory(current_path);
        }
        else if (command == "cd") {
            string dir;
            iss >> dir;
            if (dir.empty()) {
                cerr << "Usage: cd <directory>\n";
            }
            else {
                change_directory(current_path, dir);
            }
        }
        else if (command == "view") {
            string filename;
            iss >> filename;
            if (filename.empty()) {
                cerr << "Usage: view <filename>\n";
            }
            else {
                view_file(current_path, filename);
            }
        }
        else if (command == "mkdir") {
            string dirname;
            iss >> dirname;
            if (dirname.empty()) {
                cerr << "Usage: mkdir <directory>\n";
            }
            else {
                create_directory(current_path, dirname);
            }
        }
        else if (command == "copy") {
            string source, destination;
            iss >> source >> destination;
            if (source.empty() || destination.empty()) {
                cerr << "Usage: copy <source> <destination>\n";
            }
            else {
                copy_file(current_path, source, destination);
            }
        }
        else if (command == "move") {
            string source, destination;
            iss >> source >> destination;
            if (source.empty() || destination.empty()) {
                cerr << "Usage: move <source> <destination>\n";
            }
            else {
                move_file(current_path, source, destination);
            }
        }
        else if (command == "help") {
            display_help();
        }
        else if (command == "exit") {
            cout << "Exiting File Manager.\n";
            break;
        }
        else if (command.empty()) {
            continue;
        }
        else {
            cerr << "Unknown command. Type 'help' to see available commands.\n";
        }
    }

    return 0;
}
