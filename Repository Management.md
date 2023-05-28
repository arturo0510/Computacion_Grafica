# Repository Management

This document shows the organization of the current repository and how you can use it to run the project.

## Repository Structure
### Individual models
Starting with the models implemented in the project, they are located within the [models](/Modelos) folder. Inside this folder, you will find all the models
used in the project, organized in individual folders, each containing files with the extensions `mtl`, `obj`, some of which include an `fbx` file,
as well as the textures used.

### Models
To facilitate usage, there is also a folder named [Models](/Models) which contains all the `mtl` and `obj` files used in the project.

### Textures
Additionally, there is a folder containing all the [textures](Textures) used in the project, including textures for models imported in Visual,
as well as textures used directly in the code. Within the Textures folder, there are textures used for the [SkyBox](/Textures/Skybox), which
include `tga` format images used for the skybox of the scenario for both day and night.

### Codes and Shaders
The repository includes the [codes](/Version1) and [shaders](/Version1/shaders) used in the project (the latest version).

### Version History
Finally, there is a folder that displays the modified [codes](Versiones), where the name indicates the version to which each code belongs.

## Version Control
Now that we have reviewed the repository's composition, let's explain how version control was managed. In general, each version aimed to make significant
changes compared to the previous version. Each version introduces the following changes:

- **Version 1**
  - Initial loading of models in their original position: Loading and arrangement of the first models within the scene. Models:
    - Wooden cabin
    - Rocks
    - Trees
    - Fountain or well
    - Horse
    - Cow
    - Fence
    - Candle
    - Bush or straw
    - Star
    - Chest: body and lid
  - Day SkyBox: Includes the SkyBox used for the day
  - Isometric camera: The isometric camera starts to be implemented in order to better visualize the initial arrangement of models within the scene
  - *Files*: [FinalProject.cpp](/Versiones/FinalProject.cpp), [Camera.cpp](/Version1/Camera.cpp), [Camera.h](/Version1/Camera.h)
- **Version 2**
  - Inclusion of the first two (complex) animations
  - Separation of the cow and horse models, and their import
  - Incomplete implementation of changing the directional light according to a very fast day and night cycle
  - As a primitive, it includes 4 models using a grass texture, placed on the cow's path
  - *Files*: [FinalProjectv2.cpp](/Versiones/FinalProjectv2.cpp), [DirectionalLight.cpp](/Version1/DirectionalLight.cpp), [DirectionalLight.h](/Version1/DirectionalLight.h)
- **Version 3**
  - Inclusion of models:
    - Stone arch
    - Fence 2
    - Windmill and windmill propeller
    - Lamp or light post
  - Inclusion of the night SkyBox and a complete day/night cycle with a longer duration than the previous version
  - Lighting with Point Light and use of a Spot Light for lighting testing
  - Rotation of the windmill propeller, as a precedent to the first simple animation
  - *Files:* [FinalProjectv3.cpp](/Versiones/FinalProjectv3.cpp)
- **Version 3.1**
  - Correction of models:
    - Star
    - Stone arch
    - Fence 2
    - Replacement: Windmill and propeller
    - Lamp or light post
  - Implementation of turning off Point Light during the day and turning on during the night
  - *Files:* [FinalProjectv3-1.cpp](/Versiones/FinalProjectv3-1.cpp), [PointLight.cpp](/Versiones/PointLight.cpp)
- **Version 4**
  - Inclusion of models:
  - Eagle
  - Butterfly
  - Complex animation: Butterfly
  - Simple animations: Eagle and windmill propeller
  - Inclusion of a sixth Point Light (well)
  - Correction of turning off Point Light during the day
  - *Files*: [FinalProjectv4.cpp](/Versiones/FinalProjectv4.cpp), [Windowv4.cpp](/Versiones/Windowv4.cpp), [Windowv4.h](/Versiones/Windowv4.h), [shader_lightv4.frag](/Versiones/shader_lightv4.frag)
- **Version 5**
  - Animation by KeyFrames: Star
  - *Files:* [FinalProjectv5.cpp](/Versiones/FinalProjectv5.cpp)

To facilitate the use of the repository, within [Version 1](/Version1), you will find the codes in their latest version, which are updated
according to each change made.

If you want to check, the more specific changes that have been made, you can go to a specific commit, where you can see
the files the commit is referring to and the changes made (especially for the codes).


