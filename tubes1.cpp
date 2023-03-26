#include <iostream>
#include <string>

using namespace std;

// Deklarasi Struktur Data Film
struct Film
{
    string judul;
    string genre;
    int tahun;
    Film *next;
};

// Fungsi untuk menambahkan film ke dalam linked list
void tambahFilm(Film *&head, string judul, string genre, int tahun)
{
    Film *baru = new Film;
    baru->judul = judul;
    baru->genre = genre;
    baru->tahun = tahun;
    baru->next = NULL;

    if (head == NULL)
    {
        head = baru;
    }
    else
    {
        Film *tail = head;
        while (tail->next != NULL)
        {
            tail = tail->next;
        }
        tail->next = baru;
    }
}

// Fungsi untuk menampilkan semua film dalam linked list
void tampilkanSemuaFilm(Film *head)
{
    if (head == NULL)
    {
        cout << "Tidak ada film dalam daftar." << endl;
    }
    else
    {
        cout << "Daftar Film:" << endl;
        Film *curr = head;
        while (curr != NULL)
        {
            cout << "- " << curr->judul << " (" << curr->tahun << ") - " << curr->genre << endl;
            curr = curr->next;
        }
    }
}

// Fungsi untuk merekomendasikan film berdasarkan genre
void rekomendasiFilm(Film *head, string genre)
{
    bool found = false;
    if (head == NULL)
    {
        cout << "Tidak ada film dalam daftar." << endl;
    }
    else
    {
        cout << "Rekomendasi Film Berdasarkan Genre \"" << genre << "\":" << endl;
        Film *curr = head;
        while (curr != NULL)
        {
            if (curr->genre == genre)
            {
                cout << "- " << curr->judul << " (" << curr->tahun << ")" << endl;
                found = true;
            }
            curr = curr->next;
        }
        if (!found)
        {
            cout << "Tidak ada film yang cocok dengan genre tersebut." << endl;
        }
    }
}

int main()
{
    Film *head = NULL;

    // Menambahkan film-film ke dalam linked list
    tambahFilm(head, "Avengers: Endgame", "Action", 2019);
    tambahFilm(head, "The Dark Knight", "Action", 2008);
    tambahFilm(head, "Inception", "Action", 2010);
    tambahFilm(head, "The Shawshank Redemption", "Drama", 1994);
    tambahFilm(head, "Forrest Gump", "Drama", 1994);
    tambahFilm(head, "The Godfather", "Crime", 1972);
    tambahFilm(head, "The Godfather: Part II", "Crime", 1974);
    tambahFilm(head, "The Silence of the Lambs", "Thriller", 1991);
    tambahFilm(head, "Psycho", "Horror", 1960);

    // Menampilkan semua film dalam linked list
    tampilkanSemuaFilm(head);

    // Merekomendasikan film-film berdasarkan genre
    rekomendasiFilm(head, "Action");
    rekomendasiFilm(head, "Drama");
    rekomendasiFilm(head, "Comedy");

    return 0;
}
