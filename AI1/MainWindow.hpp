//#pragma once
//
//#include <windows.h>
//#include <windowsx.h>
//
//class MainWindow{
//
//public:
//    LRESULT APIENTRY WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//
//        switch (msg) {
//
//        case WM_CREATE:
//        {
//            RECT rect;
//            GetWindowRect(hWnd, &rect);
//
//            using enum button;
//
//            auto hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
//            fixSampleButton = CreateWindow(L"BUTTON", L"Fix sample", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 310, rect.bottom - 90, 120, 30, hWnd, (HMENU)ID_fixSampleButton, hInst, 0);
//            etalonModeButton = CreateWindow(L"BUTTON", L"Etalon mode", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 160, rect.bottom - 130, 130, 30, hWnd, (HMENU)ID_etalonModeButton, hInst, 0);
//            neighbourModeButton = CreateWindow(L"BUTTON", L"Neighbour mode", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 160, rect.bottom - 90, 130, 30, hWnd, (HMENU)ID_neighbourModeButton, hInst, 0);
//            comboBox = CreateWindow(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS, rect.right - 310, rect.bottom - 170, 100, 200, hWnd, (HMENU)ID_combobox, hInst, NULL);
//            createPointSetButton = CreateWindow(L"BUTTON", L"Create point set", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rect.right - 310, rect.bottom - 130, 130, 30, hWnd, (HMENU)ID_createPointSetButton, hInst, 0);
//
//            for (auto i = defaultPointType::begin; i < defaultPointType::end; i = static_cast<defaultPointType>(static_cast<size_t>(i) + 1)) {
//                SendMessage(comboBox, CB_ADDSTRING, 1, (LPARAM)ToString(i));
//            }
//            SendMessage(comboBox, CB_SETCURSEL, 0, 0L);
//
//            UpdateCoord(hWnd, rect, lParam);
//
//            break;
//        }
//        case WM_PAINT:
//            PAINTSTRUCT ps;
//            BeginPaint(hWnd, &ps);
//            EndPaint(hWnd, &ps);
//            break;
//        case WM_COMMAND:
//        {
//            using enum button;
//            switch (wParam) {
//            case ID_fixSampleButton:
//                fixSampleButIsPressed = true;
//                break;
//            case ID_etalonModeButton:
//                mtd = method::etalon;
//                MessageBox(hWnd, L"Включен метод эталонных образов", L"Message", MB_OK);
//                break;
//            case ID_neighbourModeButton:
//                mtd = method::neighbour;
//                MessageBox(hWnd, L"Включен метод ближайшего соседа", L"Message", MB_OK);
//                break;
//            case ID_createPointSetButton:
//            {
//                if (fixSampleButIsPressed)
//                    break;
//
//                int sel = SendMessage(comboBox, CB_GETCURSEL, 0, 0L);
//                switch (sel) {
//                case 0:
//                {
//                    DefaultPoints(defaultPointType::random, hWnd, lParam);
//                    break;
//                }
//                case 1:
//                {
//                    MessageBox(hWnd, L"Выбери центр", L"Message", MB_OK);
//                    POINT p = GetCursorPosition(hWnd, lParam);
//                    DefaultPoints(defaultPointType::square, hWnd, lParam, { 200, 200 });
//                    break;
//                }
//                case 2:
//                {
//                    DefaultPoints(defaultPointType::twoRegion, hWnd, lParam);
//                    break;
//                }
//                default:
//                    break;
//
//                }
//
//                break;
//            }
//            default:
//                break;
//            }
//
//        }
//        case WM_RBUTTONDOWN:
//        {
//            if (fixSampleButIsPressed)
//                break;
//
//            HDC dc = GetDC(hWnd); //Цепляемся к консольному окну
//            RECT rect;
//            GetWindowRect(hWnd, &rect);
//
//            HBRUSH brush; //Переменная brush - это кисть, она будет использоваться для закрашивания
//            brush = CreateSolidBrush(RGB(0, 255, 0));
//            SelectObject(dc, brush);
//
//            auto point = GetCursorPosition(hWnd, lParam);
//            auto x = point.x;
//            auto y = point.y;
//
//            if (x > rect.right || y > rect.bottom)
//                break;
//
//            Ellipse(dc, x - 4, y - 4, x + 4, y + 4);
//            greenPoints.push_back({ x, y });
//
//            DeleteObject(brush);
//            ReleaseDC(hWnd, dc);
//
//            break;
//        }
//
//        case WM_LBUTTONDOWN:
//        {
//
//            RECT rect;
//            GetWindowRect(hWnd, &rect);
//
//            HDC dc = GetDC(hWnd);
//
//            HBRUSH brush = NULL; //Переменная brush - это кисть, она будет использоваться для закрашивания
//
//            auto point = GetCursorPosition(hWnd, lParam);
//            auto x = point.x;
//            auto y = point.y;
//
//
//            if (!fixSampleButIsPressed) {
//                brush = CreateSolidBrush(RGB(0, 0, 255));
//                bluePoints.push_back({ x, y });
//            }
//
//            else {
//
//                auto objClass = GetNearestPointColor({ x,y }, mtd);
//                using enum objectClass;
//                switch (objClass) {
//                case blue:
//                    brush = CreateSolidBrush(RGB(0, 0, 255));
//                    bluePoints.push_back({ x, y });
//                    break;
//                case green:
//                    brush = CreateSolidBrush(RGB(0, 255, 0));
//                    greenPoints.push_back({ x, y });
//                    break;
//                default:
//                    break;
//                }
//            }
//
//            SelectObject(dc, brush);
//
//            Ellipse(dc, x - 4, y - 4, x + 4, y + 4);
//
//            DeleteObject(brush);
//            ReleaseDC(hWnd, dc);
//
//            break;
//        }
//
//        case WM_MOUSEMOVE:
//        {
//            RECT rect;
//            GetWindowRect(hWnd, &rect);
//            UpdateCoord(hWnd, rect, lParam);
//            break;
//        }
//
//        case WM_CLOSE:
//        {
//            DestroyWindow(hWnd);
//            break;
//        }
//
//        case WM_DESTROY:
//        {
//            PostQuitMessage(0);
//            break;
//        }
//        }
//
//        return DefWindowProc(hWnd, msg, wParam, lParam);
//    }
//
//	void Init() {
//
//		TCHAR szClassName[] = TEXT("MyClass");
//		WNDCLASSEX wc{ sizeof(wc), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL, szClassName, LoadIcon(NULL, IDI_APPLICATION) };
//
//		if (!RegisterClassEx(&wc))
//			return 0;
//
//
//		_hWnd = CreateWindow(szClassName, TEXT("AI1"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZE, 0, 0, 1920, 1080, (HWND)NULL, (HMENU)NULL, (HINSTANCE)hInstance, NULL);
//	}
//
//private:
//    //main handle
//	HWND _hwnd;
//    //elements
//    HWND fixSampleButton;
//    HWND etalonModeButton;
//    HWND neighbourModeButton;
//    HWND comboBox;
//    HWND createPointSetButton;
//};
//
