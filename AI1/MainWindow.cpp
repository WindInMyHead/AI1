#include "MainWindow.hpp"


POINT MainWindow::GetCursorPosition(HWND hWnd, LPARAM lParam) {

    auto x = LOWORD(lParam);
    auto y = HIWORD(lParam);

    POINT point{ x,y };
    //ScreenToClient(hWnd, &point);

    return point;
}


void MainWindow::PrintCursorCoord(HWND hWnd, RECT rect, LPARAM lParam) {

    POINT point = GetCursorPosition(hWnd, lParam);
    HDC hdc = GetDC(hWnd);

    TCHAR szBuf[80];
    auto size = wsprintf(szBuf, TEXT("(%.4d, %.4d)"), point.x, point.y);

    TextOut(hdc, rect.right - 100, rect.top + 10, szBuf, 12);
}


void MainWindow::Split(HWND hWnd) {

    // memory buf and screen buf
    HDC hdc, hdcMem;
    HBITMAP hbmMem, hbmOld;

    RECT rc;
    GetClientRect(hWnd, &rc);
    int cx = rc.right - rc.left - 320;
    int cy = rc.bottom - rc.top;

    hdc = GetDC(hWnd);
    hdcMem = CreateCompatibleDC(hdc);
    hbmMem = CreateCompatibleBitmap(hdc, cx, cy);
    hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

    HBRUSH greenBrush = CreateSolidBrush(RGB(0, 128, 0));
    HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 128));

    // отрисовываем на первом буфере
    for (auto i = 0; i < cx; i += 1) {
        for (auto j = 0; j < cy; j += 1) {

            RECT clientRect{ i, j, i + 1, j + 1 };
            auto objClass = _points->GetNearestPointColor({ i,j }, _mtd);

            using enum Class;

            if (objClass == undefined)
                return;

            switch (objClass) {
            case blue:
                FillRect(hdcMem, &clientRect, blueBrush);
                break;
            case green:
                FillRect(hdcMem, &clientRect, greenBrush);
                break;
            default:
                break;
            }
        }
    }

    DeleteObject(greenBrush);
    DeleteObject(blueBrush);

    // копируем первый буфер на второй
    BitBlt(hdc, 0, 0, cx, cy, hdcMem, 0, 0, SRCCOPY);

    // освобождаем ресурсы
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);
    ReleaseDC(hWnd, hdc);
}


