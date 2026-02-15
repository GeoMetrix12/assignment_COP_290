#include "Circle.h"

namespace editor{
    Circle::Circle(double cx, double cy, double r, double width, QString stroke, QString fill)
        :   GraphicsObject(width, stroke, fill), 
            cx_(cx), 
            cy_(cy), 
            r_(r) {}
    void Circle::Draw(QPainter* painter){
        painter->setPen(QPen(QColor(stroke_color_), stroke_width_));
        painter->setBrush(QBrush(QColor(fill_color_)));
        painter->drawEllipse(QPointF(cx_, cy_), r_, r_);
    }
    QString Circle::SvgConvert() const{
        QString svg = R"(<circle cx="%1" cy="%2" r="%3" stroke="%4" fill="%5" stroke-width="%6" />)";
        return svg.arg(cx_).arg(cy_).arg(r_).arg(stroke_color_).arg(fill_color_).arg(stroke_width_);
    }
    bool Circle::containsPoint(const QPoint& point) const{
        double dx = point.x() - cx_;
        double dy = point.y() - cy_;
        return (dx * dx + dy * dy) <= (r_ * r_);
    }
    void Circle::moveBy(const QPoint& delta){
        cx_ += delta.x();
        cy_ += delta.y();
    }
}