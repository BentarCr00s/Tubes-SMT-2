#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

// Struktur untuk merepresentasikan film
struct Film
{
    string title;
    Film *next;
};

// Struktur untuk merepresentasikan folder dan film yang sering dibuka
struct Folder
{
    string name;
    vector<string> fileList;
    unordered_map<string, int> freqMap;
    Film *filmList;
    Folder *next;
};

// Fungsi untuk mendapatkan nama folder dari path file
string getFolderName(string filePath)
{
    size_t pos = filePath.find_last_of("/\\");
    return (pos == string::npos) ? "" : filePath.substr(0, pos);
}

// Fungsi untuk menghitung frekuensi kemunculan nama file pada vector
unordered_map<string, int> countFileFrequency(vector<string> fileList)
{
    unordered_map<string, int> freqMap;
    for (string file : fileList)
    {
        freqMap[file]++;
    }
    return freqMap;
}

// Fungsi untuk membaca daftar file dari sebuah folder
vector<string> getFileList(string folderPath)
{
    vector<string> fileList;
    ifstream fileStream;
    string filePath;

    // Membuka folder dan membaca setiap file di dalamnya
    fileStream.open(folderPath.c_str());
    if (fileStream.is_open())
    {
        while (fileStream >> filePath)
        {
            fileList.push_back(filePath);
        }
        fileStream.close();
    }
    return fileList;
}

// Fungsi untuk menambahkan film ke dalam linked list
void addFilm(Film *&head, string title)
{
    Film *newFilm = new Film;
    newFilm->title = title;
    newFilm->next = nullptr;

    if (head == nullptr)
    {
        head = newFilm;
    }
    else
    {
        Film *current = head;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = newFilm;
    }
}

// Fungsi untuk menghapus linked list film
void deleteFilmList(Film *&head)
{
    while (head != nullptr)
    {
        Film *temp = head;
        head = head->next;
        delete temp;
    }
}

// Fungsi untuk menambahkan folder ke dalam linked list
void addFolder(Folder *&head, string folderPath)
{
    vector<string> fileList = getFileList(folderPath);
    string folderName = getFolderName(folderPath);
    unordered_map<string, int> freqMap = countFileFrequency(fileList);
    vector<pair<string, int>> freqVec(freqMap.begin(), freqMap.end());
    sort(freqVec.begin(), freqVec.end(), [](pair<string, int> a, pair<string, int> b)
         { return a.second > b.second; });

    Folder *newFolder = new Folder;
    newFolder->name = folderName;
    newFolder->fileList = fileList;
    newFolder->freqMap = freqMap;
    newFolder->filmList = nullptr;
    newFolder->next = nullptr;

    // Menambahkan film yang belum pernah ditonton ke dalam linked list
    ifstream filmStream;
    string filmTitle;
    filmStream.open("film-list.txt");
    if (filmStream.is_open())
    {
        while (filmStream >> filmTitle)
        {
            bool found = false;
            for (pair<string, int> freqPair : freqVec)
            {
                if (freqPair.first == filmTitle)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                addFilm(newFolder->filmList, filmTitle);
            }
        }
        filmStream.close();
    }

    if (head == nullptr)
    {
        head = newFolder;
    }
    else
    {
        Folder *current = head;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = newFolder;
    }
}

// Fungsi untuk menghapus linked list folder
void deleteFolderList(Folder *&head)
{
    while (head != nullptr)
    {
        deleteFilmList(head->filmList);
        Folder *temp = head;
        head = head->next;
        delete temp;
    }
}

// Fungsi untuk menampilkan linked list folder dan film
void printRecommendations(Folder *head)
{
    Folder *current = head;
    while (current != nullptr)
    {
        cout << "Folder: " << current->name << endl;
        cout << "Recommendations:" << endl;
        Film *currentFilm = current->filmList;
        while (currentFilm != nullptr)
        {
            cout << currentFilm->title << endl;
            currentFilm = currentFilm->next;
        }
        current = current->next;
    }
}

int main()
{
    Folder *folderList = nullptr;
    // Membaca daftar folder dari file dan menambahkan ke dalam linked list
    ifstream folderStream;
    string folderPath;
    folderStream.open("folder-list.txt");
    if (folderStream.is_open())
    {
        while (folderStream >> folderPath)
        {
            addFolder(folderList, folderPath);
        }
        folderStream.close();
    }

    // Menampilkan rekomendasi film
    printRecommendations(folderList);

    // Menghapus linked list folder
    deleteFolderList(folderList);

    return 0;
}