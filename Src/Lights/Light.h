#ifndef LIGHTS_LIGHT_H_
#define LIGHTS_LIGHT_H_

#include <GStype.h>

// ライトクラス
class Light {
private:
	// 標準環境光
#define DEFAULT_AMBIENT GScolor{0.0f, 0.0f, 0.0f, 1.0f}
	// 標準拡散反射光
#define DEFAULT_DIFFUSE GScolor{1.0f, 1.0f, 1.0f, 1.0f}
	// 標準鏡面反射光
#define DEFAULT_SPECULAR GScolor{1.0f, 1.0f, 1.0f, 1.0f}
public:
	Light(
		const GSvector3& position,
		const GScolor& ambient = DEFAULT_AMBIENT,
		const GScolor& diffuse = DEFAULT_DIFFUSE,
		const GScolor& specular = DEFAULT_SPECULAR);

	// 更新
	void update(float delta_time);
	// 描画
	void draw(void) const;

private:
	// 座標
	GSvector3 position_{ 0.0f, 0.0f, 0.0f };

	// 環境光
	GScolor ambient_{ DEFAULT_AMBIENT };
	// 拡散反射光
	GScolor diffuse_{ DEFAULT_DIFFUSE };
	// 鏡面反射光
	GScolor specular_{ DEFAULT_SPECULAR };
};

#endif // LIGHTS_LIGHT_H_