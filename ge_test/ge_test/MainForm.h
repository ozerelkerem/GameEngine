#pragma once

#include "OpenGL.h"

namespace getest {
	
	
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  panel_top;
	private: System::Windows::Forms::Splitter^  splitter1;
	private: System::Windows::Forms::Panel^  panel_right;
	private: System::Windows::Forms::Panel^  panel_properties;
	private: System::Windows::Forms::Splitter^  splitter3;
	private: System::Windows::Forms::Panel^  panel_worldOutliner;
	private: System::Windows::Forms::Panel^  panel_left;
	private: System::Windows::Forms::Splitter^  splitter5;
	private: System::Windows::Forms::Panel^  panel_scene;
	private: System::Windows::Forms::Panel^  panel_mode;
	private: System::Windows::Forms::Splitter^  splitter4;
	private: System::Windows::Forms::Panel^  panel_browser;
	private: System::Windows::Forms::Splitter^  splitter2;
	private: System::Windows::Forms::Panel^  panel_sceneRender;
	private: System::Windows::Forms::Panel^  panel_sceneSettings;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		OpenGLForm::COpenGL^ OGLForm;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->panel_top = (gcnew System::Windows::Forms::Panel());
			this->splitter1 = (gcnew System::Windows::Forms::Splitter());
			this->panel_right = (gcnew System::Windows::Forms::Panel());
			this->panel_properties = (gcnew System::Windows::Forms::Panel());
			this->splitter3 = (gcnew System::Windows::Forms::Splitter());
			this->panel_worldOutliner = (gcnew System::Windows::Forms::Panel());
			this->panel_left = (gcnew System::Windows::Forms::Panel());
			this->splitter5 = (gcnew System::Windows::Forms::Splitter());
			this->panel_scene = (gcnew System::Windows::Forms::Panel());
			this->panel_sceneRender = (gcnew System::Windows::Forms::Panel());
			this->panel_sceneSettings = (gcnew System::Windows::Forms::Panel());
			this->panel_mode = (gcnew System::Windows::Forms::Panel());
			this->splitter4 = (gcnew System::Windows::Forms::Splitter());
			this->panel_browser = (gcnew System::Windows::Forms::Panel());
			this->splitter2 = (gcnew System::Windows::Forms::Splitter());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->panel_right->SuspendLayout();
			this->panel_left->SuspendLayout();
			this->panel_scene->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel_top
			// 
			this->panel_top->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->panel_top->Dock = System::Windows::Forms::DockStyle::Top;
			this->panel_top->Location = System::Drawing::Point(0, 0);
			this->panel_top->Name = L"panel_top";
			this->panel_top->Size = System::Drawing::Size(1222, 32);
			this->panel_top->TabIndex = 0;
			// 
			// splitter1
			// 
			this->splitter1->Dock = System::Windows::Forms::DockStyle::Top;
			this->splitter1->Location = System::Drawing::Point(0, 32);
			this->splitter1->Name = L"splitter1";
			this->splitter1->Size = System::Drawing::Size(1222, 3);
			this->splitter1->TabIndex = 1;
			this->splitter1->TabStop = false;
			// 
			// panel_right
			// 
			this->panel_right->Controls->Add(this->panel_properties);
			this->panel_right->Controls->Add(this->splitter3);
			this->panel_right->Controls->Add(this->panel_worldOutliner);
			this->panel_right->Dock = System::Windows::Forms::DockStyle::Right;
			this->panel_right->Location = System::Drawing::Point(1022, 35);
			this->panel_right->Name = L"panel_right";
			this->panel_right->Size = System::Drawing::Size(200, 541);
			this->panel_right->TabIndex = 2;
			// 
			// panel_properties
			// 
			this->panel_properties->BackColor = System::Drawing::Color::Maroon;
			this->panel_properties->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel_properties->Location = System::Drawing::Point(0, 103);
			this->panel_properties->Name = L"panel_properties";
			this->panel_properties->Size = System::Drawing::Size(200, 438);
			this->panel_properties->TabIndex = 2;
			// 
			// splitter3
			// 
			this->splitter3->Dock = System::Windows::Forms::DockStyle::Top;
			this->splitter3->Location = System::Drawing::Point(0, 100);
			this->splitter3->Name = L"splitter3";
			this->splitter3->Size = System::Drawing::Size(200, 3);
			this->splitter3->TabIndex = 1;
			this->splitter3->TabStop = false;
			// 
			// panel_worldOutliner
			// 
			this->panel_worldOutliner->BackColor = System::Drawing::SystemColors::Highlight;
			this->panel_worldOutliner->Dock = System::Windows::Forms::DockStyle::Top;
			this->panel_worldOutliner->Location = System::Drawing::Point(0, 0);
			this->panel_worldOutliner->Name = L"panel_worldOutliner";
			this->panel_worldOutliner->Size = System::Drawing::Size(200, 100);
			this->panel_worldOutliner->TabIndex = 0;
			// 
			// panel_left
			// 
			this->panel_left->Controls->Add(this->splitter5);
			this->panel_left->Controls->Add(this->panel_scene);
			this->panel_left->Controls->Add(this->panel_mode);
			this->panel_left->Controls->Add(this->splitter4);
			this->panel_left->Controls->Add(this->panel_browser);
			this->panel_left->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel_left->Location = System::Drawing::Point(0, 35);
			this->panel_left->Name = L"panel_left";
			this->panel_left->Size = System::Drawing::Size(1022, 541);
			this->panel_left->TabIndex = 3;
			// 
			// splitter5
			// 
			this->splitter5->Location = System::Drawing::Point(152, 0);
			this->splitter5->Name = L"splitter5";
			this->splitter5->Size = System::Drawing::Size(3, 332);
			this->splitter5->TabIndex = 4;
			this->splitter5->TabStop = false;
			// 
			// panel_scene
			// 
			this->panel_scene->BackColor = System::Drawing::SystemColors::MenuHighlight;
			this->panel_scene->Controls->Add(this->panel_sceneRender);
			this->panel_scene->Controls->Add(this->panel_sceneSettings);
			this->panel_scene->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel_scene->Location = System::Drawing::Point(152, 0);
			this->panel_scene->Name = L"panel_scene";
			this->panel_scene->Size = System::Drawing::Size(870, 332);
			this->panel_scene->TabIndex = 3;
			// 
			// panel_sceneRender
			// 
			this->panel_sceneRender->AccessibleName = L"";
			this->panel_sceneRender->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->panel_sceneRender->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel_sceneRender->Location = System::Drawing::Point(0, 50);
			this->panel_sceneRender->Name = L"panel_sceneRender";
			this->panel_sceneRender->Size = System::Drawing::Size(870, 282);
			this->panel_sceneRender->TabIndex = 0;
			this->panel_sceneRender->SizeChanged += gcnew System::EventHandler(this, &MainForm::panel_sceneRender_SizeChanged);
			// 
			// panel_sceneSettings
			// 
			this->panel_sceneSettings->AccessibleName = L"";
			this->panel_sceneSettings->Dock = System::Windows::Forms::DockStyle::Top;
			this->panel_sceneSettings->Location = System::Drawing::Point(0, 0);
			this->panel_sceneSettings->MaximumSize = System::Drawing::Size(0, 50);
			this->panel_sceneSettings->MinimumSize = System::Drawing::Size(0, 50);
			this->panel_sceneSettings->Name = L"panel_sceneSettings";
			this->panel_sceneSettings->Size = System::Drawing::Size(870, 50);
			this->panel_sceneSettings->TabIndex = 0;
			// 
			// panel_mode
			// 
			this->panel_mode->BackColor = System::Drawing::Color::DarkOrange;
			this->panel_mode->Dock = System::Windows::Forms::DockStyle::Left;
			this->panel_mode->Location = System::Drawing::Point(0, 0);
			this->panel_mode->Name = L"panel_mode";
			this->panel_mode->Size = System::Drawing::Size(152, 332);
			this->panel_mode->TabIndex = 2;
			// 
			// splitter4
			// 
			this->splitter4->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->splitter4->Location = System::Drawing::Point(0, 332);
			this->splitter4->Name = L"splitter4";
			this->splitter4->Size = System::Drawing::Size(1022, 3);
			this->splitter4->TabIndex = 1;
			this->splitter4->TabStop = false;
			// 
			// panel_browser
			// 
			this->panel_browser->BackColor = System::Drawing::Color::GreenYellow;
			this->panel_browser->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->panel_browser->Location = System::Drawing::Point(0, 335);
			this->panel_browser->Name = L"panel_browser";
			this->panel_browser->Size = System::Drawing::Size(1022, 206);
			this->panel_browser->TabIndex = 0;
			// 
			// splitter2
			// 
			this->splitter2->Dock = System::Windows::Forms::DockStyle::Right;
			this->splitter2->Location = System::Drawing::Point(1019, 35);
			this->splitter2->Name = L"splitter2";
			this->splitter2->Size = System::Drawing::Size(3, 541);
			this->splitter2->TabIndex = 4;
			this->splitter2->TabStop = false;
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &MainForm::timer1_Tick);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1222, 576);
			this->Controls->Add(this->splitter2);
			this->Controls->Add(this->panel_left);
			this->Controls->Add(this->panel_right);
			this->Controls->Add(this->splitter1);
			this->Controls->Add(this->panel_top);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->panel_right->ResumeLayout(false);
			this->panel_left->ResumeLayout(false);
			this->panel_scene->ResumeLayout(false);
			this->ResumeLayout(false);
		}
#pragma endregion
	private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) {
		OGLForm = gcnew OpenGLForm::COpenGL(this->panel_sceneRender);
	}
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
		OGLForm->Render();
		OGLForm->SwapOpenGLBuffers();
	}
	private: System::Void panel_sceneRender_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
		OGLForm->init();
	}
};
}
