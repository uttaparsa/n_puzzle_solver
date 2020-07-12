#include "BitsetHash.h"
#include "VectorState.h"
#include <iostream>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#ifndef DISJOINTDB_H
#define DISJOINTDB_H
const smallInt DUMMY = 15;
const smallInt DUMMY2 = 8;
sqlite3 *dbs[3];
int h;
// int h555;
int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    if (argc == 0) {
        std::cout << " Callback function error : No Record found for this ID!!! " << argc << std::endl;
    }

    for (i = 0; i < argc; i++) {
        // printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        h += std::stoi(argv[i]);
    }
    // printf("\n");
    return 0;
}
void printTable(sqlite3 *db) {
    char *zErrMsg = 0;
    const char *data = "Callback function called";
    std::string sql;
    int rc;
    /* Create SQL statement */
    sql = "SELECT * from HeuristicTable";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callback, (void *)data, &zErrMsg);
}
int openDatabase(sqlite3 *db, std::string dbname) {
    /* Open database */
    int rc = sqlite3_open(dbname.c_str(), &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return (0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
    return 1;
}
int openDatabases() {

    int rc;
    char *zErrMsg = 0;
    const char *data = "OpenDB function called";
    rc = sqlite3_open("heuristics6-1.db", &dbs[0]);
    rc = sqlite3_open("heuristics6-2.db", &dbs[1]);
    rc = sqlite3_open("heuristics3-3.db", &dbs[2]);
    // rc = sqlite3_open("heuristics5-1.db", &dbs[3]);
    // rc = sqlite3_open("heuristics5-2.db", &dbs[4]);
    // rc = sqlite3_open("heuristics5-3.db", &dbs[5]);

    if (rc) {
        fprintf(stderr, "Can't open database1: %s\n", sqlite3_errmsg(dbs[0]));
        return (0);
    } else {
        fprintf(stderr, "Opened database1 successfully\n");
    }

    if (rc) {
        fprintf(stderr, "Can't open database2: %s\n", sqlite3_errmsg(dbs[1]));
        return (0);
    } else {
        fprintf(stderr, "Opened database2 successfully\n");
    }
    if (rc) {
        fprintf(stderr, "Can't open database3: %s\n", sqlite3_errmsg(dbs[2]));
        return (0);
    } else {
        fprintf(stderr, "Opened database3 successfully\n");
    }
    return 1;
}
state_holder firstPartState(state_holder &state) {
    state_holder clone = state;
    for (int i = 0; i < clone.size(); i++) {
        if (clone[i] > 5) {
            clone[i] = DUMMY;
        }
    }
    return clone;
}
state_holder secondPartState(state_holder &state) {
    state_holder clone = state;
    for (int i = 0; i < clone.size(); i++) {
        if ((clone[i] <= 5 || clone[i] > 10) && clone[i] != 0) {
            clone[i] = DUMMY;
        }
    }
    return clone;
}
state_holder thirdPartState(state_holder &state) {
    state_holder clone = state;
    for (int i = 0; i < clone.size(); i++) {
        if (clone[i] <= 10 && clone[i] != 0) {
            clone[i] = 8;
        }
    }
    return clone;
}
state_holder firstPartState2(state_holder &state) {
    state_holder clone = state;
    for (int i = 0; i < clone.size(); i++) {
        if (clone[i] > 6) {
            clone[i] = DUMMY;
        }
    }
    return clone;
}
state_holder secondPartState2(state_holder &state) {
    state_holder clone = state;
    for (int i = 0; i < clone.size(); i++) {
        if ((clone[i] <= 6 || clone[i] > 12) && clone[i] != 0) {
            clone[i] = DUMMY;
        }
    }
    return clone;
}
state_holder thirdPartState2(state_holder &state) {
    state_holder clone = state;
    for (int i = 0; i < clone.size(); i++) {
        if (clone[i] <= 12 && clone[i] != 0) {
            clone[i] = 8;
        }
    }
    return clone;
}

int disjointDatabaseHeuristic(state_holder &state) {
    state_holder statePartOne = firstPartState2(state);
    // std::cout << printState(statePartOne) << std::endl;
    state_holder statePartTwo = secondPartState2(state);
    // std::cout << printState(statePartTwo) << std::endl;
    state_holder statePartThree = thirdPartState2(state);
    // std::cout << printState(statePartThree) << std::endl;
    std::string sql;
    int rc;
    char *zErrMsg = 0;
    const char *data = "SELECT function called";
    h = 0;
    sql = "SELECT HEURISTIC from HeuristicTable WHERE ID = " +
          std::to_string((long long int)makeHash(statePartOne));

    /* Execute SQL statement */
    rc = sqlite3_exec(dbs[0], sql.c_str(), callback, (void *)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        // fprintf(stdout, "Operation done successfully\n");
    }
    sql = "SELECT HEURISTIC from HeuristicTable WHERE ID = " +
          std::to_string((long long int)makeHash(statePartTwo));
    rc = sqlite3_exec(dbs[1], sql.c_str(), callback, (void *)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        // fprintf(stdout, "Operation done successfully\n");
    }
    sql = "SELECT HEURISTIC from HeuristicTable WHERE ID = " +
          std::to_string((long long int)makeHash(statePartThree));
    rc = sqlite3_exec(dbs[2], sql.c_str(), callback, (void *)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        // fprintf(stdout, "Operation done successfully\n");
    }

    // std::cout << "found heuristic using disjount : " << h << std::endl;
    return h;
}
#endif