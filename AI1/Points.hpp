#pragma once

#include <vector>
#include <random>
#include <Windows.h>

#include "PointDrawType.hpp"
#include "Class.hpp"
#include "Method.hpp"


class Points{

private:
	std::vector<POINT> _greenPoints;
	std::vector<POINT> _bluePoints;

public:
    Points() {
        _greenPoints = {};
        _bluePoints = {};
    }

    void GetPointSet(std::vector<POINT>& pointSet, Class objClass);

    void DefaultPoints(PointDrawType type, HWND hWnd, LPARAM lParam, POINT center = { 200, 200 });

    POINT GetClassEtalon(Class objClass);

    POINT GetNearestPoint(POINT point, Class objClass);

    Class GetNearestPointColor(POINT point, method mthd);

    void Clear();

    std::vector<POINT>& GetGreenPoints() {
        return _greenPoints;
    }

    std::vector<POINT>& GetBluePoints() {
        return _bluePoints;
    }

    ~Points() = default;

};

