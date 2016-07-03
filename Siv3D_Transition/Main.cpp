#include <Siv3D.hpp>
#include <HamFramework.hpp>

#include "Fade.h"

void Main()
{
	Texture img(L"Example/Windmill.png");
	Font font(10);

	Fade fade(L"m0.png");

	while (System::Update())
	{
		img.draw();
		font(L"hoge").draw();

		if (Input::KeyU.clicked) {
			fade.FadeOut();
			//if (fade.FadeIn()) {
			//}
		}
		if(Input::KeyI.clicked)
//			if (fade.FadeIn())
			{// System::Exit();
				fade.FadeIn();
			}
		fade.Draw();
	}
}
/*# include <Siv3D.hpp>

#include "assimp\Importer.hpp"
#include "assimp\scene.h"

#pragma comment(lib, "assimp-vc130-mtd")

class myMesh
{
public:
	bool Load(const std::string& fileName)
	{
		
     bool Ret = false;
     Assimp::Importer Importer;

     const aiScene* pScene = Importer.ReadFile(fileName.c_str(), 0);

     if (pScene) {
     }
     else {
     }

     return Ret;
	}
};


void Main()
{
	myMesh mesh;
	mesh.Load("cube.fbx");

	const Font font(30);
	Window::Resize(800, 600);
	Box box(1);
	Box floor(0, -2, 0, Vec3(10, 1, 10));
	while (System::Update())
	{
		Graphics3D::FreeCamera();
		font(Profiler::FPS()).draw();
		box.draw();
		floor.draw();
		Circle(Mouse::Pos(), 50).draw({ 255, 0, 0, 127 });
	}
}
*/
