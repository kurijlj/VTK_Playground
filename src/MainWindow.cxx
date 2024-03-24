// ============================================================================
// MainWindow.cpp - Implementation of the MainWindow class
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
// * MainWindow.cpp: created.
//
// ============================================================================


// ============================================================================
//
// References (this section should be deleted in the release version)
//
// * MinimalQtVTKApp example from VTK examples page at
//   <https://examples.vtk.org/site/Cxx/Qt/MinimalQtVTKApp/
//
// * Render a Cone example from VTK book at
//   <https://book.vtk.org/en/latest/VTKBook/03Chapter3.html>
//
// ============================================================================


#define _USE_MATH_DEFINES


// ============================================================================
// Headers include section
// ============================================================================

// Related header -------------------------------------------------------------
#include "MainWindow.h"
#include "ui_MainWindow.h"

// "C" system headers ---------------------------------------------------------

// Standard Library headers ---------------------------------------------------
#include <cmath>

// External libraries headers -------------------------------------------------

// VTK headers
#include <vtkSmartPointer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkConeSource.h>
#include <vtkColor.h>
#include <vtkNamedColors.h>
#include <vtkCamera.h>

// Qt headers
#include <QAction>
#include <QChar>
#include <QKeySequence>
#include <QtWidgets>


// ============================================================================
// Constructor/Destructor Section
// ============================================================================

// ----------------------------------------------------------------------------
// MainWindow::MainWindow
// ----------------------------------------------------------------------------
//
// Description: Constructor
//
// Inputs:
// - argc: Number of command line arguments
// - argv: Array of command line arguments
//
// Outputs: None
//
// Returns: None
//
// Side Effects: None
//
// ----------------------------------------------------------------------------
MainWindow::MainWindow(int argc, char* argv[])
{
    this->ui = new Ui_MainWindow;
    this->ui->setupUi(this);

    // Initialize menu bar and actions ----------------------------------------
    this->ui->actionExit->setShortcuts(QKeySequence::Quit);
    // connect(
    //     this->ui->actionExit,
    //     &QAction::triggered,
    //     this,
    //     &MainWindow::slotExit
    //     );

    // Initialize the VTK scene -----------------------------------------------

    //Create a cone
    auto coneSource = vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetHeight(3.0);
    coneSource->SetRadius(1.5);
    coneSource->SetResolution(40);
    coneSource->Update();

    //Create a mapper and actor
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(coneSource->GetOutputPort());

    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetDiffuseColor(
        // Demonstrate how to use the named colors
        colors->GetColor3d("bisque").GetData()
        );

    //Create a renderer, render window, and interactor
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    this->render_widget = new QVTKOpenGLNativeWidget();
    vtkNew<vtkGenericOpenGLRenderWindow> render_window;
    this->render_widget->setRenderWindow(render_window.Get());
    this->ui->mainview->setRenderWindow(render_window.Get());
    render_window->AddRenderer(renderer);

    //Add the actors to the scene
    renderer->AddActor(actor);
    renderer->SetBackground(
        // Demonstrate how to use custom colors. The background is a dark
        // slate gray. We pass the relative intensity of each color in the
        // range 0 to 1.
        vtkColor3d(7.0/255.0, 54.0/255.0, 66.0/255.0).GetData()
        );
    
    // Connect the VTK events to the Qt slots
    this->vtk_event_connect = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    this->vtk_event_connect->Connect(
        renderer,
        vtkCommand::EndEvent,
        this,
        SLOT(dispatchRendererEvent(vtkObject*, unsigned long, void*))
        );

    // Set the camera position
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30.0);
    renderer->GetActiveCamera()->Elevation(30.0);

    // Initialize the status bar ----------------------------------------------
    this->ui->statusbar->showMessage("Ready");
}

// ----------------------------------------------------------------------------
// MainWindow::dispatchRendererEvent
// ----------------------------------------------------------------------------
//
// Description: Registers the VTK events from the renderer object
//
// Inputs:
// - caller: The object that triggered the event
// - vtk_event: The event that was triggered
// - client_data: The data associated with the event
//
// Outputs: None
//
// Returns: None
//
// Side Effects: None
//
// ----------------------------------------------------------------------------
void MainWindow::dispatchRendererEvent(
    vtkObject* caller,
    unsigned long vtk_event,
    void* client_data
    )
{
    // Note the use of reinterpret_cast to cast the caller to the expected type.
    auto renderer = reinterpret_cast<vtkRenderer*>(caller);

    // Calculate the azimuth and elevation of the camera
    double azimuth = asin(
        - renderer->GetActiveCamera()->GetDirectionOfProjection()[1]
        ) * 180.0 / M_PI;
    double elevation = acos(
        - renderer->GetActiveCamera()->GetDirectionOfProjection()[2]
        / cos(azimuth * M_PI / 180.0)
        ) * 180.0 / M_PI;
    
    // Display the camera position in the status bar
    QString str_azimuth = QString("azimuth: %1%2")
        .arg(azimuth, 6, 'f', 1)
        .arg(QChar(0x00B0));
    QString str_elevation = QString("elevation: %1%2")
        .arg(elevation, 6, 'f', 1)
        .arg(QChar(0x00B0));
    QString str_roll = QString("roll: %1%2")
        .arg(renderer->GetActiveCamera()->GetRoll(), 6, 'f', 1)
        .arg(QChar(0x00B0));
    QString str_zoom = QString("distance: %1").arg(
        renderer->GetActiveCamera()->GetDistance(), 6, 'f', 1
        );
    this->statusMessage(
        QString("Camera ")
        + str_azimuth + " / "
        + str_elevation + " / "
        + str_roll + " / "
        + str_zoom
        );
}

