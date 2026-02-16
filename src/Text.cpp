#include "Text.h"
#include <QFontMetrics>

namespace editor{
    Text::Text(double x, double y, const QString& content, int fontSize, const QString& color)
        : GraphicsObject(fontSize, color, color), x_(x), y_(y), content_(content), font_size_(fontSize), color_(color) {}
    void Text::Draw(QPainter* painter){
        painter->setPen(QPen(QColor(color_)));
        painter->setFont(QFont("Arial", font_size_));
        painter->drawText(QPointF(x_, y_), content_);
    }
    QRectF Text::boundBox() const{
        QFontMetrics fm(QFont("Arial", font_size_));
        return QRectF(x_, y_ - fm.ascent(), fm.horizontalAdvance(content_), fm.height());
    }
    bool Text::containsPoint(const QPoint& point) const{
        return boundBox().contains(point);
    }
    QString Text::SvgConvert() const{
        QString svg = R"(<text x="%1" y="%2" fill="%3" font-size="%4">%5</text>)";
        return svg.arg(x_).arg(y_).arg(color_).arg(font_size_).arg(content_);
    }
    void Text::moveBy(const QPoint& delta){
        x_ += delta.x();
        y_ += delta.y();
    }
    std::unique_ptr<GraphicsObject> Text::cloneShape() const{
        return std::make_unique<Text>(*this);
    }
    void Text::setStrokeWidth(int width){
        stroke_width_ = width;
    }
    void Text::setStrokeColor(const QString& color){
        color_ = color;
    }
    void Text::setFillColor(const QString& color){

    }
}