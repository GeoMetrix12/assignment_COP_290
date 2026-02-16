#include "SvgParser.h"
#include "Circle.h"
#include "Rectangle.h"
#include "RoundedRectangle.h"
#include "Hexagon.h"
#include "Text.h"
#include "Freehand.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace editor {
    bool SvgParser::ModifyFile(const QString& filename, 
                               const std::vector<std::unique_ptr<GraphicsObject>>& shapes, 
                               int canvas_width, int canvas_height){
        QFile file(filename);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "Couldnt modify file: " << filename;
            return false;
        }
        QTextStream outstream(&file);
        outstream << "<svg width=\"" << canvas_width << "\" height=\"" << canvas_height << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
        for(const auto& shape : shapes){
            outstream << "  " << shape->SvgConvert() << "\n";
        }
        outstream << "</svg>\n";
        file.close();
        return true;
    }
    QString SvgParser::getValue(const QString& line, const QString& attribute_name){
        QString to_search = attribute_name + "=\"";
        int start_index = line.indexOf(to_search);
        if(start_index == -1) return "0";
        start_index += to_search.length();
        int end_index = line.indexOf("\"", start_index);
        if(end_index == -1) return "0";
        return line.mid(start_index, end_index - start_index);
    }
}