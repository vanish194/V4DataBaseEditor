#ifndef STORAGEEDITOR_H
#define STORAGEEDITOR_H

#include <QDebug>
#include <QLocale>
#include "storage.h"
class StorageEditor
{
public:
    explicit StorageEditor(Storage *storage);

    // Function to add or replace an element
    template<typename T>
    void insertOrReplace(QList<T> &list, const T &element);

    // Function to remove an element
    template<typename T>
    void deleteElement(QList<T> &list, const T &element);

private:
    Storage *storage;
};

template<typename T>
void StorageEditor::insertOrReplace(QList<T> &list, const T &element)
{
    for (int i = 0; i < list.size(); ++i) {
        if (list[i].getId() == element.getId()) {
            list[i] = element; // Replacing an element
            qDebug() << "Element with ID" << element.getId() << "replaced.";
            return;
        }
    }
    list.append(element); // Adding a new element
    qDebug() << "Element with ID" << element.getId() << "added.";
}

template<typename T>
void StorageEditor::deleteElement(QList<T> &list, const T &element)
{
    for (int i = 0; i < list.size(); ++i) {
        if (list[i].getId() == element.getId()) {
            list[i].markAsDeleted();
            qDebug() << "Element with ID" << element.getId() << "marked as deleted.";
            return;
        }
    }
    qDebug() << "Element with ID" << element.getId() << "not found for deletion.";
}

#endif // STORAGEEDITOR_H
