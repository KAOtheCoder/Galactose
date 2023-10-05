#include "OutSerializer.h"
#include "Convert.h"

#include <ostream>

#define GT_STREAM_INSERTION_IMP(T)\
	OutSerializer& OutSerializer::operator<<(const T& a_rhs) {\
		*m_emitter << YAML::convert<T>::encode(a_rhs);\
		return *this;\
	}\

namespace Galactose {
	OutSerializer::OutSerializer() :
		m_emitter(std::make_shared<YAML::Emitter>())
	{}

	void OutSerializer::save(std::ostream& a_stream) {
		a_stream.write(m_emitter->c_str(), m_emitter->size());
	}

	OutSerializer& OutSerializer::operator<<(const nullptr_t a_rhs) {
		*m_emitter << YAML::Null;
		return *this;
	}

	OutSerializer& OutSerializer::operator<<(const Manip a_rhs) {
		*m_emitter << static_cast<YAML::EMITTER_MANIP>(a_rhs);
		return *this;
	}

	GT_STREAM_INSERTION_IMP(float)
	GT_STREAM_INSERTION_IMP(std::string)
	GT_STREAM_INSERTION_IMP(std::filesystem::path)
	GT_STREAM_INSERTION_IMP(Uuid)
	GT_STREAM_INSERTION_IMP(Vector2)
	GT_STREAM_INSERTION_IMP(Vector3)
	GT_STREAM_INSERTION_IMP(Vector4)
	GT_STREAM_INSERTION_IMP(Quaternion)
}
