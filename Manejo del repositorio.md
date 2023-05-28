# Manejo del repositorio
En este documento se muestra la organización del presente repositorio, y cómo puede utilizarlo para ejecutar el proyecto.

## Estructura del repositorio
### Modelos
Comenzando por los modelos implementados en el proyecto, estos están dentro de la carpeta [Modelos](/Modelos). Dentro de esta carpeta, 
se encontrará con todos los modelos utilizados en el proyecto, organizados por carpetas individuales en donde cada una de ellas contiene
los archivos con extensión `mtl`, `obj`, algunos que incluyen un archivo con extensión `fbx`, además de las texturas utilizadas.

### Models
Con el fin de facilitar el uso, también se tiene la carpeta [Models](/Models), en la cual se encuentran todos los archivos `mtl` y `obj`
utilizados para el proyecto.

### Textures
Además, se incluye una carpeta de todas las [texturas](Textures) utilizadas en el proyecto, tanto de los modelos importados en Visual, hasta aquellas
texturas utilizadas directamente en código. Dentro de las Texturas, se tienen las texturas usadas para el [SkyBox](/Textures/Skybox), en la cual se
incluyen las imágenes en formato `tga` usadas para el skyBox del escenario tanto para el día como para la noche.

### Códigos y Shaders
En el repositorio se incluyen los [códigos](/Version1) y [Shaders](/Version1/shaders) utilizados en el proyecto (la última versión)

### Historial de versiones
Por último, se incluye una carpeta en la que se muestran los [códigos](Versiones) modificados, cuyo nombre al final indica a que versión pertenece
cada código.

## Control de versiones
Una vez revisado como se compone el repositorio, ahora se explicará como se manejó el control de versiones en él. En general, cada versión trataba
de hacer cambios significativos en cuanto a la versión anterior. En general, cada versión introduce los siguientes cambios:
- **Versión 1**
  - Carga de modelos iniciales en su posición original: Se comienza la carga y acomodo de los primeros modelos dentro del excenario. Modelos:
    - Cabaña de madera
    - Rocas
    - Árboles
    - Fuente o pozo
    - Caballo
    - Vaca
    - Valla
    - Vela
    - Arbusto o paja
    - Estrella
    - Baúl: cuerpo y tapa
  - SkyBox día: Se incluye el SkyBox usado para el día
  - Cámara isométrica: La cámara isométrica comienza a implementarse con el objetivo de visualizar mejor el primer acomodo de los modelos dentro del escenario
  - *Archivos:* [FinalProject.cpp](/Versiones/FinalProject.cpp), [Camera.cpp](/Version1/Camera.cpp), [Camera.h](/Version1/Camera.h)
- **Versión 2**
  - Inclusión de las primeras dos animaciones (complejas)
  - Separación de los modelos de la vaca y el caballo, y su importación
  - Implementación incompleta del cambio de la luz direccional de acuerdo a un ciclo muy rápido del día y noche
  - Por primitiva, se incluye 4 modelos usando una textura de pasto, colocados en el trayecto de la vaca
  - *Archivos:* [FinalProjectv2.cpp](/Versiones/FinalProjectv2.cpp), [DirectionalLight.cpp](/Version1/DirectionalLight.cpp), [DirectionalLight.h](/Version1/DirectionalLight.h)
- **Versión 3**
  - Inclusión de modelos:
    - Arco de piedra
    - Valla 2
    - Molino y hélice del molino
    - Lámpara o poste de luz
  - Inclusión del Skybox de noche, y de un ciclo de día/noche completo con una duración más larga a la de su versión antecedente
  - Iluminación por luces de tipo Point Light y uso de una Spot Light para prueba de iluminación
  - Rotación de la hélice del molino, antecedente para una primera animación simple
  - *Archivos:* [FinalProjectv3.cpp](/Versiones/FinalProjectv3.cpp)
- **Versión 3.1**
  - Correción de modelos:
    - Estrella
    - Arco de piedra
    - Valla 2
    - Reemplazo: Molino y hélice
    - Lámpara o poste de luz
  - Implementación del apagado de luces de tipo Point Light durante el día, y encendido durante la noche
  - *Archivos:* [FinalProjectv3-1.cpp](/Versiones/FinalProjectv3-1.cpp), [PointLight.cpp](/Versiones/PointLight.cpp)
- **Versión 4**
  - Inclusión de modelos:
    - Águila
    - Mariposa
  - Animación compleja: Mariposa
  - Animaciones simples: Águila y hélice del molino
  - Inclusión de una sexta Point Light (pozo)
  - Correción apagado de luces de tipo Point Light durante el día
  - *Archivos:* [FinalProjectv4.cpp](/Versiones/FinalProjectv4.cpp), [Windowv4.cpp](/Versiones/Windowv4.cpp), [Windowv4.h](/Versiones/Windowv4.h), [shader_lightv4.frag](/Versiones/shader_lightv4.frag)
- **Versión 5**
  - Animación por KeyFrames: Estrella
  - *Archivos:* [FinalProjectv5.cpp](/Versiones/FinalProjectv5.cpp)
- **Versión 5.1**
  - Luces de tipo Spot Light en el escenario
  - Inclusión del personaje principal (gato)
  - *Archivos:* [FinalProjectv5-1.cpp](/Versiones/FinalProjectv5-1.cpp), [Windowv5-1.cpp](/Versiones/Windowv5-1.cpp), [Windowv5-1.h](/Versiones/Windowv5-1.h), 

Con el fin de facilitar el uso del repositorio, dentro de [Versión 1](/Version1) se encuentran los códigos en su última versión,
los cuales se van actualizando de acuerdo a cada cambio que se va realizando.

Si desea consultar, en código, cuáles han sido los cambios más específicos, puede ir a un commit en especial, en donde puede observar sobre
que archivos está señalando el commit, y que cambios se visualizan (sobre todo para los códigos)

Si desea ejecutar el archivo, descargue el archivo [ejecutable](/EJECUTABLE.zip), y en la carpeta en donde lo descomprima, incluya las carpetas [Models](/Models) y [Textures](/Textures). Finalmente ejecute el archivo `EXE`
