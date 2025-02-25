// #include "PhysicsSliderJointComponentEdit.h"
// #include "../../Editor/Helpers/TransformHelper.h"
// #include "../../../Core/Helper/Math.h"
//
// void PhysicsSliderJointComponentEdit::processEditor() {
//     EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
//         *m_editorSystem->m_EntityContext,
//         m_component->m_targetEntityAName,
//         "Attachment entity A##TRANSFORM_PARENT_ENTITY_NAME_A",
//         "Self"
//     );
//
//     EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
//     *m_editorSystem->m_EntityContext,
//     m_component->m_targetEntityBName,
//         "Attachment entity B##TRANSFORM_PARENT_ENTITY_NAME_B"
//     );
//
//     // ImGui::InputFloat3("Axis", reinterpret_cast<float *>(&m_component->m_axis));
//     if (ImGui::TreeNode("Attachment transform A")) {
//         TransformHelper::editTransform(m_component->m_localAttachmentMatrixA, true, true, false);
//         ImGui::TreePop();
//     }
//     if (ImGui::TreeNode("Attachment transform B")) {
//         TransformHelper::editTransform(m_component->m_localAttachmentMatrixB, true, true, false);
//         ImGui::TreePop();
//     }
//
//     ImGui::Checkbox("Limits", &m_component->m_isLimitsEnabled);
//
//     if (m_component->m_isLimitsEnabled) {
//         ImGui::InputFloat2("Min/Max", reinterpret_cast<float *>(&m_component->m_limits));
//     }
//
//     ImGui::Checkbox("Motor", &m_component->m_isMotorSettingsEnabled);
//
//     if (m_component->m_isMotorSettingsEnabled) {
//         ImGui::DragFloat("Force limit", &m_component->m_motorForceLimit, 0.1f);
//         ImGui::DragFloat("Damping", &m_component->m_motorDamping, 0.1f);
//         ImGui::DragFloat("Frequency", &m_component->m_motorFrequency, 0.1f);
//     }
// }
//
// void PhysicsSliderJointComponentEdit::handleEntitySelection(Entity &e, Component *c) {
//     ComponentEdit::handleEntitySelection(e, c);
//
//     m_linkedTransformA = getTransformComponentByName(m_component->m_targetEntityAName);
//     m_linkedTransformB = getTransformComponentByName(m_component->m_targetEntityBName);
// }
//
// TransformComponent *PhysicsSliderJointComponentEdit::getTransformComponentByName(const std::string &name) const {
//     if (name.empty()) {
//         return m_entity->getComponent<TransformComponent>();
//     }
//
//     return m_editorSystem->m_EntityContext->findEntityComponent<TransformComponent>(name);
// }
//
// PhysicsSliderJointComponentEdit::TransformOption PhysicsSliderJointComponentEdit::getTransformTargetOptions(int index) {
//     switch (index) {
//         case 1:
//             return {
//                 "Attachment point A",
//                 true,
//                 true,
//                 false,
//                 ImGuizmo::TRANSLATE,
//                 false,
//                 true,
//             };
//         case 2:
//             return {
//                 "Attachment point B",
//                 true,
//                 true,
//                 false,
//                 ImGuizmo::TRANSLATE,
//                 false,
//                 true,
//             };
//         default:
//             return {
//                 "Common attachment point",
//                 true,
//                 true,
//                 false,
//                 ImGuizmo::TRANSLATE,
//                 false,
//                 true,
//             };
//     };
// }
//
// glm::mat4 PhysicsSliderJointComponentEdit::getWorldSpaceTransform(int index) {
//     if (m_linkedTransformA == nullptr || m_linkedTransformB == nullptr) {
//         return {};
//     }
//
//     if (index == 2) {
//         auto m = m_component->m_localAttachmentMatrixB;
//         const glm::mat4 transformB = Math::rescaleMatrix(m_linkedTransformB->m_transform);
//
//         return transformB * m;
//     }
//
//     auto m = m_component->m_localAttachmentMatrixA;
//     const glm::mat4 transformA = Math::rescaleMatrix(m_linkedTransformA->m_transform);
//
//     return transformA * m;
// }
//
// void PhysicsSliderJointComponentEdit::setWorldSpaceTransform(glm::mat4 m, int index) const {
//     if (m_linkedTransformA == nullptr || m_linkedTransformB == nullptr) {
//         return;
//     }
//
//     const glm::mat4 transformA = glm::inverse(Math::rescaleMatrix(m_linkedTransformA->m_transform));
//     if (index == 1) {
//         m_component->m_localAttachmentMatrixA = transformA * m;
//
//         return;
//     }
//
//     const glm::mat4 transformB = glm::inverse(Math::rescaleMatrix(m_linkedTransformB->m_transform));
//     if (index == 2) {
//         m_component->m_localAttachmentMatrixB = transformB * m;
//
//         return;
//     }
//
//     m_component->m_localAttachmentMatrixA = transformA * m;;
//     m_component->m_localAttachmentMatrixB = transformB * m;
// }