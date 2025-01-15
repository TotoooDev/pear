local move_camera = false
local speed = 0.01

local keys = {
    forward = false,
    backward = false,
    left = false,
    right = false,
    up = false,
    down = false
}

function on_start()
    pear.transform.pos.x = -5.0
    pear.transform.pos.y = 1.0
end

function on_update(timestep)
    if not move_camera then
        return
    end

    if keys.forward then
        pear.transform.pos = pear.transform.pos + pear.camera.front * speed * timestep
    end

    if keys.backward then
        pear.transform.pos = pear.transform.pos - pear.camera.front * speed * timestep
    end

    if keys.left then
        pear.transform.pos = pear.transform.pos - pear.camera.right * speed * timestep
    end

    if keys.right then
        pear.transform.pos = pear.transform.pos + pear.camera.right * speed * timestep
    end

    if keys.up then
        pear.transform.pos.y = pear.transform.pos.y + speed * timestep
    end

    if keys.down then
        pear.transform.pos.y = pear.transform.pos.y - speed * timestep
    end

    pear.transform.rotation.x = pear.transform.rotation.x + pear.mouse.relative.x
    pear.transform.rotation.y = pear.transform.rotation.y - pear.mouse.relative.y
    if pear.transform.rotation.y > 89.0 then
        pear.transform.rotation.y = 89.0
    end
    if pear.transform.rotation.y < -89.0 then
        pear.transform.rotation.y = -89.0
    end
end

function on_key_press(key)
    if key == "PEAR_KEY_W" then
        keys.forward = true
    end
    if key == "PEAR_KEY_S" then
        keys.backward = true
    end
    if key == "PEAR_KEY_A" then
        keys.left = true
    end
    if key == "PEAR_KEY_D" then
        keys.right = true
    end
    if key == "PEAR_KEY_SPACE" then
        keys.up = true
    end
    if key == "PEAR_KEY_LEFT_CONTROL" then
        keys.down = true
    end
end

function on_key_release(key)
    if key == "PEAR_KEY_W" then
        keys.forward = false
    end
    if key == "PEAR_KEY_S" then
        keys.backward = false
    end
    if key == "PEAR_KEY_A" then
        keys.left = false
    end
    if key == "PEAR_KEY_D" then
        keys.right = false
    end
    if key == "PEAR_KEY_SPACE" then
        keys.up = false
    end
    if key == "PEAR_KEY_LEFT_CONTROL" then
        keys.down = false
    end

    if key == "PEAR_KEY_LEFT_ALT" then
        move_camera = not move_camera
        pear.window.lock_mouse(move_camera);
    end
end

function on_editor()
    speed = pear.editor.drag_number("speed", speed, 0.001, 0.0, math.huge)
    move_camera = pear.editor.checkbox("move camera", move_camera)
end
