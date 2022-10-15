#ifndef FIGURE_H
#define FIGURE_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QDebug>
#include <QRadioButton>
#include <QBoxLayout>
#include <QMainWindow>
#include <cmath>
#include <stdexcept>
#include <random>
#include <vector>
#include <cmath>
#include <iostream>
#include <QTextStream>
#include <QLineEdit>
#include <QSlider>
#include <QMenu>
#include <QCursor>
#include <QMessageBox>
#include <QFuture>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrentRun>

class Figure;
//Выбранная фигура
extern Figure* SELECTED_FIGURE;
extern std::vector<Figure*> figures;

struct Point
{
    int pointType;
    //1 - вогнутый квадрат
    //2 - вогнутый равнобедренный прямоугольник
    //3 - вогнутая четверть круга
    //4 - закруглённый край
    //5 - вогнутый прямоугольник
    //6 - вогнутый полукруг
    int pointPosition;
    //1 - сверху слева
    //2 - снизу слева
    //3 - сверху справа
    //4 - снизу справа
    //5 - сверху
    //6 - снизу
    int value;//размер
    QPoint pos;//координаты точки
};

class Figure: public QWidget
{
    Q_OBJECT
public:
    explicit Figure(QWidget *parent = nullptr, int figure_type = 0);

    //снятие выделения с данной фигуры
    void Deselect();

private slots:
    // Вывод информации о фигуре, инициализация создания модального
    // диалогового окна для ее отображения и изменения, обработка
    // и валидация полученных изменений, их сохранение.
    void InfoAboutFigure(QMouseEvent *event);

    //удаление фигуры
    void FigureDeleteClicked();

    //изменение параметров фигуры
    void FigureChangeClicked();

    //Дополнительные слоты:
    //поворот направо
    void FigureRotationRight();
    //поворот налево
    void FigureRotationLeft();
    //сохранение изменений
    void FigureChangeSave();
    //перемещение
    void FigureMove();
    //поворот
    void FigureRotate();
    //изменение угла поворота
    void SliderMove(int val);

public:
    std::vector<QPoint> PointsPositions() const;
    std::pair<int,int> GetWidthHeight() const;
    void SetRotation(int rotation);
    void SetPosition(QPoint pos);
    void SetWidthHeight(std::pair<int,int> wh);

private:
    // Обработка события перемещения мыши внутри виджета (ЛКМ)
    // и перемещения фигуры (виджета) вслед за ней
    void mouseMoveEvent(QMouseEvent *event);

    // Обработка события нажатия мыши на виджет (ЛКМ или ПКМ)
    void mousePressEvent(QMouseEvent *event);

    //проверка, что точка в фигуре
    bool IsPointOfFigure(const QPoint& pos) const;

    //проверка, что фигуры не пересекаются
    bool NoIntersectsBetweenFigures(const QPoint& pos) const;

    //проверка, что фигура в пределах окна
    bool IsFigureInTheScreen(const QPoint& pos) const;

    // Обработка события отрисовки виджета (универсальная для всех
    // наследников класса и всех видов фигур любых размеров)
    void paintEvent(QPaintEvent *event);

    // Универсальная для всех типов точек отрисовка линии данной точки
    void PaintPoint(QPainter& painter, const Point& point, const int posW, const int posH) const;

    // Обновление периметра фигуры
    void NewPerimeter();

    // Обновление площади фигуры
    void NewArea();

    QRadioButton* new_rotation_left;
    QRadioButton* new_rotation_right;
    QLineEdit* new_width;
    QLineEdit* new_height;
    QLineEdit* new_rotation;
    QLineEdit* new_x;
    QLineEdit* new_y;
    QLabel* new_perimeter;
    QLabel* new_area;
    QLabel* slider_label;
    bool rotated;
    int rotation;
    int figure_type;
    std::pair<int, int> mouse_x_y;
    std::vector<Point> points;
    std::pair<int, int> rectangle_w_h;
    double perimeter;
    double area;
    bool selected;
    bool resized;
    bool button_is_selected;
};

#endif // FIGURE_H
