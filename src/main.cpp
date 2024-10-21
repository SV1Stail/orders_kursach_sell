#include "auth.h"
void finish_with_error(PGconn *conn) {
    std::cerr << "Ошибка: " << PQerrorMessage(conn) << std::endl;
    PQfinish(conn);
    exit(1);
}

bool loginCredentials(std::string &username, std::string &password, Auth &auth) {
    std::cout << "Введите имя пользователя: ";
    std::getline(std::cin, username);

    std::cout << "Введите пароль: ";
    std::getline(std::cin, password);
    bool flag = false;
    if (auth.login(username, password)) {
        std::cout << "OK login" << std::endl; // Успешная авторизация
        flag = true;
    } else {
        std::cout << "Неверное имя пользователя или пароль попробуйте снова" << std::endl;
        flag = loginCredentials(username,password,auth);

    }
return flag;
}

bool signinCredentials(std::string &username, std::string &password, Auth &auth) {
    std::cout << "Введите новое имя пользователя: ";
    std::getline(std::cin, username);

    std::cout << "Введите новый пароль: ";
    std::getline(std::cin, password);
    bool flag = false;
    if (auth.registerUser(username, password)) {
        std::cout << "OK signin" << std::endl; // Успешная авторизация
        flag = true;
    } else {
        std::cout << "чтото пошлот не так. Попробуйте снова" << std::endl;
        flag = signinCredentials(username,password,auth);

    }
return flag;
}

void signinOrLogin(Auth &auth) {
    std::string username, password,reg;
    std::cout << "1 чтобы зарегестрироваться\n2 чтобы войти по логину и паролю\n";
    std::getline(std::cin, reg);
    if (reg == "2"){
        if (loginCredentials(username, password, auth)) {
            std::cout << "OK Успешный вход" << std::endl; // Успешная авторизация
        } else {
            std::cout << "2 Неверное имя пользователя или пароль" << std::endl;
        }
    }else if (reg=="1"){
        if (signinCredentials(username, password, auth)) {
            // std::cout << "OK main 1" << std::endl; // Успешная авторизация
        } 
        signinOrLogin(auth);
    }else{
        std::cout << "Вы ввели чтото не понятное, пробуйте снова" << std::endl; 
        signinOrLogin(auth);
    }
}


void work(Database &db){
    std::cout << "какую операцию хотите совершить?\n1 Вставка данных в БД\n2 Изменение данных в БД\n3 Удаление данных из БД" << std::endl; 
    std::string operation,customer_name,product_name,quantity_str,id_str;
    int quantity = 0,id=-1;
    std::getline(std::cin, operation);
    if (operation == "1") // INSERT
    {
        std::cout << "Введите имя клиента: ";
        std::getline(std::cin, customer_name);

        std::cout << "Введите название продукта: ";
        std::getline(std::cin, product_name);

        std::cout << "Введите количество продукта: ";
        std::getline(std::cin, quantity_str);
        try {
            quantity = std::stoi(quantity_str);
            if (db.insertOrder(customer_name,product_name,quantity))
            {
                        std::cout << "Запись успешно добавлена в таблицу orders!\n" << std::endl;
            }else
            {
                        std::cout << "Ошибка при добавлении записи.\n" << std::endl;
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Ошибка: Количество не является числом" << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Ошибка: число слишком велико" << std::endl;
        }
        
    }else if (operation == "2") // UPDATE
    {
        std::cout << "Введите ID клиента: ";
        std::getline(std::cin, id_str);

        try {
            id = std::stoi(id_str);
            std::cout << "Введите новое имя клиента: ";
            std::getline(std::cin, customer_name);

            std::cout << "Введите новое название продукта: ";
            std::getline(std::cin, product_name);

            std::cout << "Введите новое  количество продукта: ";
            std::getline(std::cin, quantity_str);

            try {
            quantity = std::stoi(quantity_str);
            if (db.updateOrderById(id,customer_name,product_name,quantity))
            {
                std::cout << "Запись успешно обновлена!\n" << std::endl;
            }else
            {
                std::cout << "Ошибка при обновлении записи.\n" << std::endl;
            }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Ошибка: количество не является числом" << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Ошибка: количество слишком велико" << std::endl;
            }

        } catch (const std::invalid_argument& e) {
            std::cerr << "Ошибка: ID не является числом" << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Ошибка: ID слишком велико" << std::endl;
        }

        
    }else if (operation == "3") // DELETE
    {
        std::cout << "Введите ID клиента: ";
        std::getline(std::cin, id_str);
            try {
            id = std::stoi(id_str);
            if (db.deleteOrderById(id))
            {
                std::cout << "Запись успешно удалена!\n" << std::endl;
            }else
            {
                std::cout << "Ошибка при удалении записи.\n" << std::endl;
            }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Ошибка: ID не является числом" << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Ошибка: ID слишком велико" << std::endl;
            }
    }else{
        std::cout << "Вы ввели чтото не понятное, пробуйте снова" << std::endl; 
    }
    work(db);
    
}


int main() {

    const char *conninfo = "dbname=kursach user=user_db password=1234 hostaddr=127.0.0.1 port=5432";

    Database db(conninfo);
    Auth auth(db);
    signinOrLogin(auth);

    work(db);
    return 0;
}

