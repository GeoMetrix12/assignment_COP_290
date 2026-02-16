# Vector Graphics Editor

A C++ based vector graphics editor built using the Qt 6 framework. This application allows users to create, edit, and save vector shapes (SVG format) with a full Undo/Redo system.

## Features

* **Shape Creation**: Create Rectangles, Rounded Rectangles, Circles, Hexagons, Lines, Freehand drawings, and Text.
* **Editing**: Select, Move, Resize, and Delete shapes.
* **Properties**: Change Stroke Color, Fill Color, Stroke Width, and Font Size.
* **Undo/Redo**: Unlimited history for all actions.
* **Clipboard**: Copy, Cut, and Paste support.
* **File Support**: Load and Save drawings in SVG format.
* **Dynamic UI**: The toolbar updates automatically to show the properties of the selected object (for example, disabling "Stroke Width" when Text is selected).

## Build Instructions

### Prerequisites
* **C++ Compiler**: Needs to support C++17 (GCC, Clang, or MSVC).
* **CMake**: Version 3.16 or higher.
* **Qt 6**: Core, Gui, and Widgets modules.

### Compilation Steps

1.  **Unzip the project**:
    Extract the submitted zip file to a folder on your computer.

2.  **Open a terminal**:
    Navigate to the extracted folder.
    ```bash
    cd path/to/extracted/folder
    ```

3.  **Create a build directory**:
    ```bash
    mkdir build && cd build
    ```

4.  **Configure with CMake**:
    If Qt is not found automatically, you might need to tell CMake where it is.
    ```bash
    cmake ..
    ```

5.  **Build**:
    ```bash
    make
    ```

6.  **Run**:
    ```bash
    ./vector_editor
    ```

## Design Decisions

### 1. Core Architecture
The application is split into two main parts to keep the code clean:
* **MainWindow**: This manages the window, menus, and toolbars. It handles user inputs like clicking "Save" or changing a color, then passes those commands to the Canvas.
* **Canvas**: This is the drawing area. It handles all mouse clicks and drawing logic. It stores the list of shapes and knows which tool is currently active.

### 2. How Shapes Work (Polymorphism)
We use a common base class called `GraphicsObject`. This allows the Canvas to treat all shapes (Circles, Lines, Hexagons) the same way.
* **Base Class**: `GraphicsObject` defines common actions like `Draw`, `moveBy`, and `SvgConvert`.
* **Specific Shapes**:
    * **Text**: Handles resizing differently by changing font size instead of stretching.
    * **Hexagon**: Uses math (trigonometry) to draw a 6-sided polygon.
    * **RoundedRectangle**: A special rectangle that allows changing the corner radius.

### 3. Undo/Redo System (Command Pattern)
To make Undo/Redo work reliably, we do not change shapes directly. Instead, we create "Command" objects.
* **Command Objects**: We have commands for `AddShape`, `DeleteShape`, `MoveShape`, and `PropertyChange`.
* **Stacks**: When you do an action, a Command is pushed onto an "Undo Stack." When you press Undo, we pop the command and reverse it. This allows for infinite undo history.

### 4. Saving and Loading
We wrote a custom parser for SVG files instead of using a heavy external library.
* **Parsing**: The code reads the file line-by-line, looks for tags like `<rect>` or `<circle>`, and creates the corresponding C++ objects.
* **Saving**: The editor loops through all shapes and asks them to convert themselves into an SVG string.

### 5. UI Updates
The interface changes based on what you select.
* **Canvas to UI**: When you click a shape, the Canvas sends a signal to the MainWindow with the shape's details. The MainWindow then updates the spin boxes and color buttons to match.
* **Blocking Signals**: When updating the UI from the Canvas, we temporarily block signals to prevent a loop where the UI tries to update the Canvas back immediately.

## File Structure

All source files are located in the main project directory:

* **Core Application**:
    * `main.cpp`: Application entry point.
    * `MainWindow.cpp` / `MainWindow.h`: The main window, menus, and toolbar logic.
    * `Canvas.cpp` / `Canvas.h`: The drawing surface, mouse event handling, and tool logic.
    * `Constants.h`: Global definitions for Tool types and Handle types.

* **Data Models & Shapes**:
    * `GraphicsObjects.cpp` / `GraphicsObjects.h`: The base class for all shapes.
    * `Rectangle.cpp` / `Rectangle.h`
    * `RoundedRectangle.cpp` / `RoundedRectangle.h`
    * `Circle.cpp` / `Circle.h`
    * `Hexagon.cpp` / `Hexagon.h`
    * `Line.cpp` / `Line.h`
    * `Text.cpp` / `Text.h`
    * `Freehand.cpp` / `Freehand.h`

* **Logic Modules**:
    * `Command.h`: Base interface for the Undo/Redo commands.
    * `Commands.cpp` / `Commands.h`: Implementation of specific commands (Add, Move, Resize, Delete, PropertyChange).
    * `SvgParser.cpp` / `SvgParser.h`: Logic for parsing and saving SVG files.