#include <Siv3D.hpp>
#include <HamFramework.hpp>

#include "Fade.h"

enum class Scene
{
	Title, 
	Game,
};

void Main()
{
	Texture img(L"Windmill.png");
	Texture img2(L"Siv3D-kun.png");

	Font font(30);

	Fade fade(L"mask00.png");

	Scene scene = Scene::Title;

	while (System::Update())
	{

		if (scene == Scene::Title)
		{
			img.draw();
			font(L"Text").draw();
			if (Input::KeyEnter.clicked)
			{
				fade.FadeOut([&scene, &fade]()
				{
					scene = Scene::Game;
					fade.FadeIn([]() 
					{
					});
				});
			}
		}
		else if (scene == Scene::Game)
		{
			img2.draw();
			if (Input::KeyI.clicked)
			{
				fade.FadeIn();
			}
		}
		fade.Draw();
	}
}
