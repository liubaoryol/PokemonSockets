# PokemonSockets
Sockets para capturar pokemos zukhulemptos

instalar packetes de MySQL: sudo apt-get install libmysqlclient-dev
para instalar paquetes en fedora: sudo yum install mysql-devel

para instalar la base de datos, se debe de instalar mysql y correr el comando
mysql -u MiUsuarioCualquiera -p MiContraseñaCualquiera   < PokeBase.sql 

cambiar en el archivo PokemonClient.c el usuario y contraseña de MySQL, si no hay contraseña setear el valor a NULL, tiene que tener permisos de root
Para compilar el cliente usar gcc PokemonClient.c -o clientPoke `mysql_config --cflags --libs`
Para compilar el cliente en fedora usar gcc PokemonClient.c -o clientPoke $(mysql_config --cflags --libs)

Para compilar el servidor usar gcc  -pthread TCPMainServer-Thread.c -o TCPMainServer-Thread

Para ejecutar el cliente usar ./clientPoke 127.0.0.1 9999
Para ejecutar el servidor usar ./TCPMainServer-Thread  

