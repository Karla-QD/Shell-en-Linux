# Shell-en-Linux

## Proyecto del Curso de Sistemas Operativos 

El proyecto consiste en el desarrollo de un Shell para Linux utilizando el lenguaje de programación C, con las siguientes funcionalidades destacadas:

### 1. Compatibilidad Total con Comandos de Linux:

El Shell es capaz de ejecutar cualquier comando propio del sistema operativo Linux, proporcionando una interfaz completa y funcional para el usuario.

### 2. Historial de Comandos:

Implementa un historial que almacena los últimos 35 comandos ejecutados. Se facilita la recuperación de comandos anteriores mediante la introducción de una posición específica seguida del signo de exclamación, como en el ejemplo !n.

### 3. Ejecución de Comandos en Segundo Plano:

Permite la ejecución de comandos en segundo plano mediante el uso de funciones fork. Para lograr esto, el usuario coloca el comando a ejecutar en segundo plano seguido de '&' y al presionar enter. Posteriormente, puede ingresar otros comandos que se ejecutarán en primer plano.

### 4. Soporte para un Pipe:

Facilita el uso de pipes para la creación de flujos de datos entre comandos. Por ejemplo, la ejecución de ls | sort realiza una operación de pipe entre los comandos ls y sort.


Este proyecto no solo proporciona un Shell completamente funcional, sino que también representa una valiosa oportunidad de aprendizaje integral. A través de la implementación de conceptos fundamentales de sistemas operativos, programación en C y gestión de procesos.
