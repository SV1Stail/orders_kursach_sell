#include <string>
#include "DB.h"


// Класс для аутентификации пользователей
class Auth {
private:
    Database &db;

public:
    // Конструктор получает объект базы данных
    Auth(Database &database) : db(database) {}

    // Метод для проверки логина и пароля
    bool login(const std::string &username, const std::string &password) {
        // Формируем SQL-запрос для проверки пользователя
        std::string query = "SELECT * FROM users WHERE name = '" + username + "' AND pass = '" + password + "';";

        PGresult *res = db.executeQuery(query.c_str());
        if (!res) {
            return false; // Ошибка выполнения запроса
        }

        // Проверяем, есть ли записи, соответствующие запросу
        int rows = PQntuples(res);
        PQclear(res);

        // Если запись найдена, возвращаем true (аутентификация успешна)
        return rows > 0;
    }
    bool registerUser(const std::string &username, const std::string &password) {
    // Формируем SQL-запрос для вставки нового пользователя
    std::string query = "INSERT INTO users (name, pass) VALUES ('" + username + "', '" + password + "');";

    // Выполняем запрос к базе данных
    PGresult *res = db.executeQuery(query.c_str());


    // Освобождаем результат запроса
    PQclear(res);
    std::cout << "OK Процесс регистрации завершен" << std::endl;
    return true; // Регистрация успешна
}


};

