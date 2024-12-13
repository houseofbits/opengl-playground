## Milestone to v0.1 (_Lay down the basics_)

I should be able to move within a 3D world, pick and place objects, open and close doors. Have triggerable buttons. etc.
Character should have a basic gun that shoots tennis balls. I should be able to shoot a shooting targets, that fall over once hit.

### Priorities
1. Transform hierarchies, and transform editing
2. Joints 
3. Behaviours
4. Physics
5. Etc.

### Physics
- [x] Basic, parametric, character controller
- [x] Hinge joint component with limits and motor drive
- [x] Fixed physics joint
- [x] Slider joint
- [ ] Trigger shape component

### Behaviour
- [x] Door activation behaviour (door state)
- [ ] Trigger shape behaviour
- [ ] Character gun look-at and character rotation 
- [ ] Basic scripting behaviour prototype

### Editor
- [ ] Transform gizmo having selectable targets (Transform, Joint pivot, Joint axis)
  - [ ] Transform getter/setter for the Joint component
  - [x] Transform getter/setter for camera component
  - [x] UI Transform gizmo helper class
- [ ] Keyboard shortcuts for transform modes

### General
- [x] Relative transform for camera 
- [x] Relative transforms (assign parent transform component)

### Rendering
- [ ] Overlay cross-hair
- [ ] Static decals
- [ ] Shadow-map improvements
- [ ] Fix reflection probes

### Bugs
 - [x] Editor - Physics body type change mass validation
 - [ ] Editor - Component addition/removal
 - [x] Editor - Name change of the linked entities