POINT MainWindow::GetLBMPosition(HWND hWnd) {

    MSG msg;
    BOOL bRet;
    POINT p;

    while ((bRet = GetMessage(&msg, hWnd, 0, 0)) != 0) {

        if (bRet == -1)
            return { 0,0 };


        switch (msg.message) {
        case WM_LBUTTONDOWN: return GetCursorPosition(hWnd, msg.lParam);
        case WM_DESTROY:     break;
        default:             break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK MainWindow::WndProcWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    MainWindow* pThis = NULL;

    if (message == WM_NCCREATE)
    {
        // Получаем указатель на объект класса из параметра lParam
        pThis = static_cast<MainWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
        // Сохраняем указатель на объект класса в пользовательских данных окна
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        // Извлекаем указатель на объект класса из пользовательских данных окна
        pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    // Вызываем нестатический метод обработки сообщений
    if (pThis)
        return pThis->WndProc(hWnd, message, wParam, lParam);

    return DefWindowProc(hWnd, message, wParam, lParam);
}


LRESULT APIENTRY MainWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    HDC hdc = GetDC(hWnd);
    RECT rect;
    GetWindowRect(hWnd, &rect);

    switch (msg) {

    case WM_CREATE:
    {
        using enum button;

        auto hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
        fixSampleButton = CreateWindow(L"BUTTON", L"Fix sample", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 310, rect.bottom - 90, 130, 30, hWnd, (HMENU)ID_fixSampleButton, hInst, 0);
        etalonModeButton = CreateWindow(L"BUTTON", L"Etalon mode", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 160, rect.bottom - 130, 130, 30, hWnd, (HMENU)ID_etalonModeButton, hInst, 0);
        neighbourModeButton = CreateWindow(L"BUTTON", L"Neighbour mode", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 160, rect.bottom - 90, 130, 30, hWnd, (HMENU)ID_neighbourModeButton, hInst, 0);
        createPointTwoRegionButton = CreateWindow(L"BUTTON", L"2 region", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 310, rect.bottom - 170, 130, 30, hWnd, (HMENU)ID_createPointTwoRegionButton, hInst, 0);
        createPointSquareButton = CreateWindow(L"BUTTON", L"Square", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 310, rect.bottom - 130, 130, 30, hWnd, (HMENU)ID_createPointSquareButton, hInst, 0);
        clearPointSet = CreateWindow(L"BUTTON", L"Clear", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 160, rect.bottom - 170, 130, 30, hWnd, (HMENU)ID_clearPointSet, hInst, 0);

        PrintCursorCoord(hWnd, rect, lParam);

        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_COMMAND:
    {
        using enum button;
        switch (wParam) {
        case ID_fixSampleButton:
            _fixSampleButIsPressed = true;
            Split(hWnd);
            break;
        case ID_etalonModeButton:
            _mtd = method::etalon;
            MessageBox(hWnd, L"Включен метод эталонных образов", L"Message", MB_OK);
            break;
        case ID_neighbourModeButton:
            _mtd = method::neighbour;
            MessageBox(hWnd, L"Включен метод ближайшего соседа", L"Message", MB_OK);
            break;
        case ID_createPointTwoRegionButton:
            _points->DefaultPoints(PointDrawType::twoRegion, hWnd, lParam);
            break;
        case ID_createPointSquareButton:
            MessageBox(hWnd, L"Выбери центр", L"Message", MB_OK);
            _points->DefaultPoints(PointDrawType::square, hWnd, lParam, GetLBMPosition(hWnd));
            break;
        case ID_clearPointSet:
            _points->Clear();

            RECT r1, r2;
            r1.left = rect.left, r1.top = rect.top, r1.bottom = rect.bottom, r1.right = rect.right - 320;
            r2.left = r1.right, r2.top = rect.top, r2.bottom = rect.bottom - 180, r2.right = rect.right;

            FillRect(hdc, &r1, NULL);
            FillRect(hdc, &r2, NULL);

            break;
        }
    }
    case WM_RBUTTONDOWN:
    {
        if (_fixSampleButIsPressed)
            break;

        auto point = GetCursorPosition(hWnd, lParam);
        auto x = point.x;
        auto y = point.y;

        if (x > rect.right - 311 && y > rect.bottom - 171)
            break;

        HBRUSH brush;
        brush = CreateSolidBrush(RGB(0, 255, 0));
        SelectObject(hdc, brush);

        if (x > rect.right || y > rect.bottom)
            break;

        Ellipse(hdc, x - 4, y - 4, x + 4, y + 4);
        _points->GetGreenPoints().push_back({ x, y });
        DeleteObject(brush);

        break;
    }

    case WM_LBUTTONDOWN:
    {
        auto point = GetCursorPosition(hWnd, lParam);
        auto x = point.x;
        auto y = point.y;

        if (x > rect.right - 311 && y > rect.bottom - 171)
            break;

        HBRUSH brush = NULL;
        if (!_fixSampleButIsPressed) {
            brush = CreateSolidBrush(RGB(0, 0, 255));
            _points->GetBluePoints().push_back({ x, y });
        }

        else {

            auto objClass = _points->GetNearestPointColor({ x,y }, _mtd);
            using enum Class;
            switch (objClass) {
            case blue:
                brush = CreateSolidBrush(RGB(0, 0, 255));
                break;
            case green:
                brush = CreateSolidBrush(RGB(0, 255, 0));
                break;
            default:
                break;
            }
        }

        SelectObject(hdc, brush);
        Ellipse(hdc, x - 4, y - 4, x + 4, y + 4);
        DeleteObject(brush);

        break;
    }

    case WM_MOUSEMOVE:
    {
        RECT rect;
        GetWindowRect(hWnd, &rect);
        PrintCursorCoord(hWnd, rect, lParam);
        break;
    }

    case WM_CLOSE:
    {
        DestroyWindow(hWnd);
        break;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    }

    ReleaseDC(hWnd, hdc);

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

const char* ToString(PointDrawType type) {
    using enum PointDrawType;

    switch (type) {
    case square:    return "Square";
    case twoRegion: return "Two region";
    default:        return "[Unknown type]";
    }
}