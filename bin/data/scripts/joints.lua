print("Run test scrtipt")

function InputEventHandler(event)
    --print(event.keyCode)

    -- A
    --if event.keyCode == 4 then
    --    c = Manager:getComponent("Dynamic1Detached", "Joint")
    --    c:connectToEntity("Static1")
    --end
    ---- Z
    --if event.keyCode == 29 then
    --    c = Manager:getComponent("Dynamic1Detached", "Joint")
    --    print(c)
    --    c:disconnect()
    --end

    
    --print(c);
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