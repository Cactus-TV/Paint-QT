#include <Figure.h>

Figure::Figure(QWidget *parent, int figure_type) : QWidget(parent)//конструктор
{
    this->figure_type = figure_type;
    button_is_selected = false;
    rotated = true;
    new_rotation_left = new QRadioButton("Против часовой");
    new_rotation_right = new QRadioButton("По часовой");
    new_rotation_left->setChecked(true);
    new_rotation_right->setChecked(false);
    connect(new_rotation_left, SIGNAL(clicked()), this, SLOT(FigureRotationLeft()));
    connect(new_rotation_right, SIGNAL(clicked()), this, SLOT(FigureRotationRight()));
    do
    {
        rotation = rand() % 360;
        points.clear();
        int width = rand() % parentWidget()->width()/4 + 100;
        int height = (rand() % parentWidget()->height()/4 + 20) % width;
        if (height < 60)
        {
            height = 60;
        }
        mouse_x_y = std::make_pair(rand() % (parentWidget()->width() - width), rand() % (parentWidget()->height() - height));
        this->move(mouse_x_y.first, mouse_x_y.second);
        rectangle_w_h = std::make_pair(width, height);
        if (figure_type == 12)
        {
            //A
            points.push_back({2, 3, rand() % (rectangle_w_h.second / 3), {0,0}});
            //B
            points.push_back({3, 4, rand() % (rectangle_w_h.second / 3), {0,0}});
            //C
            points.push_back({2, 2, rand() % (rectangle_w_h.second / 3), {0,0}});
            //D
            points.push_back({3, 1, rand() % (rectangle_w_h.second / 3), {0,0}});
            //E
            points.push_back({0, 5, (rand() % (rectangle_w_h.first / 4)) % rectangle_w_h.second, {0,0}});
            //F
            points.push_back({6, 6, (rand() % (rectangle_w_h.first / 4)) % rectangle_w_h.second, {0,0}});
        }
        else if (figure_type == 22)
        {
            //A
            points.push_back({3, 3, rand() % (rectangle_w_h.second / 3), {0,0}});
            //B
            points.push_back({4, 4, rand() % (rectangle_w_h.second / 3), {0,0}});
            //C
            points.push_back({3, 2, rand() % (rectangle_w_h.second / 3), {0,0}});
            //D
            points.push_back({2, 1, rand() % (rectangle_w_h.second / 3), {0,0}});
            //E
            points.push_back({0, 5, (rand() % (rectangle_w_h.first / 4)) % rectangle_w_h.second, {0,0}});
            //F
            points.push_back({6, 6, (rand() % (rectangle_w_h.first / 4)) % rectangle_w_h.second, {0,0}});
        }
        int new_widgetWidth = abs(cos(qDegreesToRadians(rotation)) * width) + abs(cos(qDegreesToRadians(90 - rotation)) * height);
        int new_widgetHeight = abs(sin(qDegreesToRadians(rotation)) * width) + abs(sin(qDegreesToRadians(90 - rotation)) * height);
        this->repaint();
        resized = false;
        //смещение виджета для устранения смещения центра фигуры
        this->move(this->x() + (this->width() - new_widgetWidth - 2) / 4, this->y() + (this->height() - new_widgetHeight - 4) / 2);
        //запас в 4 пикселя для устранения погрешностей вычислений
        this->resize(new_widgetWidth + 4, new_widgetHeight + 4);
        QPainterPath painter_path;
        double radius = sqrt(width*width + height*height) / 2;
        int ex_angle = qRadiansToDegrees(acos((2*radius*radius - height*height) / (2*radius*radius)));
        painter_path.translate(this->x(), this->y());
        painter_path.moveTo(0,0);
        int dir_rotation = rotation;
        if (!rotated)
        {
            dir_rotation *= (-1);
        }
        QPoint ficha(this->width()/2 - radius, this->height()/2 - radius);
        painter_path.arcMoveTo(ficha.x(), ficha.y(), radius*2, radius*2, dir_rotation + ex_angle/2);//точка A
        points[0].pos = {int(painter_path.currentPosition().x()), int(painter_path.currentPosition().y())};
        painter_path.arcMoveTo(ficha.x(), ficha.y(), radius*2, radius*2, dir_rotation - ex_angle/2);//точка B
        points[1].pos = {int(painter_path.currentPosition().x()), int(painter_path.currentPosition().y())};
        painter_path.arcMoveTo(ficha.x(), ficha.y(), radius*2, radius*2, 180 + dir_rotation + ex_angle/2);//точка C
        points[2].pos = {int(painter_path.currentPosition().x()), int(painter_path.currentPosition().y())};
        painter_path.arcMoveTo(ficha.x(), ficha.y(), radius*2, radius*2, 180 + dir_rotation - ex_angle/2);//точка D
        points[3].pos = {int(painter_path.currentPosition().x()), int(painter_path.currentPosition().y())};
    } while(!(IsFigureInTheScreen(this->pos()) && NoIntersectsBetweenFigures({0,0})));
    NewPerimeter();
    NewArea();
    new_perimeter = new QLabel("Периметр фигуры: " + QString::number(perimeter));
    new_area = new QLabel("Площадь фигуры: " + QString::number(area));
    slider_label = new QLabel(QString::number(rotation));
    slider_label->setAlignment(Qt::AlignCenter);
}

