#include "Hexagon.h"
#include <cmath>

namespace editor{
    Hexagon::Hexagon(double cx, double cy, double r, double width, QString stroke, QString fill)
        :   GraphicsObject(width, stroke, fill), 
            cx_(cx), 
            cy_(cy), 
            r_(r) {}
    void Hexagon::Draw(QPainter* painter){
        painter->setPen(QPen(QColor(stroke_color_), stroke_width_));
        painter->setBrush(QBrush(QColor(fill_color_)));
        QPolygonF hexagon;
        for(int i = 0; i < 6; i++){
            double angle = (M_PI / 3.0) * i;
            double x = cx_ + r_ * cos(angle);
            double y = cy_ + r_ * sin(angle);
            hexagon << QPointF(x, y);
        }
        painter->drawPolygon(hexagon);
    }
    QString Hexagon::SvgConvert() const{
        QString svg = R"(<polygon points="%1" stroke="%2" fill="%3" stroke-width="%4" />)";        
        QString points;
        for(int i = 0; i < 6; i++){
            double angle = (M_PI / 3.0) * i;
            double x = cx_ + r_ * cos(angle);
            double y = cy_ + r_ * sin(angle);
            points += QString("%1,%2 ").arg(x).arg(y);
        }
        return svg.arg(points.trimmed()).arg(stroke_color_).arg(fill_color_).arg(stroke_width_);
    }
    bool Hexagon::containsPoint(const QPoint& point) const{
        double px = std::abs(point.x() - cx_);
        double py = std::abs(point.y() - cy_);
        double hex_height = r_ * std::sqrt(3) / 2.0;
        if(px > r_ || py > hex_height) return false;
        return (hex_height - px) * (r_ * 1.5) >= py * hex_height;
    }
    void Hexagon::moveBy(const QPoint& delta){
        cx_ += delta.x();
        cy_ += delta.y();
    }
}