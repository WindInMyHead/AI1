#pragma once

#include "Points.hpp"


class MainWindow {

private:

    Points* _points;

    bool _fixSampleButIsPressed = false;

    method _mtd = method::neighbour;

    //buttons
    HWND fixSampleButton;
    HWND etalonModeButton;
    HWND neighbourModeButton;
    HWND createPointTwoRegionButton;
    HWND createPointSquareButton;
    HWND clearPointSet;

    enum button {
        ID_fixSampleButton = 0,
        ID_etalonModeButton,
        ID_neighbourModeButton,
        ID_createPointTwoRegionButton,
        ID_createPointSquareButton,
        ID_clearPointSet,
        _count,
        begin = 0,
        end = _count
    };

public:
    MainWindow(Points* points) {
        _points = points;
    }


    POINT GetCursorPosition(HWND hWnd, LPARAM lParam);
    void PrintCursorCoord(HWND hWnd, RECT rect, LPARAM lParam);
    POINT GetLBMPosition(HWND hWnd);


    void Split(HWND hWnd);


    static LRESULT CALLBACK WndProcWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT APIENTRY WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


    ~MainWindow() {
        delete _points;
    }
};