// ----------------------------------------------------------------------------
// MainWindow::statusMessage
// ----------------------------------------------------------------------------
//
// Description: Displays a status message in the status bar
//
// Inputs:
// - message: The message to display
//
// Outputs: None
//
// Returns: None
//
// Side Effects: None
//
// ----------------------------------------------------------------------------
void MainWindow::statusMessage(const QString& message)
{
    this->ui->statusbar->showMessage(message);
}

// ----------------------------------------------------------------------------
// MainWindow::render
// ----------------------------------------------------------------------------
//
// Description: Renders the VTK scene
//
// Inputs: None
//
// Outputs: None
//
// Returns: None
//
// Side Effects: None
//
// ----------------------------------------------------------------------------
void MainWindow::render()
{
    this->ui->mainview->renderWindow()->Render();
}

// ----------------------------------------------------------------------------
// MainWindow::about
// ----------------------------------------------------------------------------
//
// Description: Displays the about dialog
//
// Inputs: None
//
// Outputs: None
//
// Returns: None
//
// Side Effects: None
//
// ----------------------------------------------------------------------------
void MainWindow::about()
{
    QMessageBox::about(
        this,
        tr("About the Qt VTK Framework"),
        tr("A lightweight C++ framework for creating graphical applications "
        "using the <a href='https://vtk.org/'>Visualization Toolkit</a> (VTK) "
        "for rendering and <a href='https://www.qt.io/product/framework'>Qt "
        "Framework</a> for the user interface."
        "<br><br>"
        "<b>Features:</b><br>"
        "<ul>"
        "<li>UI creation with Qt Designer for a streamlined development "
        "process.</li>"
        "<li>Basic \"Main Window\" application structure with an about dialog."
        "</li>"
        "<li>Eye-catching <a href='https://icons8.com/icons/isometric'>"
        "Isometric icons</a> from <a href='https://icons8.com'>Icons8</a> for "
        "a polished look.</li>"
        "<li>Simple VTK rendering scene with standard event interaction "
        "(camera control with mouse).</li>"
        "<li>Seamless integration of VTK events with Qt slots using "
        "<code>vtkEventQtSlotConnect</code> for responsive applications.</li>"
        "<li>Efficient linking of graphical resources (icons, etc.) with the "
        "executable using CMake's <code>qt5_add_resources</code> command.</li>"
        "<li>Streamlined post-build resource copying using CMake's "
        "<code>add_custom_command</code> for a user-friendly experience "
        "(system-agnostic).</li>"
        "</ul>"
        "<br>"
        "<b>Current Limitations:</b><br>"
        "<ul>"
        "<li>Keyboard shortcuts are not yet implemented.</li>"
        "</ul>"
        "<br><br>"
        "<b>License Notice</b><br>"
        "This program is free software: you can redistribute it and/or "
        "modify<br>"
        "it under the terms of the GNU General Public License as "
        "published by<br>"
        "the Free Software Foundation, either version 3 of the License, "
        "or<br>"
        "(at your option) any later version."
        "<br><br>"
        "This program is distributed in the hope that it will be "
        "useful,<br>"
        "but WITHOUT ANY WARRANTY; without even the implied warranty "
        "of<br>"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See "
        "the<br>"
        "GNU General Public License for more details."
        "<br><br>"
        "You should have received a copy of the GNU General Public "
        "License<br>"
        "along with this program.  If not, see "
        "<a href='https://www.gnu.org/licenses/'>"
        "https://www.gnu.org/licenses/</a>."
        )
    );
}

// ----------------------------------------------------------------------------
// MainWindow::close
// ----------------------------------------------------------------------------
//
// Description: Exits the application
//
// Inputs: None
//
// Outputs: None
//
// Returns: None
//
// Side Effects: None
//
// ----------------------------------------------------------------------------
void MainWindow::close()
{
    qApp->exit();
}