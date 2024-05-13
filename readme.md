## Playing around with various OpenGL rendering techniques.

Dependencies for Linux

`sudo apt install libglm-dev`

`sudo apt install libsdl2-2.0-0`

`sudo apt install libsdl2-dev`

`sudo apt install libglew-dev`

History

-   Normal mapping and basic specular maps.

    <p float="left">
      <img src="./docs/images/normal-maps.png" width="400" />
    </p>

-   Multiple point and spot lights casting shadows. Light projection texture. Texture atlases.

    <p float="left">
      <img src="./docs/images/multilight-scene.png" width="400" />
      <img src="./docs/images/shadow-atlas.png" width="400" />
    </p>

-   Multiple spot lights casting shadows.

    <p float="left">
      <img src="./docs/images/multiple-spot-shadows.png" width="400" />
    </p>

-   Multiple point and spot lights with shadow cast from one of the spot lights. Shadow has PCF filtering.

    <p float="left">
      <img src="./docs/images/spot-light-shadow-map.png" width="400" />
    </p>

