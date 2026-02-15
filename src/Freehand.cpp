#include "Freehand.h"

namespace editor{
    Freehand::Freehand(const QPointF& start, int width, const QString& color)
        :   GraphicsObject(width, color, color), 
            width_(width), 
            color_(color) {
        path_ << start;
    }
    void Freehand::addPoint(const QPoint& point){
        path_ << QPointF(point);
    }
    void Freehand::Draw(QPainter* painter){
        QPen pen(QColor(color_), width_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(pen);
        painter->drawPolyline(path_);
    }
    bool Freehand::containsPoint(const QPoint& point) const{
        return boundBox().contains(point);
    }
    void Freehand::moveBy(const QPoint& delta){
        path_.translate(delta.x(), delta.y());
    }
    QRectF Freehand::boundBox() const{
        return path_.boundingRect();
    }
    void Freehand::scalePath(double scaleFactor, HandleType handle){
        QRectF box = boundBox();
        QPointF fixedPoint;
        switch(handle){
            case HandleType::HANDLE_TOP_LEFT:
                fixedPoint = QPointF(box.right(), box.bottom());
                break;
            case HandleType::HANDLE_TOP_RIGHT:
                fixedPoint = QPointF(box.left(), box.bottom());
                break;
            case HandleType::HANDLE_BOTTOM_LEFT:
                fixedPoint = QPointF(box.right(), box.top());
                break;
            case HandleType::HANDLE_BOTTOM_RIGHT:
                fixedPoint = QPointF(box.left(), box.top());
                break;
            default:
                return;
        }
        for(int i = 0; i < path_.size(); i++){
            QPointF vec = path_[i] - fixedPoint;
            path_[i] = fixedPoint + vec * scaleFactor;
        }
    }
    QString Freehand::SvgConvert() const{
        QString svg = R"(<polyline points="%1" stroke="%2" stroke-width="%3" fill="none" stroke-linecap="round" stroke-linejoin="round" />)";
        QString points;
        for(const QPointF& pt : path_){
            points += QString("%1,%2 ").arg(pt.x()).arg(pt.y());
        }
        return svg.arg(points.trimmed()).arg(color_).arg(width_);
    }
    std::unique_ptr<GraphicsObject> Freehand::cloneShape() const{
        return std::make_unique<Freehand>(*this);
    }
    void Freehand::setStrokeWidth(int width){
        width_ = width;
    }
    void Freehand::setStrokeColor(const QString& color){
        color_ = color;
    }
    void Freehand::setFillColor(const QString& color){

    }
}