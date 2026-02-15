#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace editor {
    enum class ToolType{
        SELECTION, RECTANGLE, CIRCLE, HEXAGON, 
        ROUNDED_RECTANGLE, TEXT, FREEHAND, LINE, NONE
    };

    enum class HandleType{
        HANDLE_NONE,
        HANDLE_TOP_LEFT,
        HANDLE_TOP_RIGHT,
        HANDLE_BOTTOM_LEFT,
        HANDLE_BOTTOM_RIGHT,
        HANDLE_RADIAL
    };
}

#endif