#pragma once


enum class PointDrawType {
    square = 0,
    twoRegion,
    _count,
    begin = 0,
    end = _count
};


const char* ToString(PointDrawType type);