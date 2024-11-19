cmake_minimum_required(VERSION 3.14)
project(V4DataBaseEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find necessary Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Sql Gui Widgets LinguistTools)

# Enable automatic processing of MOC, UIC, and RCC files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set the search paths for UIC
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/editor_windows")

# Source files
set(SOURCES
    databasemanager.cpp
    databasesaver.cpp
    db_model/additionalmnemonic.cpp
    db_model/company.cpp
    db_model/conversionformula.cpp
    db_model/mainmnemonic.cpp
    db_model/method.cpp
    db_model/producer.cpp
    db_model/sensor.cpp
    db_model/sensormnemonic.cpp
    db_model/tool.cpp
    db_model/tooldescription.cpp
    db_model/toolmnemonic.cpp
    db_model/toolsensor.cpp
    db_model/unit.cpp
    db_model/unittype.cpp
    editor_windows/additionalmnemonicaddwindow.cpp
    editor_windows/additionalmnemoniceditor.cpp
    editor_windows/comparedatadialog.cpp
    editor_windows/mainmnemonicaddwindow.cpp
    editor_windows/mainmnemoniceditor.cpp
    editor_windows/sensor_editor.cpp
    editor_windows/sensoraddwindow.cpp
    editor_windows/sensormnemonicrelationeditor.cpp
    editor_windows/tool_editor.cpp
    editor_windows/tooladdwindow.cpp
    editor_windows/toolmnemonicrelationeditor.cpp
    editor_windows/toolsensorrelationeditor.cpp
    main.cpp
    mainwindow.cpp
    storage.cpp
    storageeditor.cpp
    views/toolsensormnemonictreeview.cpp
    resources.qrc 
)

# Header files
set(HEADERS
    databasemanager.h
    databasesaver.h
    db_model/additionalmnemonic.h
    db_model/company.h
    db_model/conversionformula.h
    db_model/mainmnemonic.h
    db_model/method.h
    db_model/producer.h
    db_model/sensor.h
    db_model/sensormnemonic.h
    db_model/tool.h
    db_model/tooldescription.h
    db_model/toolmnemonic.h
    db_model/toolsensor.h
    db_model/unit.h
    db_model/unittype.h
    editor_windows/additionalmnemonicaddwindow.h
    editor_windows/additionalmnemoniceditor.h
    editor_windows/comparedatadialog.h
    editor_windows/mainmnemonicaddwindow.h
    editor_windows/mainmnemoniceditor.h
    editor_windows/sensor_editor.h
    editor_windows/sensoraddwindow.h
    editor_windows/sensormnemonicrelationeditor.h
    editor_windows/tool_editor.h
    editor_windows/tooladdwindow.h
    editor_windows/toolmnemonicrelationeditor.h
    editor_windows/toolsensorrelationeditor.h
    mainwindow.h
    storage.h
    storageeditor.h
    views/toolsensormnemonictreeview.h
)

# Translation files
set(TRANSLATION_FILES
    translations/V4DataBaseEditor_ru.ts
)
# Process translations
qt6_add_translation(QM_FILES ${TRANSLATION_FILES})

# Add a custom target for translations
add_custom_target(translations ALL DEPENDS ${QM_FILES})

# Create the executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

# Make the executable depend on the translations
add_dependencies(${PROJECT_NAME} translations)

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} # Include the directory for generated headers
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Sql
    Qt6::Gui
    Qt6::Widgets
)

# Copy translation files after building them
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/translations"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QM_FILES} "$<TARGET_FILE_DIR:${PROJECT_NAME}>/translations"
)

# Optional: Output the paths of QM_FILES for debugging
message(STATUS "QM_FILES: ${QM_FILES}")