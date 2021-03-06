#include <Snowing.h>
#include <string_view>

using namespace Snowing;

int main()
{
	// 创建引擎实例
	auto engine = PlatformImpls::WindowsImpl::MakeEngine(
		L"TextRendering",
		{ 800,600 },
		true);

	// 加载字体资源
	const Graphics::Font fnt = 
		Graphics::LoadFont(
			LoadAsset("Font-chs.fnt"));

	// 加载着色器资源
	Graphics::Effect effect
	{
		LoadAsset("FontShader.cso")
	};

	// 加载字体渲染技术，注意，渲染技术存活期间，Effect不得被销毁或移动
	auto fontTech = effect.LoadTechnique(
		"FontRendering",
		Graphics::Sprite::DataLayout);

	// 坐标系
	constexpr Math::Coordinate2DRect coord
	{
		{ 0.0f, 0.0f },
		{ 800.0f, 600.0f }
	};

	// 使用字体渲染器类型
	using FontRenderer = Graphics::FontRenderer<>;

	// 顶点缓存
	auto vertexBuffer = FontRenderer::MakeGPUVertexBuffer();

	// 字体渲染器
	Graphics::FontRenderer<> renderer
	{
		&Graphics::Device::Get().MainContext(),
		&fontTech,
		coord,
		&fnt,
		&vertexBuffer
	};

	// 要被渲染的字符串
	const std::wstring_view str = L"Hello!\nStrrationalism";

	// 字体精灵列表
	std::vector<Graphics::FontSprite> sprites;

	// 字符显示框
	constexpr Math::Vec4f box = { 100.0f,300.0f,600.0f,500.0f };

	// 字体间距
	constexpr Math::Vec2f space = { 0.7f,75.0f };

	// 字体大小
	constexpr Math::Vec2f fontSize = { 1.0f,1.0f };

	// 根据字符串创建字体精灵列表
	Graphics::FontSprite::SetString(str, fnt, box, space, fontSize, sprites);

	// 启动主循环
	Engine::Get().Run([&] {

		// 渲染图形
		Engine::Get().Draw([&]{

			// 获取图形设备
			auto &graphics = Snowing::Graphics::Device::Get();

			// 清空画布
			graphics.MainContext().ClearRenderTarget(
				graphics.MainRenderTarget());

			// 设置屏幕为当前画布
			graphics.MainContext().SetRenderTarget(
				&graphics.MainRenderTarget());

			// 渲染字符到缓存
			for(const auto& sp : sprites)
				renderer.DrawToSpriteBuffer(sp);

			// Flush字符渲染器缓存
			renderer.FlushSpriteBuffer();
		});
	});


	return 0;
}
