#include "Toolbar.h"

Toolbar::Toolbar(QMainWindow *parent) : QToolBar(parent)
{
    /*
     * По умолчанию не один тип фигуры не выбран, создаём кнопки для типов, привязываем сигналы к слотам
     */
    chosen_figure12 = false;
    chosen_figure22 = false;
    radio_button_figure12 = new QPushButton("Фигура 12");
    radio_button_figure22 = new QPushButton("Фигура 22");
    radio_button_figure12->setCheckable(true);
    radio_button_figure22->setCheckable(true);
    connect(radio_button_figure12, SIGNAL(clicked()), this, SLOT(Figure12Toggled()));
    connect(radio_button_figure22, SIGNAL(clicked()), this, SLOT(Figure22Toggled()));
    /*
     * Создаём кнопки для удаления и добавления фигур, привязываем сигналы к слотам
     */
    button_add = new QPushButton("Добавить");
    button_delete = new QPushButton("Удалить");
    button_add->setDisabled(true);
    button_delete->setDisabled(true);

    button_fit = new QPushButton("Уместить");
    connect(button_fit, SIGNAL(clicked()), this, SLOT(FiguresFit()));

    QPushButton* menu_button = new QPushButton("Удаление");
    QMenu* menu = new QMenu(this);
    QAction* action_all = new QAction();
    action_all->setData(0);
    action_all->setText("Удалить все");
    QAction* action_intersected = new QAction();
    action_intersected->setData(1);
    action_intersected->setText("Удалить пересекающиеся");
    menu->addAction(action_all);
    menu->addAction(action_intersected);
    menu_button->setMenu(menu);

    connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(MenuTriggered(QAction*)));

    connect(button_add, SIGNAL(clicked()), this, SLOT(FigureAddTriggered()));
    connect(button_delete, SIGNAL(clicked()), this, SLOT(FigureDeleteTriggered()));
    /*
     * Крепим кнопки к toolbar вместе с разделителями, а также настраиваем размер toolbar, делаем его неперемещаемым
     */
    this->resize(1000,50);
    this->setMovable(false);
    this->addWidget(radio_button_figure12);
    this->addWidget(radio_button_figure22);
    this->addSeparator();
    this->addWidget(button_add);
    this->addSeparator();
    this->addWidget(button_delete);
    this->addSeparator();
    this->addWidget(menu_button);
    this->addSeparator();
    this->addWidget(button_fit);
    this->show();
}

void Toolbar::FigureDeleteTriggered()
{
    figures.erase(std::remove(figures.begin(), figures.end(), SELECTED_FIGURE), figures.end());
    delete SELECTED_FIGURE;
    SELECTED_FIGURE = nullptr;
    button_delete->setDisabled(true);
}

void Toolbar::FigureAddTriggered()
{
    if (chosen_figure12)
    {
        Figure* newFigure = new Figure(parentWidget(), 12);
        newFigure->lower();
        newFigure->show();
        figures.push_back(newFigure);
    }
    else if (chosen_figure22)
    {
        Figure* newFigure = new Figure(parentWidget(), 22);
        newFigure->lower();
        newFigure->show();
        figures.push_back(newFigure);
    }
}

void Toolbar::Figure12Toggled()
{
    chosen_figure22 = false;
    if (chosen_figure12)
    {
        radio_button_figure12->setChecked(false);
        chosen_figure12 = false;
    }
    else
    {
        radio_button_figure22->setChecked(false);
        chosen_figure12 = true;
    }
    if (!chosen_figure12 && !chosen_figure22)
    {
        button_add->setDisabled(true);
    }
    else
    {
        button_add->setDisabled(false);
    }
}

void Toolbar::MenuTriggered(QAction* act)
{
    if (act->data() == 0)
    {
        for(auto i: figures)
        {
            if (i == SELECTED_FIGURE)
            {
                SELECTED_FIGURE = nullptr;
            }
            i->deleteLater();
        }
        figures.clear();
    }
    else
    {
        std::set<Figure*> flag_on_deletion;
        for(size_t i = 0; i < figures.size(); ++i)
        {
            if (figures[i] == SELECTED_FIGURE)
            {
                SELECTED_FIGURE = nullptr;
            }
            auto Points1 = figures[i]->PointsPositions();
            for(size_t j = i + 1; j < figures.size(); ++j)
            {
                if (figures[j] == SELECTED_FIGURE)
                {
                    SELECTED_FIGURE = nullptr;
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
                    flag_on_deletion.insert(figures[i]);
                    flag_on_deletion.insert(figures[j]);
                }
            }
        }
        for(auto i: flag_on_deletion)
        {
            figures.erase(std::remove(figures.begin(), figures.end(), i), figures.end());
            i->deleteLater();
        }
    }
}

void Toolbar::FiguresFit()
{
    int summ_w = 0;//суммарная ширина
    int summ_h = 0;//суммарная высота
    bool size_changing = false;
    //60 - min высота
    //100 - min ширина
    //Проверка на то, что даже уменьшенные фигуры влезут
    if (this->window()->width() - 2 < 102 || this->window()->height() - 25 < 62 ||
            (int(figures.size()) * 62 > this->window()->width() && this->window()->height() < 102 * 2) ||
            (int(figures.size()) * 102 > this->window()->width() && this->window()->height() - 25 < 62 * 2))
    {
        QMessageBox::warning(this, "Ошибка!", "Фигуры невозможно уместить!");
        return;
    }
    //Вычисляем суммарную ширину и высоту
    for(size_t i = 0; i < size_t(figures.size()); ++i)
    {
        auto w_h = figures[i]->GetWidthHeight();
        summ_w += w_h.first + 2;
        summ_h += w_h.second + 2;
    }
    //Проверяем, нужно ли уменьшать фигуры до минимального размера
    if(summ_w > this->window()->width() || summ_h > this->window()->height() - 25)
    {
        size_changing = true;
    }
    //"Умещаем" фигуры
    for(int i = 0, w = 0, h = 0; i < int(figures.size()); ++i)
    {
        figures[i]->SetRotation(90);
        if(size_changing)
        {
            figures[i]->SetWidthHeight(std::make_pair(100, 60));
            if (w + 62 > this->window()->width())
            {
                w = 0;
                h += 102;
            }
        }
        figures[i]->SetPosition({w, h + 25});
        auto w_h = figures[i]->GetWidthHeight();
        w += w_h.second + 2;
    }
}

void Toolbar::Figure22Toggled()
{
    chosen_figure12 = false;
    if (chosen_figure22)
    {
        radio_button_figure22->setChecked(false);
        chosen_figure22 = false;
    }
    else
    {
        radio_button_figure12->setChecked(false);
        chosen_figure22 = true;
    }
    if (!chosen_figure12 && !chosen_figure22)
    {
        button_add->setDisabled(true);
    }
    else
    {
        button_add->setDisabled(false);
    }
}
