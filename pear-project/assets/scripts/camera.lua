local move_camera = false

function on_start()
    pear.transform.pos.x = -5.0
    pear.transform.pos.y = 1.0
end

function on_update(timestep)
    if pear.key.PEAR_KEY_LEFT_ALT then
        move_camera = not move_camera
    end

    if not move_camera then
        return
    end

    if pear.key.PEAR_KEY_W then
        pear.transform.pos.x = pear.transform.pos.x + pear.camera.front.x * 0.01 * timestep
        pear.transform.pos.y = pear.transform.pos.y + pear.camera.front.y * 0.01 * timestep
        pear.transform.pos.z = pear.transform.pos.z + pear.camera.front.z * 0.01 * timestep
    end

    if pear.key.PEAR_KEY_S then
        pear.transform.pos.x = pear.transform.pos.x - pear.camera.front.x * 0.01 * timestep
        pear.transform.pos.y = pear.transform.pos.y - pear.camera.front.y * 0.01 * timestep
        pear.transform.pos.z = pear.transform.pos.z - pear.camera.front.z * 0.01 * timestep
    end

    if pear.key.PEAR_KEY_A then
        pear.transform.pos.x = pear.transform.pos.x - pear.camera.right.x * 0.01 * timestep
        pear.transform.pos.y = pear.transform.pos.y - pear.camera.right.y * 0.01 * timestep
        pear.transform.pos.z = pear.transform.pos.z - pear.camera.right.z * 0.01 * timestep
    end

    if pear.key.PEAR_KEY_D then
        pear.transform.pos.x = pear.transform.pos.x + pear.camera.right.x * 0.01 * timestep
        pear.transform.pos.y = pear.transform.pos.y + pear.camera.right.y * 0.01 * timestep
        pear.transform.pos.z = pear.transform.pos.z + pear.camera.right.z * 0.01 * timestep
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
