#include "FamilyTypeID.h"


class IComponent;
class ISystem;

namespace GameEngine::Util
{

		

		TypeID FamilyTypeID<IComponent>::s_count = 0;
		TypeID FamilyTypeID<ISystem>::s_count = 0;

		template class FamilyTypeID<IComponent>;
		template class FamilyTypeID<ISystem>;

}