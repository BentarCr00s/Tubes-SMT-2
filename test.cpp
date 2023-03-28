#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>

using namespace std;

struct Movie
{
    int id;
    string title;
    string genre;
};

struct Rating
{
    int userId;
    int movieId;
    int rating;
};

// Fungsi untuk menghitung similarity antar user
double similarity(unordered_map<int, double> &user1, unordered_map<int, double> &user2)
{
    double dotProduct = 0.0;
    double norm1 = 0.0;
    double norm2 = 0.0;

    for (auto &p1 : user1)
    {
        int movieId = p1.first;
        double rating1 = p1.second;
        if (user2.count(movieId))
        {
            double rating2 = user2[movieId];
            dotProduct += rating1 * rating2;
        }
        norm1 += rating1 * rating1;
    }

    for (auto &p2 : user2)
    {
        double rating2 = p2.second;
        norm2 += rating2 * rating2;
    }

    if (norm1 == 0 || norm2 == 0)
    {
        return 0.0;
    }
    else
    {
        return dotProduct / (sqrt(norm1) * sqrt(norm2));
    }
}

// Fungsi untuk merekomendasikan film berdasarkan rating dari user-user yang memiliki similarity yang tinggi
vector<pair<int, double>> recommendMovies(int userId, unordered_map<int, unordered_map<int, double>> &userRatings, vector<Rating> &allRatings, int numRecs)
{
    unordered_map<int, double> user1 = userRatings[userId];

    // Menghitung similarity antar user
    vector<pair<int, double>> similarities;
    for (auto &p2 : userRatings)
    {
        int userId2 = p2.first;
        if (userId2 != userId)
        {
            double sim = similarity(user1, p2.second);
            similarities.push_back({userId2, sim});
        }
    }

    // Mengurutkan user berdasarkan similarity
    sort(similarities.begin(), similarities.end(), [](const pair<int, double> &p1, const pair<int, double> &p2)
         { return p1.second > p2.second; });

    // Merekomendasikan film
    unordered_map<int, double> movieScores;
    for (auto &p : similarities)
    {
        int userId2 = p.first;
        double sim = p.second;
        unordered_map<int, double> user2 = userRatings[userId2];
        for (auto &p2 : user2)
        {
            int movieId = p2.first;
            double rating = p2.second;
            if (!user1.count(movieId))
            {
                if (!movieScores.count(movieId))
                {
                    movieScores[movieId] = 0.0;
                }
                movieScores[movieId] += rating * sim;
            }
        }
    }

    // Mengurutkan film berdasarkan skor
    vector<pair<int, double>> recommendations;
    for (auto &p : movieScores)
    {
        int movieId = p.first;
        double score = p.second;
        recommendations.push_back({movieId, score});
    }
    sort(recommendations.begin(), recommendations.end(), [](const pair<int, double> &p1, const pair<int, double> &p2)
         { return p1.second > p2.second; });
    vector<pair<int, double>> topRecs(recommendations.begin(), recommendations.begin() + numRecs);

    return topRecs;
}

int main()
{
    // Membaca file movie
    ifstream movieFile("movies.csv");
    vector<Movie> movies;
    string line;
    getline(movieFile, line); // skip header
    while (getline(movieFile, line))
    {
        stringstream ss(line);
        string id, title, genre;
        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, genre, ',');
        movies.push_back({stoi(id), title, genre});
    }
    movieFile.close(); // Membaca file rating
    ifstream ratingFile("ratings.csv");
    vector<Rating> ratings;
    getline(ratingFile, line); // skip header
    while (getline(ratingFile, line))
    {
        stringstream ss(line);
        string userId, movieId, rating;
        getline(ss, userId, ',');
        getline(ss, movieId, ',');
        getline(ss, rating, ',');
        ratings.push_back({stoi(userId), stoi(movieId), stoi(rating)});
    }
    ratingFile.close();

    // Membuat mapping dari user ke rating
    unordered_map<int, unordered_map<int, double>> userRatings;
    for (auto &rating : ratings)
    {
        userRatings[rating.userId][rating.movieId] = rating.rating;
    }

    // Merekomendasikan film untuk setiap user
    for (int userId = 1; userId <= 20; ++userId)
    {
        vector<pair<int, double>> recommendations = recommendMovies(userId, userRatings, ratings, 5);
        cout << "Recommended movies for user " << userId << ":" << endl;
        for (auto &rec : recommendations)
        {
            int movieId = rec.first;
            double score = rec.second;
            cout << movies[movieId - 1].title << " (" << movies[movieId - 1].genre << "): " << score << endl;
        }
        cout << endl;
    }

    return 0;
}