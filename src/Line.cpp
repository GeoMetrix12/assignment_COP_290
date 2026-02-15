#include "Line.h"
#include <QLineF>

namespace editor{
    Line::Line(const QPointF& start, const QPointF& end, double width, const QString& stroke)
        :   GraphicsObject(width, stroke, "none"), 
            p1_(start), 
            p2_(end) {}
    void Line::Draw(QPainter* painter){
        painter->setPen(QPen(QColor(stroke_color_), stroke_width_, Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(p1_, p2_);
    }
    QString Line::SvgConvert() const{
        QString svg = R"(<line x1="%1" y1="%2" x2="%3" y2="%4" stroke="%5" stroke-width="%6" />)";
        return svg.arg(p1_.x()).arg(p1_.y()).arg(p2_.x()).arg(p2_.y()).arg(stroke_color_).arg(stroke_width_);
    }
    bool Line::containsPoint(const QPoint& point) const{
        QLineF line(p1_, p2_);
        double t = ((point.x() - p1_.x()) * (p2_.x() - p1_.x()) + (point.y() - p1_.y()) * (p2_.y() - p1_.y())) / (std::pow(p2_.x() - p1_.x(), 2) + std::pow(p2_.y() - p1_.y(), 2));
        if(t < 0 || t > 1) return false;
        QPointF projection = (p1_ + t * (p2_ - p1_));
        return QLineF(point, projection).length() < 7.0;
    }
    void Line::moveBy(const QPoint& delta){
        p1_ += delta;
        p2_ += delta;
    }
    QRectF Line::boundBox() const{
        QRectF box = QRectF(p1_, p2_).normalized();
        double extend = 5.0;
        if(box.width() < extend){
            box.setLeft(box.center().x() - extend/2);
            box.setWidth(extend);
        }
        if(box.height() < extend){
            box.setTop(box.center().y() - extend/2);
            box.setHeight(extend);
        }
        return box;
    }
    std::unique_ptr<GraphicsObject> Line::cloneShape() const {
        return std::make_unique<Line>(*this);
    }
    void Line::setStrokeWidth(int width) {
        stroke_width_ = width;
    }
    void Line::setStrokeColor(const QString& color) {
        stroke_color_ = color;
    }
}