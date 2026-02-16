#include "Canvas.h"
#include "SvgParser.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Hexagon.h"
#include "RoundedRectangle.h"
#include "Text.h"
#include "Line.h"
#include "Commands.h"
#include "Freehand.h"
#include <QInputDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

namespace editor{
    void Canvas::loadFromFile(const QString& filename){
        selected_shape_ = nullptr;
        is_dragging_ = false;
        is_changing_radius_ = false;
        auto new_shapes = editor::SvgParser::ParseSvg(filename);
        if(new_shapes.empty()){
            qDebug() << "Failed to load shapes from file: " << filename;
        }
        else{
            qDebug() << "Loaded " << new_shapes.size() << " shapes from file: " << filename;
        }
        shapes_ = std::move(new_shapes);
        current_file_ = filename;
        update();
    }
    bool Canvas::saveToFile(const QString& filename){
        return editor::SvgParser::ModifyFile(filename, shapes_, width(), height());
    }
    void Canvas::clearCanvas(){
        shapes_.clear();
        update();
    }
}