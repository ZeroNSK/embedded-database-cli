#include "set.h"
#include <cstring>
#include <iostream>

unsigned int Set::hashStr(const string& s) {
    unsigned int h = 0; // начальное значение хеша = 0
    for (char c : s) // идем по каждому символу строки
        h = h * 131 + (unsigned char)c; 
    return h % SET_BUCKETS;  // приводим хеш к диапазону корзин
}

Set::Set() {
    // обнуляем все указатели, чтобы список был пуст
    for (int i = 0; i < SET_BUCKETS; i++)
        buckets[i] = nullptr;
}

Set::~Set() {
    clear();
}

void Set::copyFrom(const Set& other) {
    // копируем все элементы из всех корзин
    for (int i = 0; i < SET_BUCKETS; i++) {
        SetNode* cur = other.buckets[i];
        while (cur) {
            add(cur->value);
            cur = cur->next;
        }
    }
}

Set::Set(const Set& other) {
    // обнуляем все указатели, чтобы список был пуст
    for (int i = 0; i < SET_BUCKETS; i++)
        buckets[i] = nullptr;
    copyFrom(other);
}

Set& Set::operator=(const Set& other) {
    if (this != &other) { // проверяем самоприсваивание
        clear();
        copyFrom(other);
    }
    return *this;
}

bool Set::isMember(const string& value) const {
    unsigned int h = hashStr(value); // вычисляем индекс корзины
    SetNode* cur = buckets[h]; // берём начало цепочки в этой корзине
    while (cur) {
        if (cur->value == value) return true;
        cur = cur->next;
    }
    return false;
}

bool Set::add(const string& value) {

    if (isMember(value)) return false; // если элемент уже есть - не добавляем

    unsigned int h = hashStr(value);
       
    // создаём новый узел: value + указатель на текущее начало цепочки
    SetNode* node = new SetNode(value, buckets[h]); 
    
    buckets[h] = node;

    return true;
}

bool Set::remove(const string& value) {
    unsigned int h = hashStr(value);
    SetNode* cur = buckets[h]; // текущий узел
    SetNode* prev = nullptr; // предыдущий для удаления

    while (cur) {
        if (cur->value == value) {
            if (prev) prev->next = cur->next; // если элемент не первый, то пропускаем удаляемый узел
            else      buckets[h] = cur->next; // если первый узел — смещаем голову
            delete cur;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

void Set::clear() {
    for (int i = 0; i < SET_BUCKETS; i++) {
        SetNode* cur = buckets[i]; 
        while (cur) {
            SetNode* tmp = cur; // пока цепочка не пустая - сохраняем текущий
            cur = cur->next;
            delete tmp;  // удаляем текущий узел
        }
        buckets[i] = nullptr;
    }
}
