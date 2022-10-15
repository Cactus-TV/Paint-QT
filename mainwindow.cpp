#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    // Фон основной части окна - белый
    this->setStyleSheet("background-color:white;");
    // Для генерации случайных чисел
    std::srand(time(NULL));
    // Создание двух фигур по умолчанию
    /*Figure *baseFigure1 = new Figure(this, 12);
    Figure *baseFigure2 = new Figure(this, 22);
    baseFigure1->show();
    baseFigure2->show();*/
    // Создание ToolBar
    toolBar = new Toolbar(this);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->spontaneous() && SELECTED_FIGURE != nullptr)
    {
        SELECTED_FIGURE->Deselect();
    }

    if (SELECTED_FIGURE == nullptr)
    {
        toolBar->DisableDeleteButton(true);
    }
    else
    {
        toolBar->DisableDeleteButton(false);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    toolBar->resize(this->width(), 30);
    toolBar->repaint();
}