std::pair<int,int> Figure::GetWidthHeight() const//получить размеры фигуры
{
    return rectangle_w_h;
}

void Figure::SetWidthHeight(std::pair<int,int> wh)//установка новых размеров фигуры
{
    for(auto& i: points)
    {
        i.value /= (rectangle_w_h.second * rectangle_w_h.first) / (wh.second * wh.first);
    }
    rectangle_w_h = wh;
    resized = false;
    this->repaint();
}

void Figure::SetRotation(int rotation)//установка нового поворота фигуры
{
    this->rotation = rotation;
    resized = false;
    this->repaint();
}

void Figure::SetPosition(QPoint pos)//установка нового положения фигуры
{
    this->move(pos);
    this->repaint();
}

std::vector<QPoint> Figure::PointsPositions() const//возврат координат вершин
{
    std::vector<QPoint> temp;
    for(auto i: points)
    {
        temp.push_back(i.pos + this->pos());
    }
    return temp;
}

bool Figure::IsPointOfFigure(const QPoint& pos) const//проверка, что курсор внутри фигуры
{
    QPainterPath painter_path;
    QPolygon around_rec;
    for(size_t i = 0; i < 4; ++i)
    {
        around_rec.append(points[i].pos);
    }
    painter_path.addPolygon(around_rec);
    QRect around_mouse(pos, QSize(1,1));
    return painter_path.intersects(around_mouse) || painter_path.contains(around_mouse);
}

bool Figure::IsFigureInTheScreen(const QPoint& pos) const//проверка, что фигура в пределах окна приложения
{
    QPainterPath painter_path;
    QPainterPath screen_path;
    QPolygon around_rec;
    for(size_t i = 0; i < 4; ++i)
    {
        around_rec.append(points[i].pos + pos);
    }
    painter_path.addPolygon(around_rec);
    QRect around_screen({1,20}, QSize(parentWidget()->width() - 1, parentWidget()->height() - 10));
    screen_path.addRect(around_screen);
    return screen_path.contains(painter_path);
}

bool Figure::NoIntersectsBetweenFigures(const QPoint& pos) const//проверка, что нету пересечений между фигурами
{
    auto Points1 = PointsPositions();
    for (auto& i: Points1)
    {
        i += pos;
    }
    for(size_t j = 0; j < figures.size(); ++j)
    {
        if (figures[j] == this)
        {
            continue;
        }
        auto Points2 = figures[j]->PointsPositions();
        QPainterPath painter_path1;
        QPainterPath painter_path2;
        QPolygon around_rec1;
        QPolygon around_rec2;
        for(size_t c = 0; c < 4; ++c)
        {
            around_rec1.append(Points1[c]);
        }
        painter_path1.addPolygon(around_rec1);
        for(size_t c = 0; c < 4; ++c)
        {
            around_rec2.append(Points2[c]);
        }
        painter_path2.addPolygon(around_rec2);
        if(painter_path1.intersects(painter_path2))
        {
            return false;
        }
    }
    return true;
}

