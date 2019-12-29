#include "vision.hpp"

namespace lib7842::Vision {

Vision::Vision(std::uint8_t iport) : pros::Vision(iport), port(iport) {}

Container Vision::getAll() {}

} // namespace lib7842::Vision