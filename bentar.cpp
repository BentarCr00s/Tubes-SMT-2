// Fungsi untuk merekomendasikan film berdasarkan rating dari user-user yang memiliki similarity yang tinggi
vector<pair<int, double>> recommendMovies(vector<int> &userIds, unordered_map<int, unordered_map<int, double>> &userRatings, vector<Rating> &allRatings, int numRecs)
{
    unordered_map<int, unordered_map<int, double>> selectedUserRatings;
    for (auto &userId : userIds)
    {
        selectedUserRatings[userId] = userRatings[userId];
    }

    // Menghitung similarity antar user
    vector<pair<int, double>> similarities;
    for (auto &p1 : selectedUserRatings)
    {
        int userId1 = p1.first;
        unordered_map<int, double> user1 = p1.second;
        for (auto &p2 : selectedUserRatings)
        {
            int userId2 = p2.first;
            if (userId2 > userId1)
            {
                unordered_map<int, double> user2 = p2.second;
                double sim = similarity(user1, user2);
                similarities.push_back({userId1 * 1000 + userId2, sim});
            }
        }
    }

    // Mengurutkan user berdasarkan similarity
    sort(similarities.begin(), similarities.end(), [](const pair<int, double> &p1, const pair<int, double> &p2)
         { return p1.second > p2.second; });

    // Merekomendasikan film
    unordered_map<int, double> movieScores;
    for (auto &p : similarities)
    {
        int userId1 = p.first / 1000;
        int userId2 = p.first % 1000;
        double sim = p.second;
        unordered_map<int, double> user2 = selectedUserRatings[userId2];
        for (auto &p2 : user2)
        {
            int movieId = p2.first;
            double rating = p2.second;
            if (!selectedUserRatings[userId1].count(movieId))
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
    int n = movieScores.size();
    int movieIds[n];
    double scores[n];
    int i = 0;
    for (auto &p : movieScores)
    {
        movieIds[i] = p.first;
        scores[i] = p.second;
        i++;
    }
    sort(movieIds, movieIds + n, [&](int a, int b)
         { return scores[a] > scores[b]; });
    vector<pair<int, double>> topRecs;
    for (int i = 0; i < numRecs && i < n; i++)
    {
        int movieId = movieIds[i];
        double score = scores[movieId];
        topRecs.push_back({movieId, score});
    }
    return topRecs;
}