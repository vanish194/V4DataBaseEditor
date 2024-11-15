#include "storageeditor.h"

StorageEditor::StorageEditor(Storage *storage)
    : storage(storage)
{
    if (!storage) {
        qDebug() << QObject::tr("Warning: Storage pointer is null!");
    }
}
