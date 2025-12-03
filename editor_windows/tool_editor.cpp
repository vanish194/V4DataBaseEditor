#include "editor_windows/tool_editor.h"
#include "storage.h"
#include "storageeditor.h"

ToolEditor::ToolEditor(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    // Подключаем кнопку Save к слоту обработки
    connect(ui.pushButton_save, &QPushButton::clicked, this, &ToolEditor::onSaveButtonClicked);
}

void ToolEditor::setTool(const Tool &tool)
{
    currentTool = tool;

    // Устанавливаем данные инструмента в элементы формы
    ui.lineEdit_name->setText(tool.getName());

    // Получаем описание инструмента из хранилища
    ToolDescription description;
    for (const ToolDescription &desc : Storage::getInstance()->getToolDescriptions()) {
        if (desc.getId() == tool.getToolDescriptionId()) {
            description = desc;
            break;
        }
    }

    ui.lineEdit_description->setText(description.getDescription());
    ui.spinBox_length->setValue(description.getLengthMm());
    ui.spinBox_outerDiameter->setValue(description.getOuterDiameterMm());
    ui.spinBox_innerDiameter->setValue(description.getInnerDiameterMm());

    // Получаем производителя и устанавливаем в выпадающий список
    for (const Producer &producer : Storage::getInstance()->getProducers()) {
        ui.comboBox_producer->addItem(producer.getName(), QVariant(producer.getId()));
        if (producer.getId() == description.getProducerId()) {
            ui.comboBox_producer->setCurrentText(producer.getName());
        }
    }
}

Tool ToolEditor::getTool() const
{
    // Обновляем информацию об инструменте
    Tool updatedTool = currentTool;
    updatedTool.setName(ui.lineEdit_name->text());

    // Обновляем описание инструмента
    ToolDescription updatedDescription;
    updatedDescription.setId(updatedTool.getToolDescriptionId());
    updatedDescription.setDescription(ui.lineEdit_description->text());
    updatedDescription.setLengthMm(ui.spinBox_length->value());
    updatedDescription.setOuterDiameterMm(ui.spinBox_outerDiameter->value());
    updatedDescription.setInnerDiameterMm(ui.spinBox_innerDiameter->value());

    int producerId = ui.comboBox_producer->currentData().toInt();
    updatedDescription.setProducerId(producerId);

    // Обновляем запись в хранилище
    StorageEditor editor(Storage::getInstance());
    editor.insertOrReplace(Storage::getInstance()->getToolDescriptions(), updatedDescription);

    return updatedTool;
}

void ToolEditor::onSaveButtonClicked()
{
    // Сохранение данных и закрытие окна
    accept();
}
