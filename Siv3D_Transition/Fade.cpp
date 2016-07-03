#include "Fade.h"

#include <Siv3D\Window.hpp>
#include <Siv3D\Graphics2D.hpp>

Fade::Fade(const s3d::FilePath& maskFile, const s3d::ColorF& color) :
	m_mask(maskFile),
	m_dummyTex(s3d::Image(s3d::Window::Size())),
	m_ps(L"Transition.hlsl")
{
	m_range->x = 1.0f;
	m_color->set(color.rgba());
}

Fade::Fade(const s3d::ColorF& color) :
	m_mask(L""),
	m_dummyTex(s3d::Image(s3d::Window::Size())),
	m_ps(L"TransitionAlpha.hlsl")
{
	m_color->set(color.rgba());
}

void Fade::FadeIn(const std::function<void()>& callback,
	double duration,
	const std::function<double(double)>& easingFunc,
	Ease ease)
{
	// Ç±ÇÃä÷êîÇÕÇPìxÇµÇ©åƒÇÒÇ≈ÇÕÇ¢ÇØÇ»Ç¢
	if (m_inCalled || m_timer.isActive()) return;
	m_inCalled = true;

	m_callback = callback;

	m_inThread.set(std::thread([this, duration, easingFunc, ease]()
	{
		m_timer.start();
		while (true)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			double t = s3d::Min(m_timer.ms() / 1000.0, duration);

			auto value = ease == Ease::In ? s3d::Easing::EaseIn(-1.0, 1.0, easingFunc, t / duration) :
				ease == Ease::Out ? s3d::Easing::EaseOut(-1.0, 1.0, easingFunc, t / duration) :
				/*ease == Ease::InOut?*/s3d::Easing::EaseInOut(-1.0, 1.0, easingFunc, t / duration);

			m_range->x = static_cast<float>(value);

			if (t == duration) break;
		}
		m_timer.reset();
		m_callback();
	}));
}

void Fade::FadeOut(const std::function<void()>& callback,
	double duration,
	const std::function<double(double)>& easingFunc,
	Ease ease)
{
	// Ç±ÇÃä÷êîÇÕÇPìxÇµÇ©åƒÇÒÇ≈ÇÕÇ¢ÇØÇ»Ç¢
	if (m_outCalled || m_timer.isActive()) return;
	m_outCalled = true;

	m_callback = callback;

	m_outThread.set(std::thread([this, duration, easingFunc, ease]()
	{
		m_timer.start();
		while (true)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			double t = s3d::Min(m_timer.ms() / 1000.0, duration);

			auto value = ease == Ease::In   ?  s3d::Easing::EaseIn(1.0, -1.0, easingFunc, t / duration) :
                         ease == Ease::Out  ?  s3d::Easing::EaseOut(1.0, -1.0, easingFunc, t / duration) :
                       /*ease == Ease::InOut?*/s3d::Easing::EaseInOut(1.0, -1.0, easingFunc, t / duration);

			m_range->x = static_cast<float>(value);

			if (t == duration) break;
		}
		m_timer.reset();
		m_callback();
	}));
}

void Fade::Draw() const
{
	if (!m_inCalled && !m_outCalled) return;

	s3d::Graphics2D::BeginPS(m_ps);
	s3d::Graphics2D::SetConstant(s3d::ShaderStage::Pixel, 0, m_range);
	s3d::Graphics2D::SetConstant(s3d::ShaderStage::Pixel, 1, m_color);
	s3d::Graphics2D::SetTexture(s3d::ShaderStage::Pixel, 1, m_mask);
	m_dummyTex.draw();
	s3d::Graphics2D::EndPS();
}
