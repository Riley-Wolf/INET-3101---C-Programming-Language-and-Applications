import socket
import struct
import time
import random

# Configuration matching our C server settings
SERVER_IP = "127.0.0.1"
SERVER_PORT = 8080
ACCOUNT_ID = 1001

# This matches the C layout exactly: uint32, uint32, uint32, double, uint32
# Total structure size = 24 bytes
C_STRUCT_FORMAT = "=IIIdI"

def send_mock_metrics():
    print(f"🚀 [STRESS TESTER] Starting stream to C Billing Server...")
    
    try:
        # Open a TCP network socket connection to our C binary app
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((SERVER_IP, SERVER_PORT))
        print("✅ Connected to C server successfully.")
        
        # Simulate blasting 5 consecutive usage tracking metric packets
        for i in range(1, 6):
            cpu = random.randint(2, 8)
            ram = random.randint(8, 32)
            outbound_mb = float(random.randint(100, 1500))
            hours = random.randint(5, 24)
            
            print(f"\n📦 Sending Metric Packet #{i}:")
            print(f"   CPU Cores: {cpu} | RAM: {ram}GB | Outbound Traffic: {outbound_mb}MB | Hours: {hours}")
            
            # Pack the Python variables into raw data bytes matching the C struct format
            packed_data = struct.pack(C_STRUCT_FORMAT, ACCOUNT_ID, cpu, ram, outbound_mb, hours)
            
            # Fire the raw packet across the network stream socket
            client_socket.sendall(packed_data)
            time.sleep(0.5) # Short pause between metrics data packages
            
        client_socket.close()
        print("\n🏁 [STRESS TESTER] Stream complete. Sockets closed smoothly.")
        
    except ConnectionRefusedError:
        print("❌ Error: Could not connect to the C server. Make sure it is running first!")

if __name__ == "__main__":
    send_mock_metrics()