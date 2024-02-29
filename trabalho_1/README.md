# Controle de semaforo em um cruzamento (2023-2)

Nome: Alex Gabriel Alves Faustino  

# como rodar

## Para rodar o servidor distribuido
entre na subpasta <./distribuido> e execute o seguinte comando

```console
make all  
```
apos compilar execute  

```
make run porta=<numero da porta>  ip=<numero do ip> cruzamento=<numero do cruzamento>
```
Lembre-se de substituir `<numero_da_porta>`, `<numero_do_ip>` e `<numero_do_cruzamento>` pelos valores específicos que você deseja fornecer.


## Para rodar servidor central 
entre na subpasta <./central> e execute o seguinte comando

```console
make all  
```
apos compilar execute  

```
make run porta1=<numero da porta> ip1=<numero do ip> porta2=<numero da porta> ip2=<numero do ip>
```
Lembre-se de substituir `<numero_da_porta>` e  `<numero_do_ip>` pelos valores específicos que você deseja fornecer.

