#include "storageeditor.h"

// Реализация конструктора StorageEditor
StorageEditor::StorageEditor(Storage *storage)
    : storage(storage)
{
    if (!storage) {
        qDebug() << "Warning: Storage pointer is null!";
    }
}
