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
    std::vector<std::unique_ptr<GraphicsObject>> SvgParser::ParseSvg(const QString& filename){
        std::vector<std::unique_ptr<GraphicsObject>> shapes;
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "Error opening file: " << filename;
            return shapes;
        }
        QTextStream instream(&file);
        while(!instream.atEnd()){
            QString line = instream.readLine();
            if(line.contains("<circle")) {
                double cx = getValue(line, "cx").toDouble();
                double cy = getValue(line, "cy").toDouble();
                double r = getValue(line, "r").toDouble();
                double stroke_width = getValue(line, "stroke-width").toDouble();
                QString stroke = getValue(line, "stroke");
                QString fill = getValue(line, "fill");
                shapes.push_back(std::make_unique<Circle>(cx, cy, r, stroke_width, stroke, fill));
            }
            else if(line.contains("<rect")) {
                double x = getValue(line, "x").toDouble();
                double y = getValue(line, "y").toDouble();
                double width = getValue(line, "width").toDouble();
                double height = getValue(line, "height").toDouble();
                double stroke_width = getValue(line, "stroke-width").toDouble();
                QString stroke = getValue(line, "stroke");
                QString fill = getValue(line, "fill");
                if(line.contains("rx=\"")){
                    double radius = getValue(line, "rx").toDouble();
                    shapes.push_back(std::make_unique<RoundedRectangle>(x, y, width, height, radius, stroke_width, stroke, fill));
                }
                else{
                    shapes.push_back(std::make_unique<Rectangle>(x, y, width, height, stroke_width, stroke, fill));
                }
            }
            else if(line.contains("<polygon")){
                QString pointsStr = getValue(line, "points");
                double stroke_width = getValue(line, "stroke-width").toDouble();
                QString stroke = getValue(line, "stroke");
                QString fill = getValue(line, "fill");
                QStringList pointsList = pointsStr.split(" ", Qt::SkipEmptyParts);
                if(pointsList.size() >= 6){
                    double minX = 1e9, minY = 1e9, maxX = -1e9, maxY = -1e9;
                    for(const QString& point : pointsList){
                        QStringList coords = point.split(",");
                        if(coords.size() == 2){
                            double x = coords[0].toDouble();
                            double y = coords[1].toDouble();
                            minX = std::min(minX, x);
                            minY = std::min(minY, y);
                            maxX = std::max(maxX, x);
                            maxY = std::max(maxY, y);
                        }
                    }
                    double cx = (minX + maxX) / 2.0;
                    double cy = (minY + maxY) / 2.0;
                    double r = (maxX - minX) / 2.0;
                    shapes.push_back(std::make_unique<Hexagon>(cx, cy, r, stroke_width, stroke, fill));
                }
            }
            else if(line.contains("<text")){
                double x = getValue(line, "x").toDouble();
                double y = getValue(line, "y").toDouble();
                int fontSize = getValue(line, "font-size").toInt();
                QString color = getValue(line, "fill");
                int startIndex = line.indexOf(">") + 1;
                int endIndex = line.indexOf("</text>");
                QString content = line.mid(startIndex, endIndex - startIndex);
                shapes.push_back(std::make_unique<Text>(x, y, content, fontSize, color));
            }
            else if(line.contains("<polyline")){
                QString pointsStr = getValue(line, "points");
                double stroke_width = getValue(line, "stroke-width").toDouble();
                QString stroke = getValue(line, "stroke");
                QStringList pointsList = pointsStr.split(" ", Qt::SkipEmptyParts);
                if(!pointsList.empty()){
                    QStringList firstCoord = pointsList[0].split(",");
                    QPointF startPoint(firstCoord[0].toDouble(), firstCoord[1].toDouble());
                    auto freehand = std::make_unique<Freehand>(startPoint, stroke_width, stroke);
                    for(int i = 1; i < pointsList.size(); i++){
                        QStringList coords = pointsList[i].split(",");
                        if(coords.size() == 2){
                            double x = coords[0].toDouble();
                            double y = coords[1].toDouble();
                            freehand->addPoint(QPoint(x, y));
                        }
                    }
                    shapes.push_back(std::move(freehand));
                }                   
            }
        }
        file.close();
        return shapes;
    }
}