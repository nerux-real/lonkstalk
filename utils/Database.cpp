#include "Database.h"
#include <iostream>
#include <unordered_set>

bool Database::init(const std::string& path) {
    if (sqlite3_open(path.c_str(), &m_db) != SQLITE_OK) {
        std::cerr << "Failed to open database: " 
                  << sqlite3_errmsg(m_db) << "\n";
        sqlite3_close(m_db);
        m_db = nullptr;
        return false;
    }

    const char* createTable =
        "CREATE TABLE IF NOT EXISTS scores ("
        "lk_hash TEXT NOT NULL,"
        "difficulty TEXT NOT NULL,"
        "score INTEGER NOT NULL,"
        "accuracy REAL NOT NULL,"
        "max_combo INTEGER NOT NULL,"
        "grade TEXT NOT NULL,"
        "excellent_count INTEGER NOT NULL,"
        "good_count INTEGER NOT NULL,"
        "miss_count INTEGER NOT NULL,"
        "no_fail INTEGER NOT NULL DEFAULT 0,"
        "timestamp INTEGER NOT NULL DEFAULT (strftime('%s','now'))"
        ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, createTable, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to create table (" << rc << "): "
                  << errMsg << "\n";
        sqlite3_free(errMsg);
        sqlite3_close(m_db);
        m_db = nullptr;
        return false;
    }

    const std::vector<std::pair<std::string, std::string>> requiredColumns = {
        { "lk_hash",        "TEXT NOT NULL DEFAULT ''" },
        { "difficulty",     "TEXT NOT NULL DEFAULT ''" },
        { "score",          "INTEGER NOT NULL DEFAULT 0" },
        { "accuracy",       "REAL NOT NULL DEFAULT 0.0" },
        { "max_combo",      "INTEGER NOT NULL DEFAULT 0" },
        { "grade",          "TEXT NOT NULL DEFAULT ''" },
        { "excellent_count","INTEGER NOT NULL DEFAULT 0" },
        { "good_count",     "INTEGER NOT NULL DEFAULT 0" },
        { "miss_count",     "INTEGER NOT NULL DEFAULT 0" },
        { "no_fail",        "INTEGER NOT NULL DEFAULT 0" },
        { "timestamp",      "INTEGER NOT NULL DEFAULT (strftime('%s','now'))" },
    };

    std::unordered_set<std::string> existingColumns;
    {
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(m_db, "PRAGMA table_info(scores);",
                               -1, &stmt, nullptr) == SQLITE_OK)
        {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                const char* colName =
                    reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                if (colName) existingColumns.insert(colName);
            }
        }
        sqlite3_finalize(stmt);
    }

    for (const auto& [colName, colDef] : requiredColumns) {
        if (existingColumns.count(colName)) continue;

        std::string alterSQL =
            "ALTER TABLE scores ADD COLUMN " + colName + " " + colDef + ";";

        if (sqlite3_exec(m_db, alterSQL.c_str(), nullptr, nullptr, &errMsg)
            != SQLITE_OK)
        {
            std::cerr << "Failed to add column '" << colName << "': "
                      << errMsg << "\n";
            sqlite3_free(errMsg);
        } else {
            std::cout << "Schema migration: added column '" << colName << "'\n";
        }
    }

    return true;
}

void Database::close(){
    if(m_db) sqlite3_close(m_db);
    m_db = nullptr;
}

void Database::saveScore(const ScoreEntry &entry){
    const char* insertSQL = "INSERT INTO scores (lk_hash, difficulty, score, accuracy, max_combo, grade, excellent_count, good_count, miss_count, no_fail) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(m_db, insertSQL, -1, &stmt, nullptr) != SQLITE_OK){
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, entry.lkHash.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, entry.difficulty.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, entry.score);
    sqlite3_bind_double(stmt, 4, entry.accuracy);
    sqlite3_bind_int(stmt, 5, entry.maxCombo);
    sqlite3_bind_text(stmt, 6, entry.grade.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 7, entry.excellentCounts);
    sqlite3_bind_int(stmt, 8, entry.goodCounts);
    sqlite3_bind_int(stmt, 9, entry.missCounts);
    sqlite3_bind_int(stmt, 10, entry.noFail);
    //sqlite3_bind_int64(stmt, 11, entry.timestamp);

    if(sqlite3_step(stmt) != SQLITE_DONE){
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(m_db) << "\n";
    }

    sqlite3_finalize(stmt);
}

void Database::getScores(const std::string &lkHash, const std::string &difficulty, std::vector<ScoreEntry> &outScores){
    const char* selectSQL = "SELECT lk_hash, difficulty, score, accuracy, max_combo, grade, excellent_count, good_count, miss_count, no_fail, timestamp FROM scores WHERE lk_hash = ? AND difficulty = ? ORDER BY score DESC;";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(m_db, selectSQL, -1, &stmt, nullptr) != SQLITE_OK){
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, lkHash.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, difficulty.c_str(), -1, SQLITE_TRANSIENT);

    while(sqlite3_step(stmt) == SQLITE_ROW){
        ScoreEntry entry;
        entry.lkHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        entry.difficulty = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        entry.score = sqlite3_column_int(stmt, 2);
        entry.accuracy = static_cast<float>(sqlite3_column_double(stmt, 3));
        entry.maxCombo = sqlite3_column_int(stmt, 4);
        entry.grade = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        entry.excellentCounts = sqlite3_column_int(stmt, 6);
        entry.goodCounts = sqlite3_column_int(stmt, 7);
        entry.missCounts = sqlite3_column_int(stmt, 8);
        entry.noFail = sqlite3_column_int(stmt, 9);
        entry.timestamp = static_cast<long>(sqlite3_column_int64(stmt, 10));

        outScores.push_back(entry);
    }

    sqlite3_finalize(stmt);
}