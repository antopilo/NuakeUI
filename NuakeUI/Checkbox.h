#pragma once
#include "Node.h"


namespace NuakeUI
{
	class Checkbox : public Node
	{
	private:
		bool mChecked = false;
	public:
		Checkbox(const std::string& name);
		~Checkbox() {};

		static std::shared_ptr<Checkbox> New(const std::string& name);

		bool Checked() const { return mChecked; }
		bool SetCheck(bool check) { mChecked = check; }

		void Draw();
		void Tick() override;
		void UpdateInput(InputManager* inputManager) override;
	};
}