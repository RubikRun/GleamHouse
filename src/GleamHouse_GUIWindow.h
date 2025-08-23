#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "ColorEdit3Widget.h"
#include "DragFloat2Widget.h"

namespace Demo
{

	class GleamHouse_GUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		GleamHouse_GUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		bool init() override;

		// Returns current value of the "background color" parameter
		glm::vec4 getBackgroundColor() const;

	private: /* functions */

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
			Pekan::GUI::TextWidget_Ptr textWidget_backgroundColor    = std::make_shared<Pekan::GUI::TextWidget>();
			Pekan::GUI::ColorEdit3Widget_Ptr colorEditWidget         = std::make_shared<Pekan::GUI::ColorEdit3Widget>();
		} gui;
	};

} // namespace Demo