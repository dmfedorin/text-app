import socket
import threading

HOST = "192.168.1.186"
PORT = 15582

running = True
sock = 0

def client_recieve():
        while running: 
                data = sock.recv(1024)
                print(f"recieved {data!r}")

def client_send():
        global running

        while running:
                uin = input();
                sock.sendall(bytes(uin, "utf-8"))
                
                if (uin == "!exit"):
                        running = False

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))

recv_thread = threading.Thread(target = client_recieve, args = [])
send_thread = threading.Thread(target = client_send, args = [])

running = True

recv_thread.start()
send_thread.start()

while running:
        pass

recv_thread.join()
send_thread.join()
