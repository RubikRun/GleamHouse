#pragma once

#include "Layer.h"
#include "RectangleShape.h"
#include "Camera2D.h"
#include "Floor.h"

namespace GleamHouse
{

	class GleamHouse_Scene : public Pekan::Layer
	{
	public:

		GleamHouse_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;

		inline std::string getLayerName() const override { return "scene_layer"; }

	private: /* functions */

		void createCamera();

	private: /* variables */

		// A small square to mark coordinate system's center
		Pekan::Renderer2D::RectangleShape m_centerSquare;

		Floor m_floor;

		Pekan::Renderer2D::Camera2D_Ptr m_camera;
	};

} // namespace GleamHouse