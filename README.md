# PokemonSockets
Sockets para capturar pokemos zukhulemptos ;P

instalar packetes de MySQL: sudo apt-get install libmysqlclient-dev
para instalar paquetes en fedora: sudo yum install mysql-devel

para instalar la base de datos, se debe de instalar mysql y correr el comando
mysql -u MiUsuarioCualquiera -p MiContraseñaCualquiera   < PokeBase.sql 

cambiar en el archivo PokemonClient.c el usuario y contraseña de MySQL, si no hay contraseña setear el valor a NULL, tiene que tener permisos de root
Para compilar ejecutar make en el directorio del proyecto

Para ejecutar el cliente usar ./clientPoke 127.0.0.1 9999
Para ejecutar el servidor usar ./TCPMainServer-Thread  

