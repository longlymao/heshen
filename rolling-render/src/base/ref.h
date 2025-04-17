/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

namespace rrender {

	template <typename T> class SharedPtr;
	template <typename T> class WeakPtr;
	template <typename T> class EnableShareFromThis;

	template<typename T>
	struct ControlBlock
	{
	public:
		ControlBlock() = delete;

		constexpr ControlBlock(std::nullptr_t) {}

		explicit ControlBlock(T* ptr) : refCount(1), data(ptr) {
		}

		~ControlBlock() {
			delete data;
		}

		void AddRef()
		{
			refCount++;
		}

		void Release()
		{
			refCount--;
			CheckRefCount();
		}

		void AddWeakRef() {
			weakCount++;
		}

		void ReleaseWeak() {
			weakCount--;
			CheckRefCount();
		}

		bool IsAlive() const
		{
			return refCount > 0;
		}

	private:
		int refCount = 0;
		int weakCount = 0;
		T* data = nullptr;

		void CheckRefCount()
		{
			if (refCount == 0 && weakCount == 0) {
				delete this;
			}
		}
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
			return block && block.data != nullptr;
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
	};

	template<typename T>
	class EnableShareFromThis {
	public:
		EnableShareFromThis() = default;
		EnableShareFromThis(const EnableShareFromThis&) = delete;
		EnableShareFromThis(EnableShareFromThis&&) = delete;
		EnableShareFromThis& operator=(const EnableShareFromThis&) = delete;
		EnableShareFromThis& operator=(EnableShareFromThis&&) = delete;
		virtual ~EnableShareFromThis() {
			m_Wptr.Release();
		}
		SharedPtr<T> ShareFromThis() {
			return SharedPtr<T>(m_Wptr);
		}
	private:
		WeakPtr<T> m_Wptr;
	};
};