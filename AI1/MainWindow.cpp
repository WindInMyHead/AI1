#include <Windows.h>
#include <string>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    auto CLASS_NAME = L"Sample Window Class";
    auto WINDOW_NAME = L"Sample Window";
    const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
    const int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        WINDOW_NAME,
        WS_POPUP | WS_VISIBLE,
        0,
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static int cxClient, cyClient;
    static int mouseX, mouseY;
    static std::string selection;
    static bool leftButtonDown = false, rightButtonDown = false;

    switch (msg) {
    case WM_CREATE:
    {
        // create buttons and combo box
        HWND hwndComboBox = CreateWindow(
            WC_COMBOBOX,
            L"",
            CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
            cxClient - 200, 0, 100, 300,
            hwnd,
            NULL,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL);
        SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"1");
        SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"2");
        SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"3");
        SendMessage(hwndComboBox, CB_SETCURSEL, 0, 0);

        CreateWindow(L"BUTTON", L"Сгенерировать точки", WS_VISIBLE | WS_CHILD, 10, cyClient - 50, 150, 30, hwnd, NULL, NULL, NULL);
        CreateWindow(L"BUTTON", L"Зафиксировать выборку", WS_VISIBLE | WS_CHILD, 10, cyClient - 90, 150, 30, hwnd, NULL, NULL, NULL);
        CreateWindow(L"BUTTON", L"Метод ближайшего соседа", WS_VISIBLE | WS_CHILD, cxClient - 160, cyClient - 50, 150, 30, hwnd, NULL, NULL, NULL);
        CreateWindow(L"BUTTON", L"Метод эталонов", WS_VISIBLE | WS_CHILD, cxClient - 160, cy



            natkorotkina@gmail.com
            допиши остаток кода, который не влез, в следующем сообщении
            Конец кода :

        cpp
            Copy code
            break;
    }
    case WM_LBUTTONDOWN:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        points.push_back({ x, y, 0 });
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        points.push_back({ x, y, 1 });
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}