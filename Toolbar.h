#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QMainWindow>
#include <QToolBar>
#include <QPushButton>
#include <QToolButton>
#include <Figure.h>
#include <QComboBox>
#include <vector>
#include <set>
#include <QDebug>
#include <QMessageBox>
#include <QMenu>

class Toolbar : public QToolBar
{
    Q_OBJECT
public:
    explicit Toolbar(QMainWindow *parent = nullptr);

    void DisableDeleteButton (bool is_disabled)
    {
        button_delete->setDisabled(is_disabled);
    }

private:
    bool chosen_figure12;//тип фигуры 12
    bool chosen_figure22;//тип фигуры 22
    QPushButton* radio_button_figure12;//кнопка выбора типа фигуры 12
    QPushButton* radio_button_figure22;//кнопка выбора типа фигуры 22
    QPushButton* button_add;//кнопка добавления
    QPushButton* button_delete;//кнопка выбора типа фигуры 12
    QPushButton* button_fit;//кнопка выбора типа фигуры 12
    QComboBox* combobox_delete;

private slots:
    void FigureAddTriggered();//добавление фигуры
    void FigureDeleteTriggered();//удаление фигуры
    void Figure12Toggled();//выбор типа фигуры 12
    void Figure22Toggled();//выбор типа фигуры 22
    void FiguresFit();//уместить
    void MenuTriggered(QAction* act);//выбор типа удаления: все или пересекающиеся
};

#endif // TOOLBAR_H
