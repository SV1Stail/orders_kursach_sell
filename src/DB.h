#include <iostream>
#include <libpq-fe.h>

class Database {
private:
    PGconn *conn;

    void finish_with_error() {
        std::cerr << "Ошибка подключения: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        exit(1);
    }

public:

    // Конструктор: подключается к БД
    Database(const char *conninfo) {
        conn = PQconnectdb(conninfo);
        if (PQstatus(conn) != CONNECTION_OK) {
            finish_with_error();
        }
    }

    // SELECTы
    PGresult* executeQuery(const char *query) {
        PGresult *res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "Ошибка выполнения запроса: " << PQerrorMessage(conn) << std::endl;
            PQclear(res);
            return nullptr;
        }

        return res;
    }
        // INSERTы
    bool insertOrder(const std::string &customer_name, const std::string &product_name, int quantity) {
        // Формируем SQL-запрос для вставки
        std::string query = "INSERT INTO orders (customer_name, product_name, quantity) VALUES ('" 
                            + customer_name + "', '" 
                            + product_name + "', " 
                            + std::to_string(quantity) + ");";

        PGresult *res = PQexec(conn, query.c_str());

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Ошибка вставки записи: " << PQerrorMessage(conn) << std::endl;
            PQclear(res);
            return false;
        }

        PQclear(res);
        return true;
    }
    // DELETEы
    bool deleteOrderById(int id) {
        std::string query = "DELETE FROM orders WHERE id = " + std::to_string(id) + ";";

        PGresult *res = PQexec(conn, query.c_str());

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Ошибка удаления записи: " << PQerrorMessage(conn) << std::endl;
            PQclear(res);
            return false;
        }

        PQclear(res);
        return true;
    }

    // UPDATEы
    bool updateOrderById(int id, const std::string &customer_name, const std::string &product_name, int quantity) {
        std::string query = "UPDATE orders SET customer_name = '" + customer_name + "', "
                            "product_name = '" + product_name + "', "
                            "quantity = " + std::to_string(quantity) + " "
                            "WHERE id = " + std::to_string(id) + ";";

        PGresult *res = PQexec(conn, query.c_str());

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Ошибка обновления записи: " << PQerrorMessage(conn) << std::endl;
            PQclear(res);
            return false;
        }

        PQclear(res);
        return true;
    }

    void printDB(PGresult* res){
        if (res) {
        int rows = PQntuples(res);
        int cols = PQnfields(res);

        for (int i = 0; i < cols; i++) {
            std::cout << PQfname(res, i) << "\t";
        }
        std::cout << std::endl;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                std::cout << PQgetvalue(res, i, j) << "\t";
            }
            std::cout << std::endl;
        }

        PQclear(res);
    }

    }


    // Деструктор: закрывает соединение
    ~Database() {
        PQfinish(conn);
    }
};
