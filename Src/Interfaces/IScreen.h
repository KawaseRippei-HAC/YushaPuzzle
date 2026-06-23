#ifndef INTERFACES_ISCREEN_H_
#define INTERFACES_ISCREEN_H_

class IScreen {
public:
	//デストラクタ
	virtual ~IScreen(void) = default;

	// 初期化
	virtual void initialize() = 0;
	// 更新
	virtual void update(float delta_time) = 0;
	// 描画
	virtual void draw(void) const = 0;
};

#endif
