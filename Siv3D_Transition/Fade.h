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

/**@brief シーン遷移時のフェードを行うクラス
*@attention FadeIn, FadeOutそれぞれ１度ずつしか呼べない（各シーンクラス内でのインスタンス化を想定）
*@note http://easings.net/ja を参照
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

	/**@brief マスク画像を用いたトランジション
	*@param[in] maskFile ルール画像のファイルパス
	*@param[in] color マスクの色
	*/
	explicit Fade(const s3d::FilePath& maskFile, const s3d::ColorF& color = s3d::ColorF(0, 0, 0));

	/**@brief マスク画像を用いないトランジション(ただのフェード)
	*@param[in] color マスクの色
	*/
	explicit Fade(const s3d::ColorF& color = s3d::ColorF(0, 0, 0));

	/**@brief フェードイン
	*@param[in] callback フェードが終了したときに呼び出す関数
	*@param[in] duration フェードにかける時間(秒)
	*@param[in] easingFunc イージング関数(指定しないときは線形にフェード)
	*@param[in] ease InかOutかInOut
	*/
	void FadeIn(const std::function<void()>& callback = []() {},
		double duration = 2.0,
		const std::function<double(double)>& easingFunc = s3d::Easing::Linear,
		Ease ease = Ease::InOut);

	/**@brief フェードアウト
	*@param[in] callback フェードが終了したときに呼び出す関数
	*@param[in] duration フェードにかける時間(秒)
	*@param[in] easingFunc イージング関数(指定しないときは線形にフェード)
	*@param[in] ease InかOutかInOut
	*/
	void FadeOut(const std::function<void()>& callback = []() {},
		double duration = 2.0,
		const std::function<double(double)>& easingFunc = s3d::Easing::Linear,
		Ease ease = Ease::InOut);

	/**@brief ルール画像を描画する
	*@attention シーン内のすべてのオブジェクトのDrawの後に呼び出すこと
	*/
	void Draw() const;
};
