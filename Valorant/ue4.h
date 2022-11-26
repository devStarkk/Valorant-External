#pragma once
#include <vector>

template<class T>
class TArray
{
public:
	int Length() const
	{
		return m_nCount;
	}

	bool IsValid() const
	{
		if (m_nCount > m_nMax)
			return false;
		if (!m_Data)
			return false;

		return true;
	}

	uint64_t GetAddress() const
	{
		return m_Data;
	}

	T operator[](uint64_t i) const {
		return Cool.ReadGuardedWrapper<uintptr_t>(((uintptr_t)m_Data) + i * sizeof(T));
	};

	[[nodiscard]]
	std::vector<T> Iteration(int maxCount = 1000) {
		if (m_nCount > maxCount)
			return {};

		std::vector<T> buffer(m_nCount);
		Cool.ReadArray<T>(m_Data, buffer.data(), m_nCount);
		return buffer;
	}

protected:
	uint64_t m_Data;
	uint32_t m_nCount;
	uint32_t m_nMax;
};