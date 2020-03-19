#include "modules/common/include/registerer.h"

#include <string>
#include <vector>

namespace atd {
namespace common {

BaseClassMap& GlobalFactoryMap() {
  static BaseClassMap factory_map;
  return factory_map;
}

bool GetRegisteredClasses(
    const std::string& base_class_name,
    std::vector<std::string>* registered_derived_classes_names) {
  //CHECK_NOTNULL(registered_derived_classes_names);

  if (registered_derived_classes_names == nullptr) {
    return false;
  }

  BaseClassMap& map = GlobalFactoryMap();
  auto iter = map.find(base_class_name);
  if (iter == map.end()) {
    //<<"error:" << "class not registered:" << base_class_name<<std::endl;
    return false;
  }
  for (auto pair : iter->second) {
    registered_derived_classes_names->push_back(pair.first);
  }
  return true;
}

}  // namespace common
}  // namespace atd
