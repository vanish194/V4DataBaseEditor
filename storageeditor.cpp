#include "storageeditor.h"

StorageEditor::StorageEditor(Storage *storage)
    : storage(storage)
{
    if (!storage) {
        qDebug() << "Warning: Storage pointer is null!";
    }
}
