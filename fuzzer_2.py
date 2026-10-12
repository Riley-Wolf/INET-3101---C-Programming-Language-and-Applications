import socket

HOST = "127.0.0.1"
PORT = 8080


def test_payload(name, payload):
    print(f"\n--- {name} ---")
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(2)
        sock.connect((HOST, PORT))
        sock.sendall(payload)

        try:
            response = sock.recv(1024)
            print("Response:", response.decode(errors="replace"))
        except socket.timeout:
            print("Response: No response received.")

        sock.close()
    except Exception as e:
        print("Error:", e)


# Path traversal and absolute paths
test_payload("Path Traversal", b"../../etc/passwd")
test_payload("Absolute Path", b"/etc/passwd")

# 10,000-byte payload with no newline or null terminator
test_payload("10,000-Byte Payload Flood", b"A" * 10000)

# Abrupt disconnect before sending data
print("\n--- Abrupt Disconnect: Before Sending ---")
try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))
    sock.close()
    print("Connected and closed without sending data.")
except Exception as e:
    print("Error:", e)

# Disconnect during transmission
print("\n--- Abrupt Disconnect: During Transmission ---")
try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))
    sock.sendall(b"B" * 100)
    sock.close()
    print("Sent partial payload and closed connection.")
except Exception as e:
    print("Error:", e)

# Null byte injection
test_payload("Null Byte Injection", b"solution.c\x00/etc/passwd")

print("\nFuzzing complete.")
