set -x
./udp_server &
SERVER_PID=$!
sleep 2
./client &
CLIENT_PID=$!
echo "Running both server and client"
sleep 10
echo "Stopping both server and client"
kill $CLIENT_PID 2>/dev/null
kill $SERVER_PID 2>/dev/null
wait $CLIENT_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null
echo "Both processes stopped."
