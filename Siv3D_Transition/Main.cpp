#include <Siv3D.hpp>
#include <HamFramework.hpp>

#include "Fade.h"

void Main()
{
	Texture img(L"Example/Windmill.png");
	Font font(30);

	Fade fade(L"mask00.png");

	while (System::Update())
	{
		img.draw();
		font(L"Text").draw();

		if (Input::KeyU.clicked) 
		{
			fade.FadeOut();
		}
		if (Input::KeyI.clicked)
		{
			fade.FadeIn();
		}
		fade.Draw();
	}
}
