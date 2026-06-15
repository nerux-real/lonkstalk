#pragma once
#include <string>
#include <vector>
#include "sqlite3.h"

struct ScoreEntry {
    std::string lkHash;
    std::string difficulty;
    int score;
    float accuracy;
    int maxCombo;
    std::string grade;
    int excellentCounts;
    int goodCounts;
    int missCounts;
    long timestamp;
};

class Database {
    sqlite3 *m_db=nullptr;
    std::vector<ScoreEntry> m_scores;
public:
    bool init(const std::string &path);
    void saveScore(const ScoreEntry &entry);
    void getScores(const std::string &lkHash, const std::string &difficulty, std::vector<ScoreEntry> &outScores);
    std::vector<ScoreEntry> getScores(const std::string &lkHash, const std::string &difficulty);
    void close();
};