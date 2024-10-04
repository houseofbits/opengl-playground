#pragma once

#include "../../../Core/API.h"

class EntityBehaviourComponent : public Component {
TYPE_DEFINITION(EntityBehaviourComponent);

public:
    inline static const std::string BEHAVIOURS_KEY = "behaviours";

    enum BehaviourType {
        BEHAVIOUR_CHARACTER_MOVEMENT,
        BEHAVIOUR_CAMERA_MOUSE_LOOK,
        BEHAVIOUR_CAMERA_MOVEMENT,
        BEHAVIOUR_CAMERA_PICKING,
        BEHAVIOUR_DOOR_ACTIVATION,
        //etc.
    };

    inline const static std::map<BehaviourType, std::string> BehaviourNames = {
            {BEHAVIOUR_CHARACTER_MOVEMENT, "active-character-input-move"},
            {BEHAVIOUR_CAMERA_MOUSE_LOOK,  "active-camera-mouse-look"},
            {BEHAVIOUR_CAMERA_MOVEMENT,    "active-camera-input-move"},
            {BEHAVIOUR_CAMERA_PICKING,     "active-camera-picking"},
            {BEHAVIOUR_DOOR_ACTIVATION,    "door-activation"}
    };

    inline const static std::map<BehaviourType, std::string> BehaviourDescriptions = {
            {BEHAVIOUR_CHARACTER_MOVEMENT, "Move character with WASD keys. Active character is the one that has an active camera. Requires CameraComponent, CharacterControllerComponent"},
            {BEHAVIOUR_CAMERA_MOUSE_LOOK,  "Mouse look for the active camera. Requires CameraComponent"},
            {BEHAVIOUR_CAMERA_MOVEMENT,    "Moves active camera with mouse and WASD. Requires CameraComponent"},
            {BEHAVIOUR_CAMERA_PICKING,     "Ray cast from point-of-view for object picking. Requires CameraComponent and/or CharacterControllerComponent. Emits PhysicsPickingEvent"},
            {BEHAVIOUR_DOOR_ACTIVATION,    "Activate the door by character input... Requires PhysicsJointComponent"}
    };

    EntityBehaviourComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    void registerWithSystems(EntityContext &ctx) override;

    void addBehaviour(BehaviourType type);

    void addBehaviour(bool isEnabled, BehaviourType type);

    void removeBehaviour(BehaviourType type);

    bool hasBehaviour(BehaviourType type);

    bool hasActiveBehaviour(BehaviourType type);

    std::list<std::pair<bool, BehaviourType>> m_behaviours;

private:

};
