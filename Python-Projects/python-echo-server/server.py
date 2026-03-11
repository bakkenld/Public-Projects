"""IPv4 TCP/UDP python echo server"""

import socket
import networking

def python_server():
    """Attempts to create a server at HOST:PORT 
    HOST:PORT come from networking file
    Utilizes IPv4 addressing and a TCP connection"""
    tcp_or_udp = input("TCP or UDP?\n(T) for TCP or (U) for UDP\n -> ")
    if tcp_or_udp.lower().strip() == "t":
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as \
        tcp_server_socket:
            # socket.AF_INET specifies IPv4 address
            # socket.SOCK_STREAM specifies TCP connection
            print(f"Starting on {networking.HOST}:{networking.PORT}")
            try:
                tcp_server_socket.bind((networking.HOST, networking.PORT))
                print(f"TCP listening on {networking.HOST}:{networking.PORT}")
                # bind server to HOST:PORT
            except socket.error as bonk:
                print(f"Bind failed. Error: {bonk}")
                return
            # listen for connections
            tcp_server_socket.listen()
            print("Server is waiting for connections...")
            # recv connection from client
            conn, addr = tcp_server_socket.accept()
            with conn:
                print(f"Connected by {addr}") # announce connection
                while True:
                    try:
                        expected_length = networking.packet_unstuffer(conn)
                    except TypeError:
                        break
                    data = networking.recv_everything(conn, expected_length)
                    if not data:
                        print(f"Client {addr} disconnected.")
                        break
                    print(f"Received from client:\n {data.decode('utf-8')}")
                    print(f"Echoing back to client:\n {data.decode('utf-8')}")
                    networking.send_everything(conn, networking.packet_stuffer\
                    (data.decode('utf-8'))) # accounting for partial send/recv
        print("Connection closed")
    elif tcp_or_udp.lower().strip() == "u":
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as \
        udp_server_socket:
            # socket.AF_INET specifies IPv4 address
            # socket.SOCK_DGRAM specifies UDP connection
            print(f"Starting on {networking.HOST}:{networking.PORT}")
            udp_server_socket.settimeout(10)
            try:
                udp_server_socket.bind((networking.HOST, networking.PORT))
                print(f"UDP listening on {networking.HOST}:{networking.PORT}\
                \nWaiting 10 seconds for client")
            except socket.error as bonk:
                print(f"Bind failed. Error: {bonk}")
                return
            while True:
                try:
                    data, client = udp_server_socket.recvfrom(networking.BUFFER)
                    print(f"Received from client: \n {data.decode('utf-8')}")
                    print(f"Echoing back to client: \n {data.decode('utf-8')}")
                    udp_server_socket.sendto(data, client)
                except TimeoutError:
                    print("Server timed out.")
                    break
        print("Connection Closed")
    else:
        print("Try again")

if __name__ == '__main__':
    try:
        python_server()
    except KeyboardInterrupt:
        print("\nShutting down")
