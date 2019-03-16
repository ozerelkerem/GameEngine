#include "FamilyTypeID.h"


class IComponent;

namespace GameEngine::Util
{

		

		TypeID FamilyTypeID<IComponent>::s_count = 0;

		template class FamilyTypeID<IComponent>;

}