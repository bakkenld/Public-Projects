"""IPv4 TCP/UDP python echo client"""

import socket
import networking
import messages

TESTING = 0

def python_client():
    """Attempts to connect to a server at HOST:PORT 
    HOST:PORT come from networking file
    Utilizes IPv4 addressing and a TCP connection"""
    tcp_or_udp = input("TCP or UDP?\n(T) for TCP or (U) for UDP\n -> ")
    if tcp_or_udp.lower().strip() == "t":
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as \
        tcp_client_socket:
            # socket.AF_INET specifies IPv4 address
            # socket.SOCK_STREAM specifies TCP connection
            # attempt to connect to the server
            try:
                tcp_client_socket.connect((networking.HOST, networking.PORT))
                print(f"Connected to server at \
                \n{networking.HOST}:{networking.PORT}")
            except ConnectionRefusedError:
                print(f"BONK\nThere was nothing found at \
                \n{networking.HOST}:{networking.PORT}.")
                return
            # prompt for input if connection is successful
            if TESTING:
                message = messages.test1 # chose 1-3
            else:
                message = input("What would you like to say? \
                \n[say [bye] to terminate the connection]\n -> ")
                while len(message) == 0:
                    message = input("Is that what you meant to send?\n -> ")
            while message.lower().strip() != "bye": #check if end condition
                networking.send_everything \
                (tcp_client_socket, networking.packet_stuffer(message))
                expected_length = networking.packet_unstuffer(tcp_client_socket)
                data = networking.recv_everything(tcp_client_socket, \
                expected_length)
                print("Received from server: \n" + data.decode())
                if TESTING:
                    message = "bye"
                else:
                    message = input("\n -> ") # Take next input
                    while len(message) == 0:
                        message = input("Is that what you meant to send?\n -> ")
        print("Connection closed.")
    elif tcp_or_udp.lower().strip() == "u":
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as \
        udp_client_socket:
            # socket.AF_INET specifies IPv4 address
            # socket.SOCK_DGRAM specifies UDP
            # attempt to send to the server
            print("You have 10 seconds to send data")
            if TESTING:
                message = messages.test2 # chose 1-3
            else:
                message = input("What would you like to say? \
                \n[say [bye] to terminate the connection]\n -> ")
                while len(message) == 0:
                    message = input("Is that what you meant to send?\n -> ")
            while message.lower().strip() != "bye": #check if end condition
                udp_client_socket.sendto(networking.packet_stuffer(message), \
                (networking.HOST, networking.PORT))
                data, server = udp_client_socket.recvfrom(networking.BUFFER)
                print("\nReceived back: \n" + data.decode('utf-8'))
                if TESTING:
                    message = "bye"
                else:
                    message = input("\n -> ") # Take next input
                    while len(message) == 0:
                        message = input("Is that what you meant to send?\n -> ")
        print("Connection closed")
    else:
        print("Try again")

if __name__ == '__main__':
    try:
        python_client()
    except KeyboardInterrupt:
        print("\nShutting down")
