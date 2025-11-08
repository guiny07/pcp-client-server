./server & 
SERVER_PID=$!

sleep 1 

N_CLIENTS=4

echo "Lançando $N_CLIENTS clientes..."
for((i=0; i <= N_CLIENTS; i++)); do
    ./client &
done


wait 

kill $SERVER_PID 2>/dev/null

echo "Concluído."