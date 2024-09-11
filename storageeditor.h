#ifndef STORAGEEDITOR_H
#define STORAGEEDITOR_H

#include "storage.h"

class StorageEditor
{
public:
    explicit StorageEditor(Storage *storage);

    // Функция для добавления или замены элемента
    template<typename T>
    void insertOrReplace(QList<T> &list, const T &element);

    // Функция для удаления элемента
    template<typename T>
    void deleteElement(QList<T> &list, const T &element);

private:
    Storage *storage;
};

// Реализация конструктора
StorageEditor::StorageEditor(Storage *storage)
    : storage(storage)
{}

// Реализация функции InsertOrReplace
template<typename T>
void StorageEditor::insertOrReplace(QList<T> &list, const T &element)
{
    for (int i = 0; i < list.size(); ++i) {
        if (list[i].getId() == element.getId()) {
            list[i] = element; // Замена элемента
            qDebug() << "Element with ID" << element.getId() << "replaced.";
            return;
        }
    }
    list.append(element); // Добавление нового элемента
    qDebug() << "Element with ID" << element.getId() << "added.";
}

// Реализация функции Delete
template<typename T>
void StorageEditor::deleteElement(QList<T> &list, const T &element)
{
    for (int i = 0; i < list.size(); ++i) {
        if (list[i].getId() == element.getId()) {
            list[i].markAsDeleted(); // Помечаем элемент как удаленный
            qDebug() << "Element with ID" << element.getId() << "marked as deleted.";
            return;
        }
    }
    qDebug() << "Element with ID" << element.getId() << "not found for deletion.";
}

#endif // STORAGEEDITOR_H
