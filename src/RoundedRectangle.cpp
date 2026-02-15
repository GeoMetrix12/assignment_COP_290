#include "RoundedRectangle.h"
#include <cmath>

namespace editor{
    RoundedRectangle::RoundedRectangle(double x, double y, double width, double height, double radius, double stroke_width, QString stroke, QString fill)
        :   Rectangle(x, y, width, height, stroke_width, stroke, fill), 
            radius_(radius) {}
    void RoundedRectangle::Draw(QPainter* painter){
        painter->setPen(QPen(QColor(stroke_color_), stroke_width_));
        painter->setBrush(QBrush(QColor(fill_color_)));
        painter->drawRoundedRect(QRectF(x_, y_, width_, height_), radius_, radius_);
        painter->setPen(QPen(Qt::blue, 1));
        painter->setBrush(QBrush(Qt::white));
        painter->drawEllipse(QPointF(x_ + radius_, y_ + radius_), 4, 4);
    }
    QString RoundedRectangle::SvgConvert() const{
        QString svg = R"(<rect x="%1" y="%2" width="%3" height="%4" rx="%5" ry="%5" stroke="%6" fill="%7" stroke-width="%8" />)";
        return svg.arg(x_).arg(y_).arg(width_).arg(height_).arg(radius_).arg(stroke_color_).arg(fill_color_).arg(stroke_width_);
    }
    bool RoundedRectangle::containsPoint(const QPoint& point) const{
        double px = point.x();
        double py = point.y();
        if(px < x_ || px > x_ + width_ || py < y_ || py > y_ + height_) return false;
        if(px >= x_ + radius_ && px <= x_ + width_ - radius_) return true;
        if(py >= y_ + radius_ && py <= y_ + height_ - radius_) return true;
        double cx, cy;
        if(px < x_ + radius_) cx = x_ + radius_;
        else cx = x_ + width_ - radius_;
        if(py < y_ + radius_) cy = y_ + radius_;
        else cy = y_ + height_ - radius_;
        double dx = px - cx;
        double dy = py - cy;
        return (dx * dx + dy * dy) <= (radius_ * radius_);
    }
}