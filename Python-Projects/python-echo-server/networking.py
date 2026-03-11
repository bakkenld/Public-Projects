"""IPv4 TCP/UDP python networking"""

import socket
import struct

# Config
HOST = "127.0.0.1"  # Server (IPv4)
PORT = 45454        # Port
BUFFER = 1024       # Buffer size (bytes)
HEADER_FORMAT = '!L'# Header format (! = network; L = unsigned long)
HEADER_SIZE = struct.calcsize(HEADER_FORMAT)
# Partial sends function:
def send_everything(sock, message):
    """ Uses sock and message passed from client/server
    Tries to send full message byte by byte
    reports total number of bytes sent. """
    total_sent = 0
    data_length = len(message)
    # Loop until all bytes sent
    while total_sent < data_length:
        try:
            sent = sock.send(message[total_sent:]) # Send remaining data
        except socket.error as err:
            print("\nBonk.\nError occured:", err)
            return None #none to indicate network operation failed
        if sent == 0:
            print("Connection closed (sending).") # Connection closed during
            return None
        total_sent += sent
        print(f"Sent {sent} bytes. Total progress: {total_sent}/{data_length}")
    return total_sent
# Partial recv function:
def recv_everything(sock, n_bytes):
    """ Uses sock and message passed from client/server
    Tries to recv full message byte by byte
    reports total number of bytes recv'd. """
    heard = b''
    while len(heard) < n_bytes:
        try:
            chunk = sock.recv(n_bytes - len(heard))
        except socket.error as err:
            print("\nBonk.\nError occured:", err)
            return None #none to indicate network operation failed
        if not chunk:
            print("Peer terminated connection [bye]") # Connection closed during
            return None
        heard += chunk
    return heard
# Packing packets
def packet_stuffer(message):
    """ Usees struct to create the packet"""
    packet = struct.pack(HEADER_FORMAT, len(message)) + message.encode('utf-8')
    return packet
# Unpacking packets
def packet_unstuffer(sock):
    """ Usees struct to unpack the packet"""
    header_data = recv_everything(sock, HEADER_SIZE)
    if header_data == 0:
        print("Connection closed by peer (us).")
        return None
    try:
        message_length = struct.unpack(HEADER_FORMAT, header_data)[0]
    except struct.error as err:
        print(f"error unpacking: {err}")
        return None
    print(f"recieved message of length: {message_length}")
    return message_length
