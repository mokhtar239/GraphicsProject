#include "FileIO.h"
#include <fstream>
#include <windows.h>
#include <iostream>

bool SaveShapes(const std::vector<Shape> &shapes, const std::string &filename) {
  std::ofstream out(filename);
  if (!out.is_open())
    return false;

  out << shapes.size() << "\n";
  for (const auto &s : shapes) {
    out << s.type << " " << s.x1 << " " << s.y1 << " " << s.x2 << " " << s.y2
        << " " << (int)GetRValue(s.color) << " " << (int)GetGValue(s.color)
        << " " << (int)GetBValue(s.color) << "\n";
  }
  return true;
}

bool LoadShapes(std::vector<Shape> &shapes, const std::string &filename) {
  std::ifstream in(filename);
  if (!in.is_open())
    return false;

  shapes.clear();
  size_t count;
  if (!(in >> count))
    return false;

  for (size_t i = 0; i < count; i++) {
    Shape s;
    int type;
    int r, g, b;
    if (in >> type >> s.x1 >> s.y1 >> s.x2 >> s.y2 >> r >> g >> b) {
      s.type = static_cast<ShapeType>(type);
      s.color = RGB(r, g, b);
      shapes.push_back(s);
    } else {
      break;
    }
  }
  
  if (shapes.size() != count) {
    std::cerr << "Warning: Malformed file. Expected " << count 
              << " shapes, loaded " << shapes.size() << ".\n";
    return false;
  }
  
  return true;
}
