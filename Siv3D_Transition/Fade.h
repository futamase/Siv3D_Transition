#pragma once

#include <thread>
#include <future>
#include <mutex>
#include <chrono>

#include <Siv3D\Stopwatch.hpp>
#include <Siv3D\Texture.hpp>
#include <Siv3D\PixelShader.hpp>
#include <Siv3D\ConstantBuffer.hpp>
#include <Siv3D\Easing.hpp>
#include <Siv3D\Fwd.hpp>

class ThreadGuard 
{
    std::thread th;
public:
	ThreadGuard() = default;
	void set(std::thread&& th_) { th = std::move(th_); }
    ~ThreadGuard() {
        if(th.joinable()) {
            th.join();
        }
    }
    ThreadGuard(ThreadGuard const&) = delete;
    ThreadGuard& operator=(ThreadGuard const&) = delete;
};

/**@brief �V�[���J�ڎ��̃t�F�[�h���s���N���X
*@attention FadeIn, FadeOut���ꂼ��P�x�������ĂׂȂ��i�e�V�[���N���X���ł̃C���X�^���X����z��j
*@note http://easings.net/ja ���Q��
*/
class Fade
{
private:
	s3d::Texture m_mask;
	s3d::Texture m_dummyTex;
	s3d::PixelShader m_ps;
	s3d::ConstantBuffer<s3d::Float4> m_range, m_color;
	s3d::Stopwatch m_timer;

	std::mutex m_mutex;
	bool m_inCalled = false;
	bool m_outCalled = false;
	std::function<void()> m_callback;
	ThreadGuard m_outThread;
	ThreadGuard m_inThread;
public:
	enum class Ease
	{
		In, Out, InOut
	};

	/**@brief �}�X�N�摜��p�����g�����W�V����
	*@param[in] maskFile ���[���摜�̃t�@�C���p�X
	*@param[in] color �}�X�N�̐F
	*/
	explicit Fade(const s3d::FilePath& maskFile, const s3d::ColorF& color = s3d::ColorF(0, 0, 0));

	/**@brief �}�X�N�摜��p���Ȃ��g�����W�V����(�����̃t�F�[�h)
	*@param[in] color �}�X�N�̐F
	*/
	explicit Fade(const s3d::ColorF& color = s3d::ColorF(0, 0, 0));

	/**@brief �t�F�[�h�C��
	*@param[in] callback �t�F�[�h���I�������Ƃ��ɌĂяo���֐�
	*@param[in] duration �t�F�[�h�ɂ����鎞��(�b)
	*@param[in] easingFunc �C�[�W���O�֐�(�w�肵�Ȃ��Ƃ��͐��`�Ƀt�F�[�h)
	*@param[in] ease In��Out��InOut
	*/
	void FadeIn(const std::function<void()>& callback = []() {},
		double duration = 2.0,
		const std::function<double(double)>& easingFunc = s3d::Easing::Linear,
		Ease ease = Ease::InOut);

	/**@brief �t�F�[�h�A�E�g
	*@param[in] callback �t�F�[�h���I�������Ƃ��ɌĂяo���֐�
	*@param[in] duration �t�F�[�h�ɂ����鎞��(�b)
	*@param[in] easingFunc �C�[�W���O�֐�(�w�肵�Ȃ��Ƃ��͐��`�Ƀt�F�[�h)
	*@param[in] ease In��Out��InOut
	*/
	void FadeOut(const std::function<void()>& callback = []() {},
		double duration = 2.0,
		const std::function<double(double)>& easingFunc = s3d::Easing::Linear,
		Ease ease = Ease::InOut);

	/**@brief ���[���摜��`�悷��
	*@attention �V�[�����̂��ׂẴI�u�W�F�N�g��Draw�̌�ɌĂяo������
	*/
	void Draw() const;
};
