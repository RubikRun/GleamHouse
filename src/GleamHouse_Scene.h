#pragma once

#include "Layer.h"
#include "RectangleShape.h"
#include "Camera2D.h"

#include "GleamHouse_GUIWindow.h"

namespace Demo
{

	class GleamHouse_Scene : public Pekan::Layer
	{
	public:

		GleamHouse_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;
		
		// Attaches a GUI window for controlling background's color
		void attachGUIWindow(const std::shared_ptr<const GleamHouse_GUIWindow>& guiWindow) { m_guiWindow = guiWindow; }

		inline std::string getLayerName() const override { return "scene_layer"; }

	private: /* functions */

		void createCamera();

	private: /* variables */

		// A small square to mark coordinate system's center
		Pekan::Renderer2D::RectangleShape m_centerSquare;

		Pekan::Renderer2D::Camera2D_Ptr m_camera;

		// GUI window for controlling background's color
		std::shared_ptr<const GleamHouse_GUIWindow> m_guiWindow;
	};

} // namespace Demo