#include <iostream>
#include <libpq-fe.h>

void exit_nicely(PGconn* conn) {
    PQfinish(conn);
    exit(1);
}

int main() {
    // 连接数据库
    PGconn* conn = PQconnectdb("host=localhost port=5432 dbname=my_db user=postgres password=tangjicheng12");

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        exit_nicely(conn);
    }

    // 创建表（如果还不存在）
    const char* create_table_sql =
        "CREATE TABLE IF NOT EXISTS Users ("
        "Id SERIAL PRIMARY KEY, "
        "Name VARCHAR(100) NOT NULL, "
        "Email VARCHAR(100) NOT NULL UNIQUE"
        ")";
    PGresult* res = PQexec(conn, create_table_sql);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "CREATE TABLE command failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        exit_nicely(conn);
    }

    PQclear(res);

    // 插入数据
    const char* insert_sql = "INSERT INTO Users (Name, Email) VALUES ('John Doe', 'john.doe@example.com')";
    res = PQexec(conn, insert_sql);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "INSERT command failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        exit_nicely(conn);
    }

    PQclear(res);

    // 查询数据
    const char* query_sql = "SELECT Id, Name, Email FROM Users";
    res = PQexec(conn, query_sql);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "SELECT command failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        exit_nicely(conn);
    }

    int nrows = PQntuples(res);
    int nfields = PQnfields(res);

    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < nfields; j++) {
            std::cout << PQgetvalue(res, i, j) << (j < nfields - 1 ? "\t" : "\n");
        }
    }

    PQclear(res);
    PQfinish(conn);

    return 0;
}
