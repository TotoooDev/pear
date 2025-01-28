-- from https://www.mbsoftworks.sk/tutorials/opengl4/026-camera-pt3-orbit-camera/

local move_camera = false
local move_target = false

local sensitivity = 0.1
local azimuth = 0.0
local polar = 0.0
local radius = 6.0
local target = {}

function on_start()
    target = pear.vec3.new(0.0)
end

function on_update(timestep)
    local sine_azimuth = math.sin(azimuth * (math.pi / 180.0))
    local cosine_azimuth = math.cos(azimuth * (math.pi / 180.0))
    local sine_polar = math.sin(polar * (math.pi / 180.0))
    local cosine_polar = math.cos(polar * (math.pi / 180.0))

    pear.transform.pos.x = target.x + radius * cosine_polar * cosine_azimuth
    pear.transform.pos.y = target.y + radius * sine_polar
    pear.transform.pos.z = target.z + radius * cosine_polar * sine_azimuth

    pear.transform.rotation.x = azimuth + 180
    pear.transform.rotation.y = -polar
end

function on_mouse_movement()
    if move_camera then
        azimuth = azimuth + pear.mouse.relative.x * sensitivity
        polar = polar + pear.mouse.relative.y * sensitivity
    end
    if move_target then
        local up = pear.camera.up
        local right = pear.camera.right

        target = target - right * pear.mouse.relative.x * sensitivity * radius * 0.01
        target = target + up * pear.mouse.relative.y * sensitivity * radius * 0.01
    end
end

function on_button_down()
    if not pear.editor.is_viewport_hovered then
        return
    end
    
    if pear.mouse.button.middle then
        move_camera = true
    end
    if pear.mouse.button.right then
        move_target = true
    end
end

function on_button_up()
    if not pear.mouse.button.middle then
        move_camera = false
    end
    if not pear.mouse.button.right then
        move_target = false
    end
end

function on_scroll(x, y)
    if not pear.editor.is_viewport_hovered then
        return
    end
    
    radius = radius - y * 0.5
end

function on_editor()
    radius = pear.editor.drag_number("radius", radius, 0.01, 0.0, math.huge)
    azimuth = pear.editor.drag_number("azimuth", azimuth, 1.0, -math.huge, math.huge)
    polar = pear.editor.drag_number("polar", polar, 1.0, -90.0 + 0.001, 90.0 - 0.001)
    target = pear.editor.drag_vec3("target", target, 0.1, -math.huge, math.huge)
end
