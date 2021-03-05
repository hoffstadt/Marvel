"use strict";

import mvGraphics from "./mvGraphics.mjs";
import mvSphere from "./drawables/mvSphere.mjs";
import mvPointLight from "./mvPointLight.mjs";

let graphics = new mvGraphics("glcanvas");

let spheres = [];

for(let i = -3; i < 3; i++){
    for(let j = -2; j < 2; j++)
    {
        let sphere = new mvSphere(graphics, 0.5, 1);
        sphere.x = i*2;
        sphere.y = j*2;
        sphere.z = -15;
        spheres.push(sphere);
    }
}

let sphere = new mvSphere(graphics, 1, 1);
sphere.x = 0;
sphere.y = 0;
sphere.z = -15;

let light = new mvPointLight(graphics, [0, 0, -8]);

let x = 0;
let y = 0;
let down = false;

document.onkeydown = function(e){
    if(e.key === "w")
        camera.translate(0, 0, -0.2);
    else if(e.key === "s")
        camera.translate(0, 0, 0.2);
    else if(e.key === "a")
        camera.translate(-0.2, 0, 0);
    else if(e.key === "d")
        camera.translate(0.2, 0, 0);
    else if(e.key === "r")
        camera.translate(0, 0.2, 0);
    else if(e.key === "f")
        camera.translate(0, -0.2, 0);
}

document.onmousedown = function(e){
    down = true;
    x = e.offsetX;
    y = e.offsetY;
}

document.onmouseup = function(e){
    down = false;
}

document.onmousemove = function(e){
    if(down) {
        camera.rotate(-(e.offsetY-y) * 0.001, -(e.offsetX-x) * 0.001, 0);
        x = e.offsetX;
        y = e.offsetY;
    }
}

let target = graphics.getRenderTarget();
let camera = graphics.getCamera();

let frame_rate = document.getElementById("frame_rate");

let then = 0;
function render(now) {

    now *= 0.001;                          // convert to seconds
    const deltaTime = now - then;          // compute time since last frame
    then = now;                            // remember time for next frame
    const fps = 1 / deltaTime;             // compute frames per second

    frame_rate.value = fps.toFixed(1);  // update fps display;

    light.setPosition(6*Math.cos(now), 2*Math.sin(now), -15 + 5*Math.sin(now));

    target.bind(graphics);

    camera.bind(graphics);

    light.bind(graphics, camera.getMatrix());
    light.draw(graphics);

    for(let i = 0; i < spheres.length; i++)
    {
        light.bind(graphics, camera.getMatrix());
        spheres[i].bind(graphics);
        spheres[i].draw(graphics);
    }

    sphere.setPosition(5*Math.sin(now), 5*Math.sin(now), -20);
    light.bind(graphics, camera.getMatrix());
    sphere.bind(graphics);
    sphere.draw(graphics);

    requestAnimationFrame(render);
}
requestAnimationFrame(render);

