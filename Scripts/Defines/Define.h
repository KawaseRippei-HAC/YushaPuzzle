#ifndef DEFINES_DEFINE_H_
#define DEFINES_DEFINE_H_

/*定数*/
// 初期ウィンドウ幅
inline constexpr int INITIAL_WINDOW_WIDTH{ 1920 };
// 初期ウィンドウ高さ
inline constexpr int INITIAL_WINDOW_HEIGHT{ 1080 };

// 最適化しきい値
inline constexpr size_t optimal_threshold{ sizeof(void*) * 2 };

// 最適化引数
template<typename _Type>
using optimal_param_t = std::conditional_t<(sizeof(_Type) <= optimal_threshold), _Type, const _Type&>;

#endif // DEFINES_DEFINE_H_