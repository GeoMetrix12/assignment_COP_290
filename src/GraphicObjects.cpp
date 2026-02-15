#include "GraphicsObjects.h"

namespace editor {
    GraphicsObject::GraphicsObject(double width, QString stroke, QString fill)
        :   stroke_width_(width), 
            stroke_color_(stroke), 
            fill_color_(fill) {}
}