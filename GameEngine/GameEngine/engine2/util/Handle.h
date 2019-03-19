#pragma once

#include <API.h>
#include<vector>
#include<algorithm>

namespace GameEngine::Util{
	
	
		template<
			typename handle_value_type,
			size_t version_bits,
			size_t index_bits
		>
		union Handle
		{
			static_assert(sizeof(handle_value_type) * CHAR_BIT >= (version_bits + index_bits), "Invalid handle layout. More bits used than base value type can hold!");

			using value_type = handle_value_type;

			static constexpr size_t			NUM_VERSION_BITS	{ version_bits };
			static constexpr size_t			NUM_INDEX_BITS		{ index_bits };


			static constexpr value_type		MIN_VERISON			{ 0 };
			static constexpr value_type		MAX_VERSION			{ (1U << NUM_VERSION_BITS) - 2U };
			static constexpr value_type		MAX_INDICES			{ (1U << NUM_INDEX_BITS)   - 2U };

			static constexpr value_type		INVALID_HANDLE		{ std::numeric_limits<value_type>::max() };

		private:

			value_type						value;

		public:

			struct
			{
				value_type					index	: NUM_INDEX_BITS;
				value_type					version : NUM_VERSION_BITS;
			};


			Handle()
			{}

			Handle(value_type value) :
				value(value)
			{}

			Handle(value_type index, value_type version) :
				index(index),
				version(version)
			{}

			inline operator value_type() const { return value; }
		};
	} 



	/// Summary:	Defines a 32 bit handle
	/// Max. possible handles							: 1048576
	/// Max. possible versions until loop per handle	: 4096
	


	/// Summary:	Defines a 64 bit handle
	/// Max. possible handles							: 1099511627776
	/// Max. possible versions until loop per handle	: 16777216


namespace GameEngine::Util {
using Handle32 = GameEngine::Util::Handle<uint32_t, 12, 20>;
#ifdef GE_64BIT
	using Handle64 = Internal::Handle<u64, 24, 40>;
#else
	using Handle64 = Handle32;
#endif
	template<class T, class handle_type, size_t grow = 1024>
	class HandleTable
	{
		using Handle = handle_type;

	private:

		using TableEntry = std::pair<typename Handle::value_type, T*>;

		std::vector<TableEntry> m_Table;


		void GrowTable()
		{
			size_t oldSize = this->m_Table.size();

			assert(oldSize < Handle::MAX_INDICES && "Max table capacity reached!");

			size_t newSize = std::min(oldSize + grow, Handle::MAX_INDICES);

			this->m_Table.resize(newSize);

			for (typename Handle::value_type i = oldSize; i < newSize; ++i)
				this->m_Table[i] = TableEntry(Handle::MIN_VERISON, nullptr);
		}

	public:

		HandleTable()
		{
			this->GrowTable();
		}

		~HandleTable()
		{}

		Handle AqcuireHandle(T* rawObject)
		{
			typename Handle::value_type i = 0;
			for (; i < this->m_Table.size(); ++i)
			{
				if (this->m_Table[i].second == nullptr)
				{
					this->m_Table[i].second = rawObject;

					this->m_Table[i].first = ((this->m_Table[i].first + 1) > Handle::MAX_VERSION) ? Handle::MIN_VERISON : this->m_Table[i].first + 1;

					return Handle(i, this->m_Table[i].first);
				}
			}

			// grow table
			this->GrowTable();

			this->m_Table[i].first = 1;
			this->m_Table[i].second = rawObject;		

			return Handle(i, this->m_Table[i].first);
		}

		void ReleaseHandle(Handle handle)
		{
			assert((handle.index < this->m_Table.size() && handle.version == this->m_Table[handle.index].first) && "Invalid handle!");
			this->m_Table[handle.index].second = nullptr;
		}


		inline bool IsExpired(Handle handle) const
		{
			return this->m_Table[handle.index].first != handle.version;
		}

		

		inline Handle operator[](typename Handle::value_type index) const
		{
			assert(index < this->m_Table.size() && "Invalid handle!");
			return Handle(index, this->m_Table[index].first);
		}


	
		

		inline T* operator[](Handle handle)
		{	
			if (handle.INVALID_HANDLE == handle)
				return nullptr;
			assert((handle.index < this->m_Table.size() && handle.version == this->m_Table[handle.index].first) && "Invalid handle!");
			return (this->m_Table[handle.index].first == handle.version ? this->m_Table[handle.index].second : nullptr);
		}

	}; // class HandleTable	

}
