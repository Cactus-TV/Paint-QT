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
    bool chosen_figure12;
    bool chosen_figure22;
    QPushButton* radio_button_figure12;
    QPushButton* radio_button_figure22;
    QPushButton* button_add;
    QPushButton* button_delete;
    QPushButton* button_fit;
    QComboBox* combobox_delete;

private slots:
    void FigureAddTriggered();
    void FigureDeleteTriggered();
    void Figure12Toggled();
    void Figure22Toggled();
    void FiguresFit();
    void MenuTriggered(QAction* act);
};

#endif // TOOLBAR_H
