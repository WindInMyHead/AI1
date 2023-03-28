#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <random>



std::vector<POINT> greenPoints;
std::vector<POINT> bluePoints;

enum class defaultPointType {
    random = 0,
    square,
    twoRegion,
    _count,
    begin = 0,
    end = _count
};

const char* ToString(defaultPointType type) {
    using enum defaultPointType;
    switch (type){
    case random:    return "Random";
    case square:    return "Square";
    case twoRegion: return "Two region";
    default:        return "[Unknown type]";
    }
}

void DefaultPoints(defaultPointType type, HWND hWnd, LPARAM lParam, POINT center = { 200, 200 }) {


    std::random_device rd;
    std::mt19937 generator(rd());

    HDC dc = GetDC(hWnd); //Цепляемся к консольному окну
    RECT window = {}; //Переменная window будет использована для получения ширины и высоты окна

    HBRUSH brush; //Переменная brush - это кисть, она будет использоваться для закрашивания
    brush = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(dc, brush);

    using enum defaultPointType;
    switch (type) {
    case random:
    {

        std::uniform_int_distribution<int> xDist(1, 1920);
        std::uniform_int_distribution<int> yDist(1, 1080);

        for (int i = 0; i < 50; i++) {
            auto x = xDist(generator);
            auto y = yDist(generator);

            Ellipse(dc, x - 4, y - 4, x + 4, y + 4);
            greenPoints.push_back({ x, y });
        }
        brush = CreateSolidBrush(RGB(0, 0, 255));
        SelectObject(dc, brush);

        for (int i = 0; i < 50; i++) {
            auto x = xDist(generator);
            auto y = yDist(generator);

            Ellipse(dc, x - 4, y - 4, x + 4, y + 4);
            bluePoints.push_back({ x, y });
        }

        brush = CreateSolidBrush(RGB(0, 0, 255));
        SelectObject(dc, brush);

        break;
    }
    case square:
    {

        auto x = center.x;
        auto y = center.y;

        auto x1 = x + 10;
        auto y1 = y - 10;
        Ellipse(dc, x1 - 4, y1 - 4, x1 + 4, y1 + 4);
        greenPoints.push_back({ x1,y1 });

        x1 = x - 10;
        y1 = y + 10;
        Ellipse(dc, x1 - 4, y1 - 4, x1 + 4, y1 + 4);
        greenPoints.push_back({ x1,y1 });

        brush = CreateSolidBrush(RGB(0, 0, 255));
        SelectObject(dc, brush);

        x1 = x - 10;
        y1 = y - 10;
        Ellipse(dc, x1 - 4, y1 - 4, x1 + 4, y1 + 4);
        bluePoints.push_back({ x1,y1 });

        x1 = x + 10;
        y1 = y + 10;
        Ellipse(dc, x1 - 4, y1 - 4, x1 + 4, y1 + 4);
        bluePoints.push_back({ x1,y1 });

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
            greenPoints.push_back({ x, y });
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
            bluePoints.push_back({ x, y });
        }

        break;
    }
    default:
        break;
    }

    DeleteObject(brush);
    ReleaseDC(hWnd, dc);
}


LRESULT APIENTRY WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT APIENTRY fixSampleButProc(HWND, UINT, WPARAM, LPARAM);
LRESULT APIENTRY ChangeModeButProc(HWND, UINT, WPARAM, LPARAM);

HWND fixSampleButton;
HWND etalonModeButton;
HWND neighbourModeButton;
HWND comboBox;
HWND createPointSetButton;

enum button{
    ID_fixSampleButton = 0,
    ID_etalonModeButton,
    ID_neighbourModeButton,
    ID_combobox,
    ID_createPointSetButton,
    _count,
    begin = 0,
    end = _count
};

enum class objectClass {
    green = 0,
    blue,
    _count,
    begin = 0,
    end = _count
};

enum class method {
    etalon = 0,
    neighbour,
    _count,
    begin = 0,
    end = _count
};

method mtd = method::neighbour;

void GetPointSet(std::vector<POINT>& pointSet, objectClass objClass) {

    using enum objectClass;

    switch (objClass) {

    case green:
        pointSet = greenPoints;
        break;
    case blue:
        pointSet = bluePoints;
        break;
    default:
        break;
    }
}

POINT GetClassEtalon(objectClass objClass) {

    std::vector<POINT> pointSet;
    GetPointSet(pointSet, objClass);

    long sumX = 0, sumY = 0;

    for (auto i = 0; i < pointSet.size(); i++) {
        sumX += pointSet[i].x;
        sumY += pointSet[i].y;
    }

    return { long(sumX / pointSet.size()), long(sumY / pointSet.size()) };
}


