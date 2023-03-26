#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <dirent.h>
#include <unordered_map>

using namespace std;

struct Node {
    string title;
    string genre;
    Node* next;
};

class LinkedList {
public:
    LinkedList() {
        head = nullptr;
        tail = nullptr;
    }
    
    void insert(string title, string genre) {
        Node* newNode = new Node;
        newNode->title = title;
        newNode->genre = genre;
        newNode->next = nullptr;
        if(head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    
    bool search(string title) {
        Node* current = head;
        while(current != nullptr) {
            if(current->title == title) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    void display() {
        Node* current = head;
        while(current != nullptr) {
            cout << current->title << " (" << current->genre << ")" << endl;
            current = current->next;
        }
    }
    
private:
    Node* head;
    Node* tail;
};

unordered_map<string, LinkedList> genreList;

void searchDirectory(string path) {
    DIR* dir;
    dirent* pdir;
    dir = opendir(path.c_str());
    while(pdir = readdir(dir)) {
        string filename = pdir->d_name;
        if(filename != "." && filename != "..") {
            string fullpath = path + "/" + filename;
            if(pdir->d_type == DT_DIR) {
                // If the current item is a directory, recursively search it
                searchDirectory(fullpath);
            }
            else {
                // If the current item is a file, check if it's a video file
                string extension = filename.substr(filename.find_last_of(".") + 1);
                if(extension == "mp4" || extension == "avi" || extension == "mkv") {
                    // Get the genre from the directory name
                    string genre = fullpath.substr(0, fullpath.find_last_of("/"));
                    genre = genre.substr(genre.find_last_of("/") + 1);
                    // Add the video file to the corresponding genre's linked list
                    genreList[genre].insert(filename, genre);
                }
            }
        }
    }
    closedir(dir);
}

int main() {
    // Search the "films" directory and its subdirectories for video files
    searchDirectory("films");
    
    // Display the linked lists for each genre
    for(auto const& genre : genreList) {
        cout << "======== " << genre.first << " ========" << endl;
        genre.second.display();
    }
    
    // Count how many times each genre's videos have been opened
    unordered_map<string, int> genreCount;
    DIR* dir;
    dirent* pdir;
    dir = opendir("films");
    while(pdir = readdir(dir)) {
        string filename = pdir->d_name;
        if(filename != "." && filename != "..") {
            string fullpath = "films/" + filename;
            if(pdir->d_type != DT_DIR && !genreList[filename].search(filename)) {
                // If the current item is a video file that hasn't been opened before, count it
                ifstream file(fullpath);
                if(file) {
                    genreCount[genreList[filename].head->genre]++;
               
}
closedir(dr);}

// fungsi untuk menampilkan rekomendasi film
void display_recommendations() {
// membuat list kosong untuk rekomendasi film
List recommendation_list;
recommendation_list.head = NULL;
recommendation_list.tail = NULL;
// mengisi list rekomendasi film dengan film-film dari genre yang sering dibuka
for (auto const& genre : frequent_genres) {
    string path = "film/" + genre + "/";
    read_directory(path, recommendation_list);
}

// menampilkan 10 film rekomendasi yang tidak pernah ditonton
int count = 0;
Node* current = recommendation_list.head;
cout << "Berikut adalah rekomendasi film untuk Anda:\n";
while (count < 10 && current != NULL) {
    if (find(watched_movies.begin(), watched_movies.end(), current->data) == watched_movies.end()) {
        cout << "- " << current->data << endl;
        count++;
    }
    current = current->next;
}
}

int main() {
// membaca file film yang sudah ditonton
read_watched_movies();
// menghitung frekuensi membuka folder genre
count_genre_frequency();

// menampilkan 10 rekomendasi film
display_recommendations();

return 0;
}