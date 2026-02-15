#include "Rectangle.h"

namespace editor {
    Rectangle::Rectangle(double x, double y, double width, double height, double stroke_width, QString stroke, QString fill)
        :   GraphicsObject(stroke_width, stroke, fill), 
            x_(x), 
            y_(y), 
            width_(width),
            height_(height) {}
    void Rectangle::Draw(QPainter* painter) {
        painter->setPen(QPen(QColor(stroke_color_), stroke_width_));
        painter->setBrush(QBrush(QColor(fill_color_)));
        painter->drawRect(x_, y_, width_, height_);
    }
    QString Rectangle::SvgConvert() const {
        QString svg = R"(<rect x="%1" y="%2" width="%3" height="%4" stroke="%5" fill="%6" stroke-width="%7" />)";
        return svg.arg(x_).arg(y_).arg(width_).arg(height_).arg(stroke_color_).arg(fill_color_).arg(stroke_width_);
    }
    bool Rectangle::containsPoint(const QPoint& point) const{
        return (point.x() >= x_ && point.x() <= x_ + width_ && 
                point.y() >= y_ && point.y() <= y_ + height_);
    }
    void Rectangle::moveBy(const QPoint& delta) {
        x_ += delta.x();
        y_ += delta.y();
    }
}