void Figure::Deselect()//удаление выбора данной фигуры
{
    SELECTED_FIGURE = nullptr;
    this->repaint();
}

void Figure::mousePressEvent(QMouseEvent *event)//нажатие кнопок мыши
{
    if (SELECTED_FIGURE != nullptr)
    {
        SELECTED_FIGURE->Deselect();
    }
    switch(event->button())
    {
        case Qt::LeftButton:
            if (IsPointOfFigure(event->pos()))
            {
                mouse_x_y = std::make_pair(event->globalPosition().x() - this->x(), event->globalPosition().y() - this->y());
                SELECTED_FIGURE = this;
                this->repaint();
            }
            break;
        case Qt::RightButton:
            if (IsPointOfFigure(event->pos()))
            {
                mouse_x_y = std::make_pair(event->globalPosition().x() - this->x(), event->globalPosition().y() - this->y());
                this->repaint();
                InfoAboutFigure(event);
            }
            break;
        default:
            QWidget::mousePressEvent(event);
            break;
    }

    QWidget::mousePressEvent(event);
}

void Figure::mouseMoveEvent(QMouseEvent *event)
{
    if (SELECTED_FIGURE == this)
    {
        QPoint temp = QPoint(event->globalPosition().x() - mouse_x_y.first - this->pos().x(), event->globalPosition().y() - mouse_x_y.second - this->pos().y());
        //проверка на выход за пределы экрана
        if (!IsFigureInTheScreen(temp + this->pos()) || !NoIntersectsBetweenFigures(temp))
        {
            return;
        }
        if (IsPointOfFigure(event->pos()))
        {
            this->move(event->globalPosition().x() - mouse_x_y.first,
                       event->globalPosition().y() - mouse_x_y.second);
            this->repaint();
        }
    }
}

