#include "Lights/Light.h"

#include <gslib.h>
#include <imgui/imgui.h>

/*実装*/
// コンストラクタ
Light::Light(
	const GSvector3& position, const GScolor& ambient, const GScolor& diffuse, const GScolor& specular) :
	position_{ position },
	ambient_{ ambient },
	diffuse_{ diffuse },
	specular_{ specular } {
}

// 更新
void Light::update(float delta_time) {
#ifdef _DEBUG
	ImGui::Begin("Lighting");
	{
		ImGui::ColorEdit4("ambient", ambient_, ImGuiColorEditFlags_Float);
		ImGui::ColorEdit4("diffuse", diffuse_, ImGuiColorEditFlags_Float);
		ImGui::ColorEdit4("specular", specular_, ImGuiColorEditFlags_Float);
		ImGui::DragFloat3("position", position_);
	}
	ImGui::End();
#endif
}

// 描画
void Light::draw() const {
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_);
	glLightfv(GL_LIGHT5, GL_POSITION, GSvector4{ position_, 0.0f });

	gsEnable(GL_LIGHT0);
	gsEnable(GL_LIGHTING);

	gsSetShadowMapLightPosition(&position_);
}