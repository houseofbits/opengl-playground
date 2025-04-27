print("Run ship.lua")

function PhysicsPickingEventHandler(event)
    --print(event.entityName)
    if event.entityName == "Button_WalkOn" then
		motor1 = Manager:getComponent("RArm2", "Motor");
		motor2 = Manager:getComponent("LArm2", "Motor");
		turnTable = Manager:getComponent("BodyTurnTable", "Motor");

        motor1:unLock();
		motor1:setMotorVelocity(1);

		motor2:unLock();
		motor2:setMotorVelocity(1);

		turnTable:lockInPlace();
    end

    if event.entityName == "Button_WalkOff" then
		motor1 = Manager:getComponent("RArm2", "Motor");
		motor2 = Manager:getComponent("LArm2", "Motor");

		motor1:setMotorOff();
		motor1:lockInPlace();

		motor2:setMotorOff();
		motor2:lockInPlace();
    end

    if event.entityName == "Button_TurnRight" then
        turnTable = Manager:getComponent("BodyTurnTable", "Motor");
        turnTable:unLock();
		turnTable:setMotorVelocity(0.05);
    end

    if event.entityName == "Button_TurnOff" then
        turnTable = Manager:getComponent("BodyTurnTable", "Motor");
		turnTable:setMotorOff();
		turnTable:lockInPlace();
    end

    if event.entityName == "Button_TurnLeft" then
        turnTable = Manager:getComponent("BodyTurnTable", "Motor");
        turnTable:unLock();
		turnTable:setMotorVelocity(-0.05);
    end
end

function InputEventHandler(event)
	--if event.keyCode == 20 then
	--	motor1 = Manager:getComponent("RArm2", "Motor");
	--	motor2 = Manager:getComponent("LArm2", "Motor");
    --
	--	motor1:setMotorVelocity(1);
	--	motor2:setMotorVelocity(1);
	--end
end

Manager:registerEventHandler("InputEvent", InputEventHandler);
Manager:registerEventHandler("PhysicsPickingEvent", PhysicsPickingEventHandler);