# V4DataBaseEditor

Краткая инструкция по сборке и запуску (Windows, Qt6 + CMake):

Требования:
- Qt 6 (dev tools, Qt6 CMake targets)
- CMake >= 3.14
- Компилятор с поддержкой C++17

Сборка (в PowerShell или CMD):

```powershell
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH="C:\\Qt\\6.x.x\\msvc2019_64"  # укажите путь к Qt
cmake --build . --config Release
```

Запуск:

```powershell
cd build\Release
V4DataBaseEditor.exe
```

Изменения, внесённые в репозиторий:
- Добавлён простой механизм миграций SQL в `DatabaseManager`.
  Миграции ищутся в папке `migrations/` и должны иметь имена `upgrade_<from>_<to>.sql`.
- Добавлен файловый лог (`logs/app.log`) через перехват сообщений Qt.
- Убран принудительный поворот изображений при показе/resize в `MainWindow`.
- Добавлен `README.md` с инструкцией по сборке.

Примечания:
- Миграции должны быть атомарными SQL-скриптами; текущая реализация делит скрипт по `;`.
- CI и тесты не добавлены автоматически; рекомендую добавить базовые unit-тесты и GitHub Actions позже.
