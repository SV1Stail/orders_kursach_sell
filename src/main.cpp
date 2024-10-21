#include "DB.h"
void finish_with_error(PGconn *conn) {
    std::cerr << "Ошибка: " << PQerrorMessage(conn) << std::endl;
    PQfinish(conn);
    exit(1);
}



int main() {

    const char *conninfo = "dbname=kursach user=user_db password=1234 hostaddr=127.0.0.1 port=5432";

    // Создаем объект базы данных
    Database db(conninfo);

    

    return 0;
}

