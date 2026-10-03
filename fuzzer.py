import struct
import random

INPUT_FILE = "flight_data.bin"

STRUCT_FORMAT = "ii50s50s"
SEAT_SIZE = struct.calcsize(STRUCT_FORMAT)


def read_valid_file():
    with open(INPUT_FILE, "rb") as file:
        return bytearray(file.read())


def write_file(filename, data):
    with open(filename, "wb") as file:
        file.write(data)

    print(f"Created {filename}")


def truncated_file():
    data = read_valid_file()

    # Keep only 10 seats instead of all 48.
    corrupted = data[:SEAT_SIZE * 10]

    write_file("corrupt_truncated.bin", corrupted)


def oversized_file():
    data = read_valid_file()

    # Add extra bytes to the end of the file.
    corrupted = data + b"\xAA" * 100

    write_file("corrupt_oversized.bin", corrupted)


def garbage_name():
    data = read_valid_file()

    # First seat:
    # 4 bytes = ID
    # 4 bytes = assignment status
    # 50 bytes = first name
    name_offset = 8

    garbage = b"\xFF\xFE\x80\x01\x02\x03\x7F"

    corrupted = data[:]

    corrupted[name_offset:name_offset + len(garbage)] = garbage

    write_file("corrupt_garbage_name.bin", corrupted)


def invalid_seat_number():
    data = read_valid_file()

    corrupted = data[:]

    # Change first seat number to an invalid value.
    corrupted[0:4] = struct.pack("i", 999)

    write_file("corrupt_seat_number.bin", corrupted)


def invalid_status():
    data = read_valid_file()

    corrupted = data[:]

    # Valid status values are 0 and 1.
    corrupted[4:8] = struct.pack("i", 999)

    write_file("corrupt_status.bin", corrupted)


def random_corruption():
    data = read_valid_file()

    corrupted = data[:]

    # Randomly corrupt 20 bytes.
    for _ in range(20):
        position = random.randrange(len(corrupted))
        corrupted[position] = random.randrange(256)

    write_file("corrupt_random.bin", corrupted)


def main():
    print("Flight Data Binary Fuzzer")
    print("-------------------------")

    truncated_file()
    oversized_file()
    garbage_name()
    invalid_seat_number()
    invalid_status()
    random_corruption()

    print("\nFuzzing complete.")


if __name__ == "__main__":
    main()