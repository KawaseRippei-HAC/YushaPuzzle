#ifndef INTERFACES_IDRAG_HANDLER_H_
#define INTERFACES_IDRAG_HANDLER_H_

#include <GStype.h>

// ドラッグハンドラ抽象インターフェース
class IDragHandler {
public:
	virtual ~IDragHandler() = default;

	// ドラッグ開始時処理
	virtual void on_drag_begin(const GSvector2& position) = 0;
	// ドラッグ中処理
	virtual void on_dragging(
		const GSvector2& start_position, const GSvector2& current_position, const GSvector2& moved_delta) = 0;
	// ドラッグ終了時処理
	virtual void on_drag_end(const GSvector2& position) = 0;
};

#endif // INTERFACES_IDRAG_HANDLER_H_