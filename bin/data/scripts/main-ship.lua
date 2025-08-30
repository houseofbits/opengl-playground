print("Run ship.lua")

g_walkingMotorSpeed = 1.5;
g_turningMotorSpeed = 0.05;
g_isRighMotorOn = false;
g_isLeftMotorOn = false;
g_isTurnMotorOn = false;
g_isReverse = false;

function runMotor(name, speed)
    motorRight = Manager:getComponent(name, "motor");
    motorRight:unLock();
    motorRight:setMotorVelocity(speed);
end

function stopMotor(name)
    motorRight = Manager:getComponent(name, "motor");
    motorRight:setMotorOff();
    motorRight:lockInPlace();
end

function updateLeftMotorSpeed()
    if g_isLeftMotorOn then
        local speed = g_isReverse and g_walkingMotorSpeed or -g_walkingMotorSpeed;
        runMotor("walker-joint2-left", speed);
    end
end

function updateRightMotorSpeed()
    if g_isRighMotorOn then
        local speed = g_isReverse and g_walkingMotorSpeed or -g_walkingMotorSpeed;
        runMotor("walker-joint2-right", speed);
    end
end

function PhysicsPickingEventHandler(event)
    --print(event.entityName)
end

function InputEventHandler(event)
    --print(event.keyCode)

	-- Z
	if event.keyCode == 29 then
        if g_isLeftMotorOn then
            stopMotor("walker-joint2-left");
            g_isLeftMotorOn = false;
            print("Left leg OFF");
        else
            local speed = g_isReverse and g_walkingMotorSpeed or -g_walkingMotorSpeed;
            runMotor("walker-joint2-left", speed);
            g_isLeftMotorOn = true;
            print("Left leg ON");
        end
	end
    -- X
	if event.keyCode == 27 then
        if g_isRighMotorOn then
            stopMotor("walker-joint2-right");
            g_isRighMotorOn = false;
            print("Right leg OFF");
        else
            local speed = g_isReverse and g_walkingMotorSpeed or -g_walkingMotorSpeed;
            runMotor("walker-joint2-right", speed);
            g_isRighMotorOn = true;
            print("Right leg ON");
        end
	end

    -- C
	if event.keyCode == 6 then
        if g_isTurnMotorOn then
            stopMotor("walker-base");
            g_isTurnMotorOn = false;
            print("Turning OFF");
        else
            local speed = g_isReverse and g_turningMotorSpeed or -g_turningMotorSpeed;
            runMotor("walker-base", speed);
            g_isTurnMotorOn = true;
            print("Turning ON");
        end
	end
	-- V
	if event.keyCode == 25 then
    	g_isReverse = not g_isReverse;
	end

	-- KP 1
	if event.keyCode == 89 then
    	g_walkingMotorSpeed = 0.3;
    	updateLeftMotorSpeed();
    	updateRightMotorSpeed();
	end
	-- KP 2
	if event.keyCode == 90 then
    	g_walkingMotorSpeed = 0.8;
    	updateLeftMotorSpeed();
    	updateRightMotorSpeed();
	end
	-- KP 3
	if event.keyCode == 91 then
    	g_walkingMotorSpeed = 1.4;
    	updateLeftMotorSpeed();
    	updateRightMotorSpeed();
	end
end

Manager:registerEventHandler("InputEvent", InputEventHandler);
Manager:registerEventHandler("PhysicsPickingEvent", PhysicsPickingEventHandler);