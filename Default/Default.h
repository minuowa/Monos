#ifndef Default_h__
#define Default_h__

namespace Default
{
	enum Const
	{
		NameSize = 24,
		PSDSize = 24,
		DataSize = 1024,
	};

	template<typename T>
	T DefaultValue() {
		T t;
		return t;
	}

	template<>
	inline int DefaultValue() {
		return 0;
	}
	template<>
	inline bool DefaultValue() {
		return false;
	}
}
#endif // Default_h__
