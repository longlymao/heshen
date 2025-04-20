/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <type_traits>
#include <memory>

namespace rolling {

	template <typename T> class SharedPtr;
	template <typename T> class WeakPtr;
	template <typename T> class EnableShareFromThis;

	template <typename T, typename = void>
	struct CanEnableShareFromThis : std::false_type {};

	template<typename T>
	struct CanEnableShareFromThis<T, 
		std::void_t<decltype(std::declval<T>().ShareFromThis()),
		std::is_same<SharedPtr<T>, decltype(std::declval<T>().ShareFromThis())>>> : std::true_type {};

	template<typename T>
	struct ControlBlock
	{
	public:
		ControlBlock() = delete;

		constexpr ControlBlock(std::nullptr_t) {}

		explicit ControlBlock(T* ptr) : refCount(1), data(ptr) {
		}

		~ControlBlock() {
			ReleaseData();
		}

		void AddRef()
		{
			refCount++;
		}

		void Release()
		{
			refCount--;
			if (refCount == 0) {
				ReleaseData();
			}
		}

		void AddWeakRef() {
			weakCount++;
		}

		void ReleaseWeak() {
			weakCount--;
			if (weakCount == 0 && refCount == 0) {
				delete this;
			}
		}

		bool IsAlive() const
		{
			return refCount > 0;
		}

	private:
		void ReleaseData() {
			if (data) {
				T* temp = data;
				data = nullptr;
				delete temp;
			}
		}

	private:
		int refCount = 0;
		int weakCount = 0;
		T* data = nullptr;

		friend class SharedPtr<T>;
		friend class WeakPtr<T>;
	};

	template<typename T>
	class SharedPtr
	{
	public:
		constexpr SharedPtr() = default;

		constexpr SharedPtr(std::nullptr_t) {}

		explicit SharedPtr(T* ptr) {
			if (ptr != nullptr) {
				block = new ControlBlock<T>(ptr);

				if constexpr (CanEnableShareFromThis<T>::value) {
					block->data->m_Wptr = WeakPtr<T>(*this);
				}
			}
		}

		SharedPtr(const SharedPtr& other) : block(other.block)
		{
			Retain();
		}

		SharedPtr(SharedPtr&& other) noexcept : block(other.block)
		{
			other.block = nullptr;
		}

		SharedPtr(WeakPtr<T>& other) noexcept : block(other.block)
		{
			Retain();
		}

		~SharedPtr() {
			Release();
		}

		SharedPtr& operator=(const SharedPtr& other)
		{
			if (this != &other)
			{
				Release();
				block = other.block;
				Retain();
			}
			return *this;
		}

		SharedPtr& operator=(const WeakPtr<T>& other)
		{
			if (this->block != &other.block)
			{
				Release();
				block = other.block;
				Retain();
			}
			return *this;
		}

		template <class T2, std::enable_if_t<std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		SharedPtr(const SharedPtr<T2>& other) noexcept {
			block = reinterpret_cast<decltype(block)>(other.block);
			Retain();
		}

		bool operator==(const SharedPtr& other) {
			if (block == nullptr && other.block == nullptr) {
				return true;
			}
			if (block == nullptr) {
				return other.block->data == nullptr;
			}
			if (other.block == nullptr) {
				return block->data == nullptr;
			}
			return block == other.block;
		}

		bool operator==(const T* other) {
			if (block == nullptr && other == nullptr) {
				return true;
			}
			if (block != nullptr) {
				return block->data == other;
			}
			return false;
		}

		operator bool() const {
			return block && block->data != nullptr;
		}

		T* operator->() const {
			return block->data;
		}

		T& operator*() const {
			return *block->data;
		}

	private:
		ControlBlock<T>* block = nullptr;

		void Retain() {
			if (block) {
				block->AddRef();
			}
		}

		void Release() {
			if (block) {
				block->Release();
			}
		}

		friend class WeakPtr<T>;
		friend class SharedPtr;
	};

	template<typename T>
	class WeakPtr {
	public:
		constexpr WeakPtr() = default;

		constexpr WeakPtr(std::nullptr_t) {}

		WeakPtr(const WeakPtr& other) :block(other.block) {
			Retain();
		}

		WeakPtr(WeakPtr&& other) noexcept : block(other.block) {
			other.block = nullptr;
		}

		WeakPtr(const SharedPtr<T>& other) : block(other.block) {
			Retain();
		}

		~WeakPtr() {
			Release();
		}

		WeakPtr& operator=(const WeakPtr& other) {
			if (this != &other) {
				Release();
				block = other.block;
				Retain();
			}
			return *this;
		}

		WeakPtr& operator=(const SharedPtr<T>& other) {
			if (this != &other) {
				Release();
				block = other.block;
				Retain();
			}
			return *this;
		}

		template <class T2, std::enable_if_t<std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		WeakPtr(const SharedPtr<T2>& other) noexcept {
			block = reinterpret_cast<decltype(block)>(other.block);
			Retain();
		}

		template <class T2, std::enable_if_t<std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		WeakPtr(const WeakPtr<T2>& other) noexcept {
			block = reinterpret_cast<decltype(block)>(other.block);
			Retain();
		}

		template <class T2, std::enable_if_t<std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		WeakPtr& operator=(WeakPtr<T2>& other) {
			if (reinterpret_cast<void*>(this) != reinterpret_cast<void*>(&other)) {
				Release();
				block = other.block;
				Retain();
			}
			return *this;
		}

		bool operator==(const WeakPtr& other) {
			if (block == nullptr && other.block == nullptr) {
				return true;
			}
			if (block == nullptr) {
				return other.block->data == nullptr;
			}
			if (other.block == nullptr) {
				return block->data == nullptr;
			}
			return block == other.block;
		}

		bool operator==(const SharedPtr<T>& other) {
			if (block == nullptr && other.block == nullptr) {
				return true;
			}
			if (block == nullptr) {
				return other.block->data == nullptr;
			}
			if (other.block == nullptr) {
				return block->data == nullptr;
			}
			return block == other.block;
		}

		bool operator==(const T* other) {
			if (block == nullptr && other == nullptr) {
				return true;
			}
			if (block != nullptr) {
				return block->data == other;
			}
			return false;
		}

		operator bool() {
			return block != nullptr 
				&& block->data != nullptr 
				&& block->IsAlive();
		}

		T* operator->() const {
			return block->data;
		}

		T& operator*() const {
			return *block->data;
		}

	private:
		ControlBlock<T>* block = nullptr;

		void Retain() {
			if (block) {
				block->AddWeakRef();
			}
		}

		void Release() {
			if (block) {
				block->ReleaseWeak();
			}
		}

		friend class SharedPtr<T>;
		friend class WeakPtr;
	};

	template<typename T>
	class EnableShareFromThis {
	public:
		EnableShareFromThis() = default;
		EnableShareFromThis(const EnableShareFromThis&) = delete;
		EnableShareFromThis(EnableShareFromThis&&) = delete;
		EnableShareFromThis& operator=(const EnableShareFromThis&) = delete;
		EnableShareFromThis& operator=(EnableShareFromThis&&) = delete;
		virtual ~EnableShareFromThis() {}
		SharedPtr<T> ShareFromThis() {
			return SharedPtr<T>(m_Wptr);
		}
	private:
		WeakPtr<T> m_Wptr;

		friend class SharedPtr<T>;

		template <typename U> friend class SharedPtr;
	};
};