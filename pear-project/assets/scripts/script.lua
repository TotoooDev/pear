time = 0.0

function on_start()
    pear.log.info("on start!")

    pear.transform.scale.x = 50.0
    pear.transform.scale.y = 50.0
    pear.transform.scale.z = 50.0
end

function on_update(timestep)
    -- pear.log.info("on update! " .. timestep)

    pear.transform.pos.y = math.sin(time * 0.005)
    pear.transform.rotation.y = pear.transform.rotation.y + timestep * 0.005

    time = time + timestep
end

function on_destroy()
    pear.log.info("on destroy!")
end
