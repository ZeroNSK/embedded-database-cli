#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <locale.h>
#include "database.h"

using namespace std;

int main(int argc, char** argv) {
    setlocale(LC_ALL, "RU");
    string filepath; 
    string query;

    for (int i = 1; i < argc; i++) { // проходи1м по аргументам
        string arg = argv[i];

        if (arg == "--file" && i + 1 < argc) { 
            filepath = argv[++i]; // следующий аргумент — путь к файлу
        }
        else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i]; // следующий аргумент — запрос
        }
    }

    if (filepath.empty() || query.empty()) {
        cout << "Ошибка, используй: --file <путь к файлу> --query <команда>" << endl;
        return 1;
    }

    Database db; // создаём базу данных
    db.load(filepath); // загружаем из файла


    istringstream ss(query);

    string cmd;
    ss >> cmd;   // читаем команду

    string result; 

    if (cmd == "SADD") {
        string name, value;
        ss >> name >> value; // читаем имя множества и значение

        // если такого множества нет — создаём
        if (!db.sets.count(name)) {
            db.sets[name] = Set(); 
        }

        bool ok = db.sets[name].add(value); // добавляем значение
        result = ok ? value : ""; // если добавлено — возвращаем значение, иначе пустую строку
    } 
    else if (cmd == "SREM") {  
        string name, value;
        ss >> name >> value; // читаем имя множества и значение

        if (db.sets.count(name)) {
            bool ok = db.sets[name].remove(value); // удаляем значение
            result = ok ? value : ""; // если удалено — возвращаем значение, иначе пустую строку
        }
    }
    else if (cmd == "SISMEMBER") { 
        string name, value;
        ss >> name >> value; // читаем имя множества и значение

        if (db.sets.count(name)) {
            bool exists = db.sets[name].isMember(value); // проверяем наличие
            result = exists ? "TRUE" : "FALSE"; // если есть — TRUE, иначе FALSE
        } else {
            result = "FALSE";
        }
    }

    else if (cmd == "SPUSH") {
        string name, value;
        ss >> name >> value; // читаем имя стека и значение

        if (!db.stacks.count(name)) {
            db.stacks[name] = Stack();
        }

        db.stacks[name].push(value); // добавляем значение в стек
        result = value;
    }

    else if (cmd == "SPOP") {
        string name;
        ss >> name; // читаем имя стека

        if (db.stacks.count(name)) {
            string v;
            bool ok = db.stacks[name].pop(v); // удаляем значение из стека
            result = ok ? v : "";
        }
    }
    else if (cmd == "QPUSH") {
        string name, value;
        ss >> name >> value; // читаем имя очереди и значение

        if (!db.queues.count(name)) {
            db.queues[name] = Queue();
        }

        db.queues[name].push(value); // добавляем значение в очередь
        result = value;
    }

    else if (cmd == "QPOP") {
        string name;
        ss >> name; // читаем имя очереди

        if (db.queues.count(name)) {
            string v;
            bool ok = db.queues[name].pop(v); // удаляем значение из очереди
            result = ok ? v : "";
        }
    }
    else if (cmd == "HSET") {
        string name, key, value;
        ss >> name >> key >> value; // читаем имя хеша, ключ и значение

        if (!db.hashes.count(name)) {
            db.hashes[name] = Hash();
        }

        db.hashes[name].set(key, value); // устанавливаем ключ-значение
        result = value; // возвращаем значение
    }

    else if (cmd == "HDEL") {
        string name, key;
        ss >> name >> key; // читаем имя хеша и ключ

        if (db.hashes.count(name)) {
            bool ok = db.hashes[name].del(key); // удаляем ключ
            result = ok ? key : ""; // если удалено — возвращаем ключ, иначе пустую строку
        }
    }

    else if (cmd == "HGET") {
        string name, key;
        ss >> name >> key; // читаем имя хеша и ключ

        if (db.hashes.count(name)) {
            string v;
            bool ok = db.hashes[name].get(key, v); // получаем значение по ключу
            result = ok ? v : ""; // если найдено — возвращаем значение, иначе пустую строку
        }
    }

    else if (cmd == "TINSERT") {
        string name, value;
        ss >> name >> value; // читаем имя дерева и значение

        if (!db.trees.count(name)) {
            db.trees[name] = BinaryTree();
        }

        db.trees[name].insert(value); // добавляем значение в дерево
        result = value; // возвращаем значение
    }

    else if (cmd == "TFIND") {
        string name, value;
        ss >> name >> value; // читаем имя дерева и значение

        if (db.trees.count(name)) {
            string path;
            int position;
            bool ok = db.trees[name].find(value, path, position); // ищем значение в дереве
            
            if (ok) {
                result = path + " " + to_string(position); // возвращаем путь и позицию
            } else {
                result = ""; // если не найдено — пустая строка
            }
        }
    }

    else {
        cout << "Ошибка: неизвестная команда " << cmd << endl;
        return 1;
    }
    db.save(filepath);
    cout << result << endl;

    return 0;
}
