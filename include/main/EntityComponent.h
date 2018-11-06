#ifndef XXX_ENTITY_COMPONENT_H
#define XXX_ENTITY_COMPONENT_H

namespace {

// Quickly thrown togethr, need to tidy up and make better
class Entity {
 public:
  Entity(){};

  void addComponent(Component componentIn) {
    componentList.push_back(componentIn);
  }

  void update(float dt) {
    for (int i = 0; i < componentList.size(); ++i) componentList[i].update(dt);
  }

 private:
  std::vector<Component> componentList;
}

// We may revert back to ECS, so no logic in component
template <class T>
class Component {
  Component(){};

  void update(float dt){
      // udpate the component type
  };

 private:
  T componentType;
}

}  // namespace

#endif  // XXX_ENTITY_COMPONENT_H
