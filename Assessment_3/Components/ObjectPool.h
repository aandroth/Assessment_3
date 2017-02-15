#pragma once

template <typename T>
class ObjectPool
{
	struct __intern
	{
		bool open;
		size_t next;
		T data;
	};
public:
	__intern *m_data;
	size_t m_size;
	size_t openHead, fillHead;
	

public:
	/* Rule of 5 */
	ObjectPool(const ObjectPool &) = delete;
	ObjectPool &operator=(const ObjectPool &) = delete;
	ObjectPool(ObjectPool &&) = delete;
	ObjectPool &operator=(ObjectPool &&) = delete;

	~ObjectPool() { delete[] m_data; }

	ObjectPool(size_t a_size) : m_size(a_size), openHead(0), fillHead(m_size)
	{
		m_data = new __intern[m_size];

		for (size_t ii = 0; ii < m_size; ++ii)
		{
			m_data[ii].open = true;
			m_data[ii].next = ii + 1;
		}
	}

	class iterator
	{	public:
		ObjectPool *m_ref;

		friend class ObjectPool;
	public:
		iterator(ObjectPool *a_ref, size_t a_idx) : m_ref(a_ref), m_idx(a_idx) {}

		size_t m_idx;
		iterator() : m_ref(nullptr), m_idx(0) {}

		T &operator* () { return m_ref->m_data[m_idx].data; }
		T *operator->() { return &m_ref->m_data[m_idx].data; }
		T *operator& () { return &m_ref->m_data[m_idx].data; }

		const T &operator* () const { return m_ref->m_data[m_idx].data; }
		const T *operator->() const { return &m_ref->m_data[m_idx].data; }
		const T *operator& () const { return &m_ref->m_data[m_idx].data; }
		
		iterator &operator++() { m_idx = m_ref->m_data[m_idx].next; return *this; }
		iterator operator++(int) { auto that = *this; operator++(); return that; }

		bool operator==(const iterator &O) const { return m_ref == O.m_ref && m_idx == O.m_idx; }
		bool operator!=(const iterator &O) const { return !(*this == O); }
		
		operator bool() const { return m_ref != nullptr && m_idx < m_ref->m_size && !m_ref->m_data[m_idx].open; }

		operator       T*()       { return operator&(); }
		operator const T*() const { return operator&(); }

		iterator &free() { return *this = m_ref->pop(*this); }
	};

	iterator push(const T &val = T())
	{
		if (openHead >= m_size)
			return iterator();

		size_t idx = openHead;

		m_data[idx].data = val;
		m_data[idx].open = false;

		openHead = m_data[openHead].next;

		if (idx < fillHead)
		{
			m_data[idx].next = fillHead;
			fillHead = idx;
		}
		else
		{
			size_t left = idx;
			while (m_data[--left].open)/* Do Nothing */;

			m_data[idx].next = m_data[left].next;
			m_data[left].next = idx;
		}
		return iterator(this, idx);
	}

	iterator pop(iterator itr)
	{
		if (!(itr && itr.m_ref == this)) 
			return iterator();

		size_t idx = itr.m_idx;
		++itr;

		m_data[idx].open = true;
		m_data[idx].data = T();

		if (idx == fillHead)
		{
			fillHead = m_data[idx].next;
		}
		else
		{
			size_t left = idx;
			while (m_data[--left].open)/*Do Nothing*/;
			m_data[left].next = m_data[idx].next;
		}

		if (idx < openHead)
		{
			m_data[idx].next = openHead;
			openHead = idx;
		}
		else
		{
			size_t left = idx;

			while (!m_data[--left].open)/*Do Nothing*/;

			m_data[idx].next = m_data[left].next;
			m_data[left].next = idx;
		}

		return itr;
	}

	iterator begin() { return iterator(this, fillHead); }
	iterator end() { return iterator(this, m_size); }
};