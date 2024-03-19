// ============================================================================
// MainWindow.h - Creates a main window and sets up a VTK renderer
//
//  Copyright (C) <yyyy> <Author Name> <author@mail.com>
//
// This file is part of QtVTKFramework.
// 
// QtVTKFramework is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
// 
// QtVTKFramework is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
// 
// You should have received a copy of the GNU General Public License along with
// QtVTKFramework. If not, see <https://www.gnu.org/licenses/>.
// ============================================================================


// ============================================================================
//
// 2024-03-18 Ljubomir Kurij <ljubomir_kurij@protonmail.com>
//
// * MainWindow.h: created.
//
// ============================================================================


#ifndef MainWindow_H
#define MainWindow_H

// ============================================================================
// Headers include section
// ============================================================================

// "C" system headers

// Standard Library headers

// External libraries headers
#include <QPointer>
#include <QMainWindow>
#include <QVTKOpenGLNativeWidget.h>


// Forward Qt class declarations
class Ui_MainWindow;


// ============================================================================
// Class Definitions Section
// ============================================================================

// ----------------------------------------------------------------------------
// MainWindow
// ----------------------------------------------------------------------------
//
// Description: A subclass of QMainWindow that sets up a VTK renderer
//
// Properties:
// - render_widget: A QVTKOpenGLNativeWidget that holds the VTK renderer
//
// Methods:
// - MainWindow: Constructor
// - ~MainWindow: Destructor
//
// Signals:
// - None
//
// Slots:
// - slotExit: Exits the application
// - Render: Renders the VTK scene
//
// Example usage:
//   QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
//   QApplication app(argc, argv);
//   MainWindow* window = new MainWindow(argc, argv);
//   window->show();
//   return app.exec();
//
// ----------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
    // Constructor/Destructor
    MainWindow(int argc, char* argv[]);
    ~MainWindow() override {}

public Q_SLOTS:
    
        virtual void slotExit();  // Exits the application
        virtual void Render();  // Renders the VTK scene

protected:
    QPointer<QVTKOpenGLNativeWidget> render_widget; // Holds the VTK renderer

private:
    // Designer form
    Ui_MainWindow* ui;
};

#endif  // MainWindow_H