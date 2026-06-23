#ifndef UTILITIES_NONCOPYABLE_H_
#define UTILITIES_NONCOPYABLE_H_

// コピー禁止クラス
template<class _ClassName>
class Noncopyable {
protected:
	Noncopyable() = default;
private:
	/*コピー禁止*/
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable operator=(const Noncopyable&) = delete;
};

#endif // UTILITIES_NONCOPYABLE_H_