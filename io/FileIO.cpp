#include "FileIO.h"
#include <fstream>
#include <windows.h>
#include <iostream>

bool SaveShapes(const std::vector<Shape> &shapes, const std::string &filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        return false;
    }

    out << shapes.size() << "\n";

    for (const auto &s : shapes) {
        out << s.type << " " << s.x1 << " " << s.y1 << " " << s.x2 << " " << s.y2
            << " " << (int)GetRValue(s.color) << " " << (int)GetGValue(s.color)
            << " " << (int)GetBValue(s.color) << "\n";

        out << (int)s.clipRegion.shape << " " 
            << s.clipRegion.xmin << " " << s.clipRegion.ymin << " " 
            << s.clipRegion.xmax << " " << s.clipRegion.ymax << " "
            << s.clipRegion.centerX << " " << s.clipRegion.centerY << " " 
            << s.clipRegion.radius << "\n";

        out << s.polygonPts.size() << "\n";
        for (const auto &pt : s.polygonPts) {
            out << pt.x << " " << pt.y << " ";
        }
        out << "\n";
    }

    out.close();
    return true;
}

bool LoadShapes(std::vector<Shape> &shapes, const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        return false;
    }

    shapes.clear();
    size_t count;
    if (!(in >> count)) {
        return false;
    }

    for (size_t i = 0; i < count; i++) {
        Shape s;
        int type, r, g, b;

        if (!(in >> type >> s.x1 >> s.y1 >> s.x2 >> s.y2 >> r >> g >> b)) {
            break;
        }
        s.type = static_cast<ShapeType>(type);
        s.color = RGB(r, g, b);

        int clipType;
        if (!(in >> clipType >> s.clipRegion.xmin >> s.clipRegion.ymin 
                >> s.clipRegion.xmax >> s.clipRegion.ymax 
                >> s.clipRegion.centerX >> s.clipRegion.centerY 
                >> s.clipRegion.radius)) {
            break;
        }
        s.clipRegion.shape = static_cast<ClipShape>(clipType);

        size_t polyCount;
        if (!(in >> polyCount)) {
            break;
        }
        
        s.polygonPts.clear();
        for (size_t j = 0; j < polyCount; j++) {
            POINT p;
            if (in >> p.x >> p.y) {
                s.polygonPts.push_back(p);
            }
        }
        
        shapes.push_back(s);
    }
    
    in.close();

    if (shapes.size() != count) {
        std::cerr << "Warning: Malformed file. Expected " << count 
                  << " shapes, loaded " << shapes.size() << ".\n";
        return false;
    }
    
    return true;
}