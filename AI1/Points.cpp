#include "Points.hpp"

void Points::GetPointSet(std::vector<POINT>& pointSet, Class objClass) {

    using enum Class;

    switch (objClass) {

    case green:
        pointSet = _greenPoints;
        break;
    case blue:
        pointSet = _bluePoints;
        break;
    default:
        break;
    }
}


void Points::DefaultPoints(PointDrawType type, HWND hWnd, LPARAM lParam, POINT center) {

    std::random_device rd;
    std::mt19937 generator(rd());

    HDC dc = GetDC(hWnd); //Цепляемся к консольному окну
    RECT window = {}; //Переменная window будет использована для получения ширины и высоты окна

    HBRUSH brush; //Переменная brush - это кисть, она будет использоваться для закрашивания
    brush = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(dc, brush);

    using enum PointDrawType;
    switch (type) {
    case square:
    {

        auto x = center.x;
        auto y = center.y;

        auto x1 = x + 10;
        auto y1 = y - 10;
        Ellipse(dc, x1 - 4, y1 - 4, x1 + 4, y1 + 4);
        _greenPoints.push_back({ x1,y1 });

        x1 = x - 10;
        y1 = y + 10;
        Ellipse(dc, x1 - 4, y1 - 4, x1 + 4, y1 + 4);
        _greenPoints.push_back({ x1,y1 });

        brush = CreateSolidBrush(RGB(0, 0, 255));
        SelectObject(dc, brush);

        x1 = x - 10;
        y1 = y - 10;
        Ellipse(dc, x1 - 4, y1 - 4, x1 + 4, y1 + 4);
        _bluePoints.push_back({ x1,y1 });

        x1 = x + 10;
        y1 = y + 10;
        Ellipse(dc, x1 - 4, y1 - 4, x1 + 4, y1 + 4);
        _bluePoints.push_back({ x1,y1 });

        break;
    }
    case twoRegion:
    {
        std::uniform_int_distribution<> x_distribution(200, 400);
        std::uniform_int_distribution<> y_distribution(200, 400);

        for (int i = 0; i < 50; ++i) {
            int x, y;
            do {
                x = x_distribution(generator);
                y = y_distribution(generator);
            } while (sqrt(pow(x - 300, 2) + pow(y - 300, 2)) > 200);

            Ellipse(dc, x - 4, y - 4, x + 4, y + 4);
            _greenPoints.push_back({ x, y });
        }

        brush = CreateSolidBrush(RGB(0, 0, 255));
        SelectObject(dc, brush);


        std::uniform_int_distribution<> x_distribution1(1200, 1400);
        std::uniform_int_distribution<> y_distribution1(700, 900);

        for (int i = 0; i < 50; ++i) {
            int x, y;
            do {
                x = x_distribution1(generator);
                y = y_distribution1(generator);
            } while (sqrt(pow(x - 1300, 2) + pow(y - 800, 2)) > 200);

            Ellipse(dc, x - 4, y - 4, x + 4, y + 4);
            _bluePoints.push_back({ x, y });
        }

        break;
    }
    default:
        break;
    }

    DeleteObject(brush);
    ReleaseDC(hWnd, dc);
}


POINT Points::GetClassEtalon(Class objClass) {

    std::vector<POINT> pointSet;
    GetPointSet(pointSet, objClass);

    if (pointSet.empty())
        return{ 0,0 };

    long sumX = 0, sumY = 0;

    for (auto i = 0; i < pointSet.size(); i++) {
        sumX += pointSet[i].x;
        sumY += pointSet[i].y;
    }

    return { long(sumX / pointSet.size()), long(sumY / pointSet.size()) };
}


POINT Points::GetNearestPoint(POINT point, Class objClass) {

    std::vector<POINT> pointSet;
    GetPointSet(pointSet, objClass);

    POINT nearestPoint{ 0,0 };
    for (auto i = 0; i < pointSet.size(); i++) {

        int distance = 0, mindistance = (int)std::numeric_limits<int>::max;
        distance = sqrt(pow((point.x - pointSet[i].x), 2) + pow((point.y - pointSet[i].y), 2));
        if (distance < mindistance) {

            nearestPoint = pointSet[i];
            mindistance = distance;
        }
    }

    return nearestPoint;
}


Class Points::GetNearestPointColor(POINT point, method mthd) {

    using enum Class;
    double distanceToNearestPoint = (int)std::numeric_limits<int>::max, distance;
    Class nearestClass;

    for (auto i = begin; i < end; i = static_cast<Class>(static_cast<size_t>(i) + 1)) {

        using enum method;
        switch (mthd) {
        case etalon:
            auto etalon = GetClassEtalon(i);

            if (etalon.x == 0 && etalon.y == 0)
                return undefined;

            distance = sqrt(pow((etalon.x - point.x), 2) + pow((etalon.y - point.y), 2));
            break;
        case neighbour:
            auto pnt = GetNearestPoint(point, i);
            distance = sqrt(pow((pnt.x - point.x), 2) + pow((pnt.y - point.y), 2));
            break;
        default:
            break;
        }


        //если расстояние до точки меньше, чем ранее - перезапишем
        if (distance < distanceToNearestPoint) {
            distanceToNearestPoint = distance;
            nearestClass = i;
        }
        //если равно - проверим каких точек больше
        else if (distance == distanceToNearestPoint) {

            std::vector<POINT> class1;
            std::vector<POINT> class2;
            GetPointSet(class1, i);
            GetPointSet(class2, nearestClass);

            if (class1.size() < class2.size())
                nearestClass = i;
        }
    }
    return nearestClass;
}


void Points::Clear() {
    _greenPoints.clear();
    _bluePoints.clear();
}