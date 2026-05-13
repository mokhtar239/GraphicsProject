#pragma once
#include <vector>
#include <string>
#include "../core/Shape.h"

bool SaveShapes(const std::vector<Shape>& shapes, const std::string& filename);
bool LoadShapes(std::vector<Shape>& shapes, const std::string& filename);
