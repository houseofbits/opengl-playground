print("Run test scrtipt")

function InputEventHandler(event)
    --print(event.keyCode)
    --c = Manager:getComponent("OmniLight", "LightComponent");
    --if c then
    --    c.isEnabled = not c.isEnabled;
    --end

    --c = Manager:getComponent("Swing1", "Hinge");
    --if c then
    --    c:setMotorVelocity(5);
    --end

    --c = Manager:getComponent("Door1", "Hinge");
    --if c then
    --    c:toggleState();
    --end
end

function PhysicsPickingEventHandler(event)
    --print(event.entityName)
    --if event.entityName == "Door1" then
    --    c = Manager:getComponent("Door1", "Hinge");
    --    if c then
    --        c:toggleState();
    --    end
    --end
    --print(event.shapeName)
end

function PhysicsSensorEventHandler(event)
    --print(event.colliderName)
    --print(event.sensorName)
    --print(event.sensorShapeName)
end

Manager:registerEventHandler("InputEvent", InputEventHandler);
Manager:registerEventHandler("PhysicsPickingEvent", PhysicsPickingEventHandler);
Manager:registerEventHandler("PhysicsSensorEvent", PhysicsSensorEventHandler);