#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QString>
#include <QPainter>

namespace editor{
    class GraphicsObject{
        public:
            virtual ~GraphicsObject() = default;
            virtual void Draw(QPainter* painter) = 0;
            virtual QString ToSvg() const = 0;
        protected : 
            double stroke_width_ = 1.0;
            QString stroke_color_ = "black";
            QString fill_color_ = "none";
    };
}
#endif