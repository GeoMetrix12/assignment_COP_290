# Vector Graphics Editor

A C++ vector graphics editor built using the Qt 6 framework. This application allows users to create, edit, and save vector shapes in SVG format. It features a complete Undo/Redo system and an organized, modular code structure.

## Features

* **Shape Creation**: Create Rectangles, Rounded Rectangles, Circles, Hexagons, Lines, Freehand drawings, and Text.
* **Editing Tools**: Select, Move, Resize, and Delete shapes.
* **Context-Aware Interface**:
    * **Dynamic Toolbar**: The toolbar updates automatically based on the selected object. For example, "Fill Color" is disabled when Text is selected, and "Font Size" only appears for Text objects.
    * **Property Persistence**: The editor remembers the last used settings (such as color and stroke width) for new shapes.
* **Properties**: Adjust Stroke Color, Fill Color, Stroke Width, and Font Size in real-time.
* **Undo/Redo**: Supports unlimited undo and redo operations for all actions.
* **Clipboard**: Support for Copy, Cut, and Paste operations.
* **File Support**: Native support for loading and saving files in the standard SVG format.

---

## Build Instructions

### Prerequisites
* **C++ Compiler**: Must support C++17 (GCC, Clang, or MSVC).
* **CMake**: Version 3.16 or higher.
* **Qt 6**: Core, Gui, and Widgets modules.

### Compilation Steps

1.  **Unzip the project**:
    Extract the source code to a folder on your computer.

2.  **Open a terminal**:
    Navigate to the project directory.
    ```bash
    cd path/to/project
    ```

3.  **Create a build directory**:
    ```bash
    mkdir build && cd build
    ```

4.  **Configure with CMake**:
    ```bash
    cmake ..
    ```
    *(Note: If Qt is not found automatically, you may need to specify the path to your Qt installation).*

5.  **Build**:
    ```bash
    make
    ```

6.  **Run**:
    ```bash
    ./vector_editor
    ```

---

## Design Decisions

### 1. Code Architecture
The application code is divided into logical modules to keep it organized:
* **MainWindow**: Manages the application window, menus, and toolbar. It handles user interface interactions and passes commands to the Canvas.
* **Canvas**: The central drawing area. It handles mouse events (clicking, dragging), renders the shapes, and manages the list of objects.

### 2. Shape Handling (Polymorphism)
All shapes inherit from a common base class called `GraphicsObject`. This allows the Canvas to treat different shapes (Circles, Lines, Hexagons) uniformly.
* **Base Class**: Defines common functions like `Draw`, `moveBy`, and `SvgConvert`.
* **Specific Implementations**: Each shape class implements its own drawing logic. For example, the Hexagon class uses trigonometry to calculate vertex positions, while the Text class handles font sizing.

### 3. Undo/Redo System (Command Pattern)
The application uses the **Command Pattern** to manage history. Actions are not applied directly; instead, they are encapsulated in "Command" objects (e.g., `AddShapeCommand`, `MoveShapeCommand`).
* **Execution**: When a user performs an action, a Command object is created and stored in a stack.
* **Reversibility**: Every command includes logic to reverse itself, allowing the user to return to the exact previous state.

### 4. Copy/Paste System (Prototype Pattern)
To implement the Clipboard, the project uses the **Prototype Pattern**.
* **Cloning**: Every shape class implements a `cloneShape()` method. This allows the editor to create an exact independent copy of any selected object without knowing its specific type (Circle, Rect, etc.) at compile time.
* **Isolation**: The copied shape is stored in a temporary buffer and duplicated again upon pasting, ensuring the clipboard content remains safe from subsequent edits.

### 5. Direct Manipulation & Hit Testing
Selecting objects requires mathematical "Hit Testing" rather than simple bounding boxes.
* **Precise Selection**: The `containsPoint()` method uses specific math for each shape.
    * **Circles**: Uses the distance formula to ignore clicks in the empty corners of the bounding box.
    * **Lines**: Uses a tolerance threshold to detect clicks near the line segment.
* **Handle Logic**: The editor distinguishes between clicking *inside* a shape (to move it) and clicking *on a handle* (to resize it), providing a professional editing experience.

### 6. Parametric Shape Editing
Certain shapes support "Smart Properties" that go beyond simple resizing.
* **Rounded Rectangles**: These feature a specialized control handle that allows the user to adjust the corner radius interactively by dragging, separate from the width/height resizing.

### 7. Visual Feedback (Ghost Rendering)
To improve the user experience, the application uses a "Ghost Rendering" technique during shape creation.
* **Temporary State**: When a user drags the mouse to draw a shape, it appears semi-transparent (faded).
* **Finalization**: The shape becomes fully visible and solid only when the user releases the mouse button.
* **Purpose**: This visual cue helps users distinguish between the action they are currently performing and the shapes that have already been finalized on the canvas.

### 8. Custom SVG Parser
The project includes a custom parser to handle SVG files without relying on external libraries.
* **Parsing**: The parser reads the SVG file line-by-line and reconstructs C++ objects from the XML tags.
* **Saving**: To save a file, the editor iterates through all shapes and converts them into standard SVG text strings.

--

## File Structure

The source code is organized as follows:

### Application Core
* `src/main.cpp`: The entry point of the program.
* `src/MainWindow.h`: The main window header file.
* `src/MainWindow_core.cpp`: Constructor and window setup.
* `src/MainWindow_Actions.cpp`: Creation of menu actions (New, Save, etc.).
* `src/MainWindow_Toolbar.cpp`: Toolbar layout and tool selection logic.
* `src/MainWindow_slots.cpp`: Functions for file dialogs and property updates.

### Canvas & Input
* `src/Canvas.h`: The main drawing surface header.
* `src/Canvas_core.cpp`: Initialization and painting logic.
* `src/Canvas_MouseMove.cpp`: Logic for dragging and drawing.
* `src/Canvas_MousePress.cpp`: Logic for selecting and starting shapes.
* `src/Canvas_MouseRelease.cpp`: Logic for finishing actions and committing commands.
* `src/Canvas_Edit.cpp`: Logic for Undo, Redo, Copy, and Paste.
* `src/Canvas_File.cpp`: Logic for loading and saving files.
* `src/Canvas_Properties.cpp`: Functions for changing color, width, and fonts.

### Shapes (Model)
* `src/GraphicsObjects.h`: The abstract base class header.
* `src/GraphicObjects.cpp`: Base class implementation.
* `src/Circle.h` & `src/Circle.cpp`
* `src/Freehand.h` & `src/Freehand.cpp`
* `src/Hexagon.h` & `src/Hexagon.cpp`
* `src/Line.h` & `src/Line.cpp`
* `src/Rectangle.h` & `src/Rectangle.cpp`
* `src/RoundedRectangle.h` & `src/RoundedRectangle.cpp`
* `src/Text.h` & `src/Text.cpp`

### Commands (Undo/Redo)
* `src/Command.h`: Base interface for commands.
* `src/Commands.h`: Header for specific command classes.
* `src/Commands.cpp`: Implementation of Add, Move, Delete, and Property commands.
* `src/ResizeCommand.cpp`: Specific logic for resizing different shapes.

### Utilities & Parsing
* `src/Constants.h`: Global definitions for Tool types and Handle types.
* `src/SvgParser.h`: Header for the SVG parser.
* `src/SvgParser_Parse.cpp`: Logic for reading SVG files.
* `src/SvgParser_Write.cpp`: Logic for writing SVG files.