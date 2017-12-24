# PokemonSockets
Sockets para capturar pokemos zukhulemptos


git init
.
.
git clone <link>
.
.
git add .
.
.
git commit -m <mesaje cagado>
.
.
git push origin master
.
.
git
.
.
instalar packetes de MySQL: sudo apt-get install libmysqlclient-dev
cambiar en el archivo PokemonClient.c el usuario y contraseña de MySQL, tiene que tener permisos de root
Para compilar el cliente usar gcc PokemonClient.c -o clientPoke `mysql_config --cflags --libs`


