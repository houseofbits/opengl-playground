print("Run test scrtipt")

function InputEventHandler(event)
    print(event.keyCode)
    c = Manager:getComponent("SpotLight", "LightComponent");
    if c then
        c.isEnabled = not c.isEnabled;
    end
end

Manager:registerEventHandler("InputEvent", InputEventHandler);