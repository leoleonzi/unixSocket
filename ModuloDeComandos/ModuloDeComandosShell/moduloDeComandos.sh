# echo 7 func7 "/tmp/func7.sock" > moduloDeComandosNamedPipe

#mkfifo moduloDeComandosNamedPipe

touch listaDeComandos.txt
cat <> moduloDeComandosNamedPipe >> listaDeComandos.txt