void Figure::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //ширина и высота ограничивающего прямоугольника
    int widgetWidth = rectangle_w_h.first;
    int widgetHeight = rectangle_w_h.second;
    //изменение размеров виджета при повороте/изменении размеров фигуры
    if (!resized)
    {
        NewPerimeter();
        NewArea();
        int new_widgetWidth = abs(cos(qDegreesToRadians(rotation)) * widgetWidth) + abs(cos(qDegreesToRadians(90 - rotation)) * widgetHeight);
        int new_widgetHeight = abs(sin(qDegreesToRadians(rotation)) * widgetWidth) + abs(sin(qDegreesToRadians(90 - rotation)) * widgetHeight);
        resized = true;
        //смещение виджета для устранения смещения центра фигуры
        this->move(this->x() + (this->width() - new_widgetWidth - 4) / 2, this->y() + (this->height() - new_widgetHeight - 4) / 2);

        //запас в 1 пиксель для устранения погрешностей вычислений
        this->resize(new_widgetWidth + 4, new_widgetHeight + 4);
    }

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 2));
    QPainterPath painter_path;
    double radius = sqrt(widgetWidth*widgetWidth + widgetHeight*widgetHeight) / 2;
    int ex_angle = qRadiansToDegrees(acos((2*radius*radius - widgetHeight*widgetHeight) / (2*radius*radius)));
    painter_path.translate(this->x(), this->y());
    //painter_path.moveTo(0,0);
    int dir_rotation = rotation;
    if (!rotated)//false - против часовой, true - по часовой
    {
        dir_rotation *= (-1);
    }
    QPoint ficha(this->width()/2 - int(radius), this->height()/2 - int(radius));
    painter_path.arcMoveTo(ficha.x(), ficha.y(), radius*2, radius*2, dir_rotation + ex_angle/2);//точка A
    points[0].pos = {int(painter_path.currentPosition().x()), int(painter_path.currentPosition().y())};
    painter_path.arcMoveTo(ficha.x(), ficha.y(), radius*2, radius*2, dir_rotation - ex_angle/2);//точка B
    points[1].pos = {int(painter_path.currentPosition().x()), int(painter_path.currentPosition().y())};
    painter_path.arcMoveTo(ficha.x(), ficha.y(), radius*2, radius*2, 180 + dir_rotation + ex_angle/2);//точка C
    points[2].pos = {int(painter_path.currentPosition().x()), int(painter_path.currentPosition().y())};
    painter_path.arcMoveTo(ficha.x(), ficha.y(), radius*2, radius*2, 180 + dir_rotation - ex_angle/2);//точка D
    points[3].pos = {int(painter_path.currentPosition().x()), int(painter_path.currentPosition().y())};

    /*painter.setPen(QPen(Qt::red, 2));//для отрисовки границ отслеживания области отслеживания нажатия курсора
    painter.drawLine(points[0].pos, points[1].pos);
    painter.drawLine(points[1].pos, points[2].pos);
    painter.drawLine(points[2].pos, points[3].pos);
    painter.drawLine(points[3].pos, points[0].pos);*/

    //смещение осей координат
    painter.translate(this->width()/2, this->height()/2);
    //поворот осей координат (по умолчанию против часовой стрелки)
    if (rotated)
    {
        painter.rotate(-rotation);
    }
    else
    {
        painter.rotate(+rotation);
    }
    //если фигура выделена, то отрисовывается синим
    if (this == SELECTED_FIGURE)
    {
        painter.setPen(QPen(Qt::blue, 3));
    }

    //отрисовка прямых, соединяющих точки A,B,C,D,E,F между собой
    // верхняя линия
    painter.drawLine(points[3].value - widgetWidth / 2, (-1)*widgetHeight / 2, (-1)*points[4].value / 2, (-1)*widgetHeight / 2);
    painter.drawLine(points[4].value / 2, (-1)*widgetHeight / 2, widgetWidth / 2 - points[0].value, (-1)*widgetHeight / 2);
    // правая линия
    painter.drawLine(widgetWidth / 2, points[0].value - widgetHeight / 2, widgetWidth / 2, widgetHeight / 2 - points[1].value);
    // нижняя линия
    painter.drawLine(points[2].value - widgetWidth / 2, widgetHeight / 2, (-1)*points[5].value / 2, widgetHeight / 2);
    painter.drawLine(points[5].value / 2, widgetHeight / 2, widgetWidth / 2 - points[1].value, widgetHeight / 2);
    // левая линия
    painter.drawLine((-1)*widgetWidth / 2, points[3].value - widgetHeight / 2, (-1)*widgetWidth / 2, widgetHeight / 2 - points[2].value);

    //универсальная отрисовка всех точек
    //A
    PaintPoint(painter, points[0], widgetWidth / 2 - points[0].value, (-1)*widgetHeight / 2 + points[0].value);
    //B
    PaintPoint(painter, points[1], widgetWidth / 2 - points[1].value, widgetHeight / 2 - points[1].value);
    //C
    PaintPoint(painter, points[2], (-1)*widgetWidth / 2 + points[2].value, widgetHeight / 2 - points[2].value);
    //D
    PaintPoint(painter, points[3], (-1)*widgetWidth / 2 + points[3].value, (-1)*widgetHeight / 2 + points[3].value);
    //E
    PaintPoint(painter, points[4], 0, (-1)*widgetHeight / 2);
    //F
    PaintPoint(painter, points[5], 0, widgetHeight / 2);
}

