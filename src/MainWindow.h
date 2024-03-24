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
#include <vtkEventQtSlotConnect.h>


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
// - dispatchRendererEvent: Registers the VTK events from the renderer object
// - statusMessage: Updates a status message in the status bar
// - Render: Renders the VTK scene
// - close: Exits the application
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

private Q_SLOTS:
        virtual void dispatchRendererEvent(
            vtkObject* caller,
            unsigned long vtk_event,
            void* client_data
            );  // Registers the VTK end event
        virtual void render();  // Renders the VTK scene
        virtual void about();  // Displays the about dialog
        virtual void close();  // Exits the application

public Q_SLOTS:
        virtual void statusMessage(
            const QString& message
            );  // Displays a status message

protected:
    QPointer<QVTKOpenGLNativeWidget> render_widget; // Holds the VTK renderer

private:
    // Designer form
    Ui_MainWindow* ui;
    vtkSmartPointer<vtkEventQtSlotConnect> vtk_event_connect;
};

#endif  // MainWindow_H