POINT GetNearestPoint(POINT point, objectClass objClass) {

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


objectClass GetNearestPointColor(POINT point, method mthd) {

    using enum objectClass;
    double distanceToNearestPoint = (int)std::numeric_limits<int>::max, distance;
    objectClass nearestClass;

    for (auto i = begin; i < end; i = static_cast<objectClass>(static_cast<size_t>(i) + 1)) {

        using enum method;
        switch (mthd) {
        case etalon:
            auto etalon = GetClassEtalon(i);
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


bool fixSampleButIsPressed = false;

POINT GetCursorPosition(HWND hWnd, LPARAM lParam) {

    auto x = LOWORD(lParam);
    auto y = HIWORD(lParam);

    POINT point{ x,y };
    //ScreenToClient(hWnd, &point);


    return point;
}

void UpdateCoord(HWND hWnd, RECT rect, LPARAM lParam, HDC hdcMem) {

    WORD xPos, yPos, nSize;
    TCHAR szBuf[80];

    // Сохраняем координаты курсора мыши
    POINT point = GetCursorPosition(hWnd, lParam);
    xPos = point.x;
    yPos = point.y;

    // Подготавливаем текстовую строку, содержащую
    // координаты курсора мыши
    nSize = wsprintf(szBuf, TEXT("(%d, %d)"), xPos, yPos);
    TextOut(hdcMem, rect.right - (nSize*9), rect.top + 10, szBuf, nSize);

    InvalidateRect(hWnd, &rect, false);
}


LRESULT APIENTRY WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT paintStruct;

    RECT rect;
    GetWindowRect(hWnd, &rect);

    HDC hdc = 0;
    HDC hdcMem = 0;
    HBITMAP buffer = 0;

    switch (msg) {

    case WM_CREATE:
    {
        /*RECT rect;
        GetWindowRect(hWnd, &rect);

        using enum button;

        auto hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
        fixSampleButton = CreateWindow(L"BUTTON", L"Fix sample", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 310, rect.bottom - 90, 120, 30, hWnd, (HMENU)ID_fixSampleButton, hInst, 0);
        etalonModeButton = CreateWindow(L"BUTTON", L"Etalon mode", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 160, rect.bottom - 130, 130, 30, hWnd, (HMENU)ID_etalonModeButton, hInst, 0);
        neighbourModeButton = CreateWindow(L"BUTTON", L"Neighbour mode", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 160, rect.bottom - 90, 130, 30, hWnd, (HMENU)ID_neighbourModeButton, hInst, 0);
        comboBox = CreateWindow(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS, rect.right - 310, rect.bottom - 170, 100, 200, hWnd, (HMENU)ID_combobox, hInst, NULL);
        createPointSetButton = CreateWindow(L"BUTTON", L"Create point set", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 310, rect.bottom - 130, 130, 30, hWnd, (HMENU)ID_createPointSetButton, hInst, 0);

        for (auto i = defaultPointType::begin; i < defaultPointType::end; i = static_cast<defaultPointType>(static_cast<size_t>(i) + 1)) {
            SendMessage(comboBox, CB_ADDSTRING, 1, (LPARAM)ToString(i));
        }
        SendMessage(comboBox, CB_SETCURSEL, 0, 0L);

        UpdateCoord(hWnd, rect, lParam);

        break;*/

        RECT rect;
        GetWindowRect(hWnd, &rect);

        using enum button;

        auto hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
        fixSampleButton = CreateWindow(L"BUTTON", L"Fix sample", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 310, rect.bottom - 90, 120, 30, hWnd, (HMENU)ID_fixSampleButton, hInst, 0);
        etalonModeButton = CreateWindow(L"BUTTON", L"Etalon mode", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 160, rect.bottom - 130, 130, 30, hWnd, (HMENU)ID_etalonModeButton, hInst, 0);
        neighbourModeButton = CreateWindow(L"BUTTON", L"Neighbour mode", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 160, rect.bottom - 90, 130, 30, hWnd, (HMENU)ID_neighbourModeButton, hInst, 0);
        comboBox = CreateWindow(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS, rect.right - 310, rect.bottom - 170, 100, 200, hWnd, (HMENU)ID_combobox, hInst, NULL);
        createPointSetButton = CreateWindow(L"BUTTON", L"Create point set", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 310, rect.bottom - 130, 130, 30, hWnd, (HMENU)ID_createPointSetButton, hInst, 0);

        for (auto i = defaultPointType::begin; i < defaultPointType::end; i = static_cast<defaultPointType>(static_cast<size_t>(i) + 1)) {
            SendMessage(comboBox, CB_ADDSTRING, 1, (LPARAM)ToString(i));
        }
        SendMessage(comboBox, CB_SETCURSEL, 0, 0L);

        hdc = BeginPaint(hWnd, &paintStruct);
        hdcMem = CreateCompatibleDC(hdc);
        buffer = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
        SelectObject(hdcMem, buffer);
        EndPaint(hWnd, &paintStruct);

        UpdateCoord(hWnd, rect, lParam, hdcMem);

        break;
    }
    case WM_PAINT:  

        hdc = BeginPaint(hWnd, &paintStruct);
        BitBlt(hdc, rect.left, rect.top, rect.right, rect.bottom, hdcMem, rect.left, rect.top, SRCCOPY);
        EndPaint(hWnd, &paintStruct);
        //PAINTSTRUCT ps;
        //BeginPaint(hWnd, &ps);
        //EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
    {
        using enum button;
        switch (wParam) {
        case ID_fixSampleButton:
            fixSampleButIsPressed = true;
            break;
        case ID_etalonModeButton:
            mtd = method::etalon;
            MessageBox(hWnd, L"Включен метод эталонных образов", L"Message", MB_OK);
            break;
        case ID_neighbourModeButton:
            mtd = method::neighbour;
            MessageBox(hWnd, L"Включен метод ближайшего соседа", L"Message", MB_OK);
            break;
        case ID_createPointSetButton:
        {
            if (fixSampleButIsPressed)
                break;

            int sel = SendMessage(comboBox, CB_GETCURSEL, 0, 0L);
            switch (sel) {
            case 0:
            {
                DefaultPoints(defaultPointType::random, hWnd, lParam);
                break;
            }
            case 1:
            {
                MessageBox(hWnd, L"Выбери центр", L"Message", MB_OK);
                POINT p = GetCursorPosition(hWnd, lParam);
                DefaultPoints(defaultPointType::square, hWnd, lParam, { 200, 200 });
                break;
            }
            case 2:
            {
                DefaultPoints(defaultPointType::twoRegion, hWnd, lParam);
                break;
            }
            default:
                break;

            }

            break;
        }
        default:
            break;
        }

    }
    case WM_RBUTTONDOWN:
    {
        if (fixSampleButIsPressed)
            break;

        HBRUSH brush; //Переменная brush - это кисть, она будет использоваться для закрашивания
        brush = CreateSolidBrush(RGB(0, 255, 0));
        SelectObject(hdcMem, brush);

        auto point = GetCursorPosition(hWnd, lParam);
        auto x = point.x;
        auto y = point.y;

        if (x > rect.right || y > rect.bottom)
            break;

        Ellipse(hdcMem, x - 4, y - 4, x + 4, y + 4);
        greenPoints.push_back({ x, y });

        DeleteObject(brush);
        InvalidateRect(hWnd, &rect, false);

        break;
    }

    case WM_LBUTTONDOWN:
    {
        HBRUSH brush = NULL; //Переменная brush - это кисть, она будет использоваться для закрашивания

        auto point = GetCursorPosition(hWnd, lParam);
        auto x = point.x;
        auto y = point.y;


        if (!fixSampleButIsPressed) {
            brush = CreateSolidBrush(RGB(0, 0, 255));
            bluePoints.push_back({ x, y });
        }

        else {

            auto objClass = GetNearestPointColor({ x,y }, mtd);
            using enum objectClass;
            switch (objClass) {
            case blue:
                brush = CreateSolidBrush(RGB(0, 0, 255));
                bluePoints.push_back({ x, y });
                break;
            case green:
                brush = CreateSolidBrush(RGB(0, 255, 0));
                greenPoints.push_back({ x, y });
                break;
            default:
                break;
            }
        }

        SelectObject(hdcMem, brush);
        Ellipse(hdcMem, x - 4, y - 4, x + 4, y + 4);
        DeleteObject(brush);

        InvalidateRect(hWnd, &rect, false);

        break;
    }

    case WM_MOUSEMOVE:
    {
        UpdateCoord(hWnd, rect, lParam, hdcMem);
        break;
    }

    case WM_CLOSE:
    {
        DestroyWindow(hWnd);
        break;
    }

    case WM_DESTROY:
    {
        ::DeleteObject(buffer);
        ::ReleaseDC(hWnd, hdc);
        PostQuitMessage(0);
        break;
    }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}


void SplitPlane(HWND hWnd) {

    RECT rect;
    GetWindowRect(hWnd, &rect);



}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    HWND hWnd;

    TCHAR szClassName[] = TEXT("MyClass");
    WNDCLASSEX wc{ sizeof(wc), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL, szClassName, LoadIcon(NULL, IDI_APPLICATION) };

    if (!RegisterClassEx(&wc))
        return 0;
  

    hWnd = CreateWindow(szClassName, TEXT("AI1"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZE, 0, 0, 1920, 1080, (HWND)NULL, (HMENU)NULL, (HINSTANCE)hInstance, NULL);
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
    UpdateWindow(hWnd);
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}


