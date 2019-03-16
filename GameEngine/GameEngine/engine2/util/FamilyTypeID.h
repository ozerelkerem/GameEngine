#pragma once

#include<Api.h>

namespace GameEngine::Util
{


	template<class T>
	class FamilyTypeID
	{
		
	public:
		static TypeID s_count;
		template<class U>
		static const TypeID Get()
		{
			static TypeID STATIC_TYPE_ID{ s_count++ };
			return STATIC_TYPE_ID;
		}

		static const TypeID Get()
		{
			return s_count;
		}

	};

}
