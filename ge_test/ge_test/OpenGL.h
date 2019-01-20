#pragma once

#include <windows.h>

#pragma comment(lib, "libs/glew32.lib")
#pragma comment(lib, "opengl32.lib")

#include "includes/GL/glew.h"
#include "engine/Grid.h"
#include "engine/Shader.h"
#include "engine/SceneCamera.h"

#include "includes/glm/glm.hpp"

using namespace System::Windows::Forms;

namespace OpenGLForm
{
	public ref class COpenGL : public System::Windows::Forms::NativeWindow
	{

	private:
		CreateParams^ cp;
		System::Windows::Forms::Panel^ parentForm;
		Shader* shader;
		Grid* grid;
		SceneCamera* sceneCamera;
		
	public:
		COpenGL(System::Windows::Forms::Panel ^ parentForm)
		{
			this->parentForm = parentForm;

			init();

			shader = new Shader("shaders/mainShadervs.glsl", "shaders/mainShaderfs.glsl");
			shader->Use();
			

			grid = new Grid(100, 0.1, shader->getProgramID());
			sceneCamera = new SceneCamera(glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

			shader->setMat4("projectionMatrix", glm::perspective(30.0f, (float)(parentForm->Width / parentForm->Height), 0.1f, 1000.0f));
			shader->setMat4("viewMatrix", sceneCamera->getViewMatrix());
			
		}

		void init()
		{
			cp = gcnew CreateParams;
			GLsizei iWidth = parentForm->Width, iHeight = parentForm->Height;
			// Set the position on the form
			cp->X = 0;
			cp->Y = 0;
			cp->Height = iHeight;
			cp->Width = iWidth;

			// Specify the form as the parent.
			cp->Parent = parentForm->Handle;

			// Create as a child of the specified parent and make OpenGL compliant (no clipping)
			cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

			// Create the actual window
			this->DestroyHandle();
			this->CreateHandle(cp);

			m_hDC = GetDC((HWND)this->Handle.ToPointer());

			if (m_hDC)
			{
				MySetPixelFormat(m_hDC);
				glViewport(0, 0, iWidth, iHeight);
				InitGL();
			}
		}

		System::Void Render(System::Void)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear screen and depth buffer
			
			grid->Draw();
		}

		System::Void SwapOpenGLBuffers(System::Void)
		{
			SwapBuffers(m_hDC);
		}

	private:
		HDC m_hDC;
		HGLRC m_hglrc;

	protected:
		~COpenGL(System::Void)
		{
			this->DestroyHandle();
		}

		GLint MySetPixelFormat(HDC hdc)
		{
			static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
			{
				sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
				1,											// Version Number
				PFD_DRAW_TO_WINDOW |						// Format Must Support Window
				PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
				PFD_DOUBLEBUFFER,							// Must Support Double Buffering
				PFD_TYPE_RGBA,								// Request An RGBA Format
				16,										// Select Our Color Depth
				0, 0, 0, 0, 0, 0,							// Color Bits Ignored
				0,											// No Alpha Buffer
				0,											// Shift Bit Ignored
				0,											// No Accumulation Buffer
				0, 0, 0, 0,									// Accumulation Bits Ignored
				16,											// 16Bit Z-Buffer (Depth Buffer)  
				0,											// No Stencil Buffer
				0,											// No Auxiliary Buffer
				PFD_MAIN_PLANE,								// Main Drawing Layer
				0,											// Reserved
				0, 0, 0										// Layer Masks Ignored
			};

			GLint  iPixelFormat;

			// get the device context's best, available pixel format match 
			if ((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
			{
				MessageBox::Show("ChoosePixelFormat Failed");
				return 0;
			}

			// make that match the device context's current pixel format 
			if (SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
			{
				MessageBox::Show("SetPixelFormat Failed");
				return 0;
			}

			if ((m_hglrc = wglCreateContext(m_hDC)) == NULL)
			{
				MessageBox::Show("wglCreateContext Failed");
				return 0;
			}

			if ((wglMakeCurrent(m_hDC, m_hglrc)) == NULL)
			{
				MessageBox::Show("wglMakeCurrent Failed");
				return 0;
			}


			return 1;
		}

		bool InitGL(GLvoid)										// All setup for opengl goes here
		{
			glShadeModel(GL_SMOOTH);							// Enable smooth shading
			glClearColor(0.6f, 0.6f, 0.6f, 0.5f);				// Black background
			glClearDepth(1.0f);									// Depth buffer setup
			glEnable(GL_DEPTH_TEST  | GL_DEBUG_OUTPUT);							// Enables depth testing
			glDepthFunc(GL_LEQUAL);								// The type of depth testing to do
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations
			return TRUE;										// Initialisation went ok
		}

		public: GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize and initialise the gl window
		{
			if (height == 0)										// Prevent A Divide By Zero By
				height = 1;										// Making Height Equal One

			shader->setMat4("projectionMatrix", glm::perspective(30.0f, (float)(width / height), 0.1f, 1000.0f));

			glViewport(0, 0, width, height);						// Reset The Current Viewport
		}
	};
}