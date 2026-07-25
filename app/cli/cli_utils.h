#pragma once

#include <string>
#include <vector>
#include "../../sdk/earth/earth_point.h"

std::vector<double> parseDoubleList(const std::string& str);
yalgo::earth::EarthPoint parsePoint2D(const std::string& str);
yalgo::earth::EarthPoint parsePoint3D(const std::string& str);
double toWatts(double value, const std::string& unit);
double fromWatts(double value, const std::string& unit);
