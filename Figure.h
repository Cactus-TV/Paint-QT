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
//выбранная фигура
extern Figure* SELECTED_FIGURE;
//вектор всех фигур
extern std::vector<Figure*> figures;

struct Point
{
    int pointType;//тип вершины
    //1 - вогнутый квадрат
    //2 - вогнутый равнобедренный прямоугольник
    //3 - вогнутая четверть круга
    //4 - закруглённый край
    //5 - вогнутый прямоугольник
    //6 - вогнутый полукруг
    int pointPosition;//положение точки на прямоугольнике
    //1 - сверху слева
    //2 - снизу слева
    //3 - сверху справа
    //4 - снизу справа
    //5 - сверху
    //6 - снизу
    int value;//размер
    QPoint pos;//координаты точки
};

class Figure: public QWidget//класс фигуры
{
    Q_OBJECT
public:
    //Конструктор
    explicit Figure(QWidget *parent = nullptr, int figure_type = 0);
    //Снятие выделения с данной фигуры
    void Deselect();
    //Получение координат точек
    std::vector<QPoint> PointsPositions() const;
    //Получение размеров
    std::pair<int,int> GetWidthHeight() const;
    //Установка угла поворота
    void SetRotation(int rotation);
    //Установка положения
    void SetPosition(QPoint pos);
    //Установка размеров
    void SetWidthHeight(std::pair<int,int> wh);

private slots:
    //Удаление, изменение, перемещение, поворот фигуры
    void InfoAboutFigure(QMouseEvent *event);
    //Удаление фигуры
    void FigureDeleteClicked();
    //Изменение параметров фигуры
    void FigureChangeClicked();

    //Дополнительные слоты:
    // поворот направо
    void FigureRotationRight();
    // поворот налево
    void FigureRotationLeft();
    // сохранение изменений
    void FigureChangeSave();
    // перемещение
    void FigureMove();
    // поворот
    void FigureRotate();
    // изменение угла поворота через слайдер
    void SliderMove(int val);

private://методы
    //Обработка события перемещения мыши внутри виджета (ЛКМ) и перемещения фигуры вслед за курсором
    void mouseMoveEvent(QMouseEvent *event);
    //Обработка события нажатия мыши на виджет (ЛКМ или ПКМ)
    void mousePressEvent(QMouseEvent *event);
    //Проверка, что точка в фигуре
    bool IsPointOfFigure(const QPoint& pos) const;
    //Проверка, что фигуры не пересекаются
    bool NoIntersectsBetweenFigures(const QPoint& pos) const;
    //Проверка, что фигура в пределах окна
    bool IsFigureInTheScreen(const QPoint& pos) const;
    //Обработка события отрисовки виджета
    void paintEvent(QPaintEvent *event);
    //Универсальная для всех типов точек отрисовка линии данной точки
    void PaintPoint(QPainter& painter, const Point& point, const int posW, const int posH) const;
    //Обновление периметра фигуры
    void NewPerimeter();
    //Обновление площади фигуры
    void NewArea();

private://поля
    //Поля для изменения параметров фигуры, кнопок
    //для поворота против часовой
    QRadioButton* new_rotation_left;
    //для поворота по часовой
    QRadioButton* new_rotation_right;
    //для ввода новой ширины фигуры
    QLineEdit* new_width;
    //для ввода новой высоты фигуры
    QLineEdit* new_height;
    //для ввода нового угла поворота фигуры
    QLineEdit* new_rotation;
    //для ввода новых координат фигуры
    QLineEdit* new_x;//X
    QLineEdit* new_y;//Y
    //для вывода периметра фигуры
    QLabel* new_perimeter;
    //для вывода площади фигуры
    QLabel* new_area;
    //для вывода текущего угла поворота фигуры
    QLabel* slider_label;

    //основные поля - параметры фигуры
    bool rotated;//если true, то против часовой, если false - по часовой
    int rotation;//угол поворота в градусах
    int figure_type;//тип: 12 или 22
    std::pair<int, int> mouse_x_y;//координаты последнего клика мыши по фигуре
    std::vector<Point> points;//точки вершин
    std::pair<int, int> rectangle_w_h;//ширина и высота
    double perimeter;//периметр
    double area;//площадь
    //bool selected;//выбрана ли фигура
    bool resized;//изменён ли размер фигуры
    bool button_is_selected;//включена ли кнопка перемещения фигуры
};

#endif // FIGURE_H
