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


// ============================================================================
// Headers include section
// ============================================================================

// Related header
#include "MainWindow.h"
#include "ui_MainWindow.h"

// "C" system headers

// Standard Library headers

// External libraries headers
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
}

// ----------------------------------------------------------------------------
// MainWindow::slotExit
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
void MainWindow::slotExit()
{
    qApp->exit();
}

// ----------------------------------------------------------------------------
// MainWindow::Render
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
void MainWindow::Render()
{
    this->ui->mainview->renderWindow()->Render();
}