void Figure::PaintPoint(QPainter& painter, const Point& point, const int posW, const int posH) const
{
    switch(point.pointType)
    {
        case 0:
            switch(point.pointPosition)
            {
                case 1:
                    //сверху слева обычный угол
                    painter.drawLine(posW - point.value, posH, posW - point.value, posH - point.value);
                    painter.drawLine(posW - point.value, posH - point.value, posW, posH - point.value);
                    break;
                case 2:
                    //снизу слева обычный угол
                    painter.drawLine(posW - point.value, posH, posW - point.value, posH + point.value);
                    painter.drawLine(posW - point.value, posH + point.value, posW, posH + point.value);
                    break;
                case 3:
                    //сверху справа обычный угол
                    painter.drawLine(posW + point.value, posH, posW + point.value, posH - point.value);
                    painter.drawLine(posW + point.value, posH - point.value, posW, posH - point.value);
                    break;
                case 4:
                    //снизу справа обычный угол
                    painter.drawLine(posW + point.value, posH, posW + point.value, posH + point.value);
                    painter.drawLine(posW + point.value, posH + point.value, posW, posH + point.value);
                    break;
                case 5:
                    //сверху обычная линия
                    painter.drawLine(posW - point.value, posH, posW + point.value, posH);
                    break;
                case 6:
                    //снизу обычная линия
                    painter.drawLine(posW - point.value, posH, posW + point.value, posH);
                    break;
                default:
                    break;
            }
            break;
        case 1:
            switch(point.pointPosition)
            {
                case 1:
                    //сверху слева вогнутый квадрат
                    painter.drawLine(posW, posH, posW - point.value, posH);
                    painter.drawLine(posW, posH, posW, posH - point.value);
                    break;
                case 2:
                    //снизу слева вогнутый квадрат
                    painter.drawLine(posW, posH, posW - point.value, posH);
                    painter.drawLine(posW, posH, posW, posH + point.value);
                    break;
                case 3:
                    //сверху справа вогнутый квадрат
                    painter.drawLine(posW, posH, posW + point.value, posH);
                    painter.drawLine(posW, posH, posW, posH - point.value);
                    break;
                case 4:
                    //снизу справа вогнутый квадрат
                    painter.drawLine(posW, posH, posW + point.value, posH);
                    painter.drawLine(posW, posH, posW, posH + point.value);
                    break;
                default:
                    break;
            }
            break;
        case 2:
            switch(point.pointPosition)
            {
                case 1:
                    //сверху слева вогнутый треугольник
                    painter.drawLine(posW - point.value, posH, posW, posH - point.value);
                    break;
                case 2:
                    //снизу слева вогнутый треугольник
                    painter.drawLine(posW - point.value, posH, posW, posH + point.value);
                    break;
                case 3:
                    //сверху справа вогнутый треугольник
                    painter.drawLine(posW, posH - point.value, posW + point.value, posH);
                    break;
                case 4:
                    //снизу справа вогнутый треугольник
                    painter.drawLine(posW, posH + point.value, posW + point.value, posH);
                    break;
                default:
                    break;
            }
            break;
        case 3:
            switch(point.pointPosition)
            {
                case 1:
                    //сверху слева вогнутая четверть круга
                    painter.drawArc(posW - point.value*2, posH - point.value*2, point.value*2, point.value*2, 270*16, 90*16);
                    break;
                case 2:
                    //снизу слева вогнутая четверть круга
                    painter.drawArc(posW - point.value*2, posH, point.value*2, point.value*2, 0, 90*16);
                    break;
                case 3:
                    //сверху справа вогнутая четверть круга
                    painter.drawArc(posW, posH - point.value*2, point.value*2, point.value*2, 180*16, 90*16);
                    break;
                case 4:
                    //снизу справа вогнутая четверть круга
                    painter.drawArc(posW, posH, point.value*2, point.value*2, 90*16, 90*16);
                    break;
                default:
                    break;
            }
            break;
        case 4:
            switch(point.pointPosition)
            {
                case 1:
                    //сверху слева выпуклая четверть круга
                    painter.drawArc(posW - point.value, posH - point.value, point.value*2, point.value*2, 90*16, 90*16);
                    break;
                case 2:
                    //снизу слева выпуклая четверть круга
                    painter.drawArc(posW - point.value, posH - point.value, point.value*2, point.value*2, 180*16, 90*16);
                    break;
                case 3:
                    //сверху справа выпуклая четверть круга
                    painter.drawArc(posW - point.value, posH - point.value, point.value*2, point.value*2, 0, 90*16);
                    break;
                case 4:
                    //снизу справа выпуклая четверть круга
                    painter.drawArc(posW - point.value, posH - point.value, point.value*2, point.value*2, 270*16, 90*16);
                    break;
                default:
                    break;
            }
            break;
        case 5:
            switch(point.pointPosition)
            {
                case 5:
                    //сверху вогнутый прямоугольник
                    painter.drawLine(posW - point.value, posH, posW - point.value, posH + point.value / 2);
                    painter.drawLine(posW - point.value, posH + point.value / 2, posW - point.value, posH + point.value / 2);
                    painter.drawLine(posW + point.value, posH, posW + point.value, posH + point.value / 2);
                    break;
                case 6:
                    //снизу вогнутый прямоугольник
                    painter.drawLine(posW - point.value, posH, posW - point.value, posH - point.value / 2);
                    painter.drawLine(posW - point.value, posH - point.value / 2, posW - point.value, posH + point.value / 2);
                    painter.drawLine(posW + point.value, posH, posW + point.value, posH - point.value / 2);
                    break;
                default:
                    break;
            }
            break;
        case 6:
            switch(point.pointPosition)
            {
                case 5:
                    //сверху вогнутый полукруг
                    painter.drawArc(posW - point.value/2, posH - point.value/2, point.value, point.value, 180*16, 180*16);
                    break;
                case 6:
                    //снизу вогнутый полукруг
                    painter.drawArc(posW - point.value/2, posH - point.value/2, point.value, point.value, 0, 180*16);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void Figure::FigureDeleteClicked()//удаление фигуры
{
    figures.erase(std::remove(figures.begin(), figures.end(), this), figures.end());
    this->deleteLater();
    SELECTED_FIGURE = nullptr;
}

void Figure::FigureRotationRight()//поворот фигуры направо
{
    if(!new_rotation_left->isChecked() && !new_rotation_right->isChecked())
    {
        new_rotation_right->setChecked(true);
    }
    else if(new_rotation_right->isChecked())
    {
        new_rotation_right->setChecked(false);
    }
    else
    {
        new_rotation_right->setChecked(true);
        new_rotation_left->setChecked(false);
    }
}

void Figure::FigureRotationLeft()//поворот фигуры налево
{
    if(!new_rotation_left->isChecked() && !new_rotation_right->isChecked())
    {
        new_rotation_left->setChecked(true);
    }
    else if(new_rotation_left->isChecked())
    {
        new_rotation_left->setChecked(false);
    }
    else
    {
        new_rotation_right->setChecked(false);
        new_rotation_left->setChecked(true);
    }
}

void Figure::FigureChangeSave()//сохранение изменений параметров фигуры
{
    try
    {
        int temp = 0;
        if (new_width->text() != "")
        {
            temp = new_width->text().toInt();
            if(temp >= 0 && temp >= 100)
            {
                rectangle_w_h.first = temp;
            }
        }
        if (new_height->text() != "")
        {
            temp = new_height->text().toInt();
            if(temp >= 60 && temp < rectangle_w_h.first)
            {
                rectangle_w_h.second = temp;
            }
        }
        if (new_rotation->text() != "")
        {
            temp = new_rotation->text().toInt();
            if(temp >= 0 && temp < 360)
            {
                rotation = temp;
            }
        }
        if (new_rotation_right->isChecked())
        {
            rotated = false;
        }
        else
        {
            rotated = true;
        }
        if (new_x->text() != "")
        {
            temp = new_x->text().toInt();
            if (temp + this->width() <= this->parentWidget()->width() && temp >= 0)
            {
                this->move(temp, this->y());
            }
        }
        if (new_y->text() != "")
        {
            temp = new_y->text().toInt();
            if (temp + this->height() <= this->parentWidget()->height() && temp >= 0)
            {
                this->move(this->x(), temp + 30);
            }
        }
    }
    catch(...)
    {
        QMessageBox::warning(this, "Ошибка!", "Введён неверный параметр!");
    }
    resized = false;
    this->repaint();
    NewArea();
    NewPerimeter();
    new_perimeter->setText("Периметр фигуры: " + QString::number(perimeter));
    new_area->setText("Площадь фигуры: " + QString::number(area));
}

void Figure::FigureChangeClicked()//кликнута кнопка изменения параметров фигуры
{
    QDialog* modalDialog = new QDialog(this);
    modalDialog->setModal(true);

    QBoxLayout* boxLayout = new QBoxLayout(QBoxLayout::Down);
    boxLayout->addWidget(new_perimeter);
    boxLayout->addWidget(new_area);

    QBoxLayout* widthLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    widthLayout->addWidget(new QLabel("Ширина:"));
    new_width = new QLineEdit();
    widthLayout->addWidget(new_width);
    boxLayout->addLayout(widthLayout);

    QBoxLayout* heightLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    heightLayout->addWidget(new QLabel("Высота:"));
    new_height = new QLineEdit();
    heightLayout->addWidget(new_height);
    boxLayout->addLayout(heightLayout);

    QBoxLayout* XLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    XLayout->addWidget(new QLabel("Координата X:"));
    new_x = new QLineEdit();
    XLayout->addWidget(new_x);
    boxLayout->addLayout(XLayout);

    QBoxLayout* YLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    YLayout->addWidget(new QLabel("Координата Y:"));
    new_y = new QLineEdit();
    YLayout->addWidget(new_y);
    boxLayout->addLayout(YLayout);

    QBoxLayout* angleLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    angleLayout->addWidget(new QLabel("Угол поворота:"));
    new_rotation = new QLineEdit();
    angleLayout->addWidget(new_rotation);
    boxLayout->addLayout(angleLayout);

    QBoxLayout* rotationLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    rotationLayout->addWidget(new QLabel("Направление поворота:"));
    rotationLayout->addWidget(new_rotation_left);
    rotationLayout->addWidget(new_rotation_right);
    boxLayout->addLayout(rotationLayout);

    QPushButton* save_button = new QPushButton("Сохранить");
    connect(save_button, SIGNAL(clicked()), this, SLOT(FigureChangeSave()));
    boxLayout->addWidget(save_button);

    modalDialog->setLayout(boxLayout);
    modalDialog->exec();
}

void Figure::NewPerimeter()//рассчёт нового периметра фигуры
{
    perimeter = 2 * rectangle_w_h.first + 2 * rectangle_w_h.second;
    for (const auto& i: points)
    {
        switch(i.pointType)
        {
            case 2:
                perimeter -= i.value * 2;
                perimeter += sqrt(2*i.value*i.value);
                break;
            case 3:
                perimeter -= i.value * 2;
                perimeter += M_PI * i.value / 2;
                break;
            case 4:
                perimeter -= i.value * 2;
                perimeter += M_PI * i.value / 2;
                break;
            case 5:
                perimeter += i.value;
                break;
            case 6:
                perimeter -= i.value;
                perimeter += M_PI * i.value;
                break;
            default:
                break;
        }
    }
}

void Figure::NewArea()//рассчёт новой площади фигуры
{
    area = rectangle_w_h.first * rectangle_w_h.second;
    for (const auto& i: points)
    {
        switch(i.pointType)
        {
            case 1:
                area -= i.value * i.value;
                break;
            case 2:
                area -= i.value * i.value / 2;
                break;
            case 3:
                area -= M_PI * i.value * i.value / 4;
                break;
            case 4:
                area -= i.value * i.value;;
                area += M_PI * i.value * i.value / 4;
                break;
            case 5:
                area -= i.value / 2 * i.value;
                break;
            case 6:
                area -= M_PI * i.value * i.value / 2;
                break;
            default:
                break;
        }
    }
}

void Figure::FigureMove()//кнопка перемещения фигуры
{
    SELECTED_FIGURE = this;
    if(!button_is_selected)
    {
        this->move(mapToGlobal(QCursor::pos()).x() - mapToGlobal(this->parentWidget()->pos()).x() - this->width()/2, mapToGlobal(QCursor::pos()).y() - mapToGlobal(this->parentWidget()->pos()).y()- this->height()/2);
    }
    button_is_selected = !button_is_selected;
    this->setMouseTracking(button_is_selected);
    this->repaint();
}

void Figure::SliderMove(int val)//изменение положения ползунка поворота
{
    if (val < 0)
    {
        rotated = false;
        new_rotation_left->setChecked(false);
        new_rotation_right->setChecked(true);
    }
    else
    {
        rotated = true;
        new_rotation_left->setChecked(true);
        new_rotation_right->setChecked(false);
    }
    slider_label->setText(QString::number(val));
    rotation = abs(val);
    resized = false;
    this->repaint();
}

void Figure::FigureRotate()//
{
    QDialog* modalDialog = new QDialog(this);

    QBoxLayout* boxLayout = new QBoxLayout(QBoxLayout::Down);

    QBoxLayout* labelLayout = new QBoxLayout(QBoxLayout::LeftToRight);

    QLabel* l1 = new QLabel("-180");
    l1->setAlignment(Qt::AlignLeft);
    labelLayout->addWidget(l1);
    QLabel* l2 = new QLabel("0");
    l2->setAlignment(Qt::AlignCenter);
    labelLayout->addWidget(l2);
    QLabel* l3 = new QLabel(" 180");
    l3->setAlignment(Qt::AlignRight);
    labelLayout->addWidget(l3);

    QSlider* slider = new QSlider(Qt::Orientation::Horizontal);
    slider->setMinimum(-180);
    slider->setMaximum(180);
    slider->setSingleStep(1);
    slider->setTickInterval(1);
    slider->setPageStep(1);
    int temp_rot = rotation;
    if (temp_rot < -180)
    {
        temp_rot += 360;
    }
    else if(temp_rot > 180)
    {
        temp_rot -= 360;
    }
    slider_label->setText(QString::number(temp_rot));
    slider->setValue(temp_rot);
    slider->setTracking(true);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(SliderMove(int)));

    boxLayout->addWidget(slider_label);
    boxLayout->addWidget(slider);
    boxLayout->addLayout(labelLayout);

    QPushButton* close_button = new QPushButton("Закрыть");
    connect(close_button, SIGNAL(clicked()), modalDialog, SLOT(accept()));
    boxLayout->addWidget(close_button);

    modalDialog->setLayout(boxLayout);
    modalDialog->exec();
}

void Figure::InfoAboutFigure(QMouseEvent *event)//нажатие правой кнопкой мыши по фигуре
{
    QMenu* cursorMenu = new QMenu();

    QBoxLayout* boxLayout = new QBoxLayout(QBoxLayout::Down);

    QPushButton* button_delete = new QPushButton("Удалить");
    connect(button_delete, SIGNAL(clicked()), cursorMenu, SLOT(close()));
    connect(button_delete, SIGNAL(clicked()), this, SLOT(FigureDeleteClicked()));

    QPushButton* button_change = new QPushButton("Изменить");
    connect(button_change, SIGNAL(clicked()), this, SLOT(FigureChangeClicked()));
    boxLayout->addWidget(button_delete);
    boxLayout->addWidget(button_change);

    QPushButton* button_move = new QPushButton("Переместить");
    button_move->setCheckable(true);
    button_move->setChecked(button_is_selected);
    connect(button_move, SIGNAL(clicked()), this, SLOT(FigureMove()));
    connect(button_move, SIGNAL(clicked()), cursorMenu, SLOT(close()));

    QPushButton* button_rotate = new QPushButton("Повернуть");
    connect(button_rotate, SIGNAL(clicked()), this, SLOT(FigureRotate()));

    boxLayout->addWidget(button_move);
    boxLayout->addWidget(button_rotate);

    cursorMenu->setLayout(boxLayout);
    cursorMenu->popup(QPoint(event->globalPosition().x(), event->globalPosition().y()));
}



