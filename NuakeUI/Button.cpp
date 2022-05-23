#include "Button.h"


namespace NuakeUI
{
	std::shared_ptr<Button> Button::New(const std::string& name, const std::string& label)
	{
		return std::make_shared<Button>(name, label);
	}

	Button::Button(const std::string& name, const std::string& label) :
		Label(label)
	{
		ID = name;
		mNode = YGNodeNew();
	}

	void Button::Tick()
	{
		
	}
}