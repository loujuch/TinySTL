#ifndef _ALLOC_HPP__
#define _ALLOC_HPP__

#include <new>
#include <cstddef>

#include <cassert>

namespace stl {

// 使用伙伴系统进行分配
class Alloc {
private:
	class AllocInstance {
		// 预分配内存
		char *const m_buffer_start_;
		char *const m_buffer_end_;
		// 伙伴链，第0位为8B，后一位大小为其前一位大小的2倍
		char *m_pool_list_head_[8];
		char *m_pool_list_tail_[8];

		// 判断一个指针是否在内存池中
		inline bool in_memory(const char *p) const {
			return m_buffer_start_ <= p && p < m_buffer_end_;
		}

		// 在一个内存块前部设置next指针已形成链表，要求ptr指向内存块大小大于指针大小
		inline void set_next_ptr(char *ptr, char *next) {
			*((char **)ptr) = next;
		}

		// 获得一个内存块的next指针
		inline char *get_next_ptr(char *ptr) {
			return *((char **)ptr);
		}
	public:
		AllocInstance(std::size_t size) :
			m_buffer_start_(static_cast<char *>(::operator new(size))),
			m_buffer_end_(m_buffer_start_ + size),
			m_pool_list_head_{ nullptr },
			m_pool_list_tail_{ nullptr } {
			static_assert(min_block_size >= sizeof(void *));

			// 要求内存池大小必须不为0且为1024倍数
			assert(size >= max_block_size && (size & 0x3ff) == 0);

			// 设置内存块起始地址
			m_pool_list_head_[7] = m_buffer_start_;

			// 设置第一块内存
			char *p = m_pool_list_head_[7];
			size -= max_block_size;

			// 设置接下来的块内存
			while(size >= max_block_size) {
				set_next_ptr(p, p + max_block_size);
				p = get_next_ptr(p);
				size -= max_block_size;
			}

			// 设置尾块next指针为nullptr
			set_next_ptr(p - max_block_size, nullptr);

			// 设置尾指针
			m_pool_list_tail_[7] = p;
		}

		~AllocInstance() {
			::operator delete(m_buffer_start_);
		}

		// 分配n字节内存
		inline void *allocate(std::size_t n, const void *p = nullptr) {
			assert(n <= max_block_size);
		}

		// 回收n字节内存
		inline void deallocate(void *p, std::size_t n) {
		}
	}; // class Memory
public:
	static constexpr std::size_t memory_pool_size = 32 * 1024;
	static constexpr std::size_t max_block_size = 1024;
	static constexpr std::size_t min_block_size = 8;
private:
	// 预分配内存
	static thread_local AllocInstance m_alloc_instance_;
public:
	// 分配n字节内存
	inline void *allocate(std::size_t n, const void *p = nullptr) {
		return m_alloc_instance_.allocate(n, p);
	}

	// 回收n字节内存
	inline void deallocate(void *p, std::size_t n) {
		m_alloc_instance_.deallocate(p, n);
	}
}; // class Alloc

thread_local Alloc::AllocInstance Alloc::m_alloc_instance_(Alloc::memory_pool_size);

} // namespace stl

#endif // _ALLOC_HPP__