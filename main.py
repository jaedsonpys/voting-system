import serial


def main():
    ser = serial.Serial('/dev/ttyUSB0')
    print(f'Connect to "{ser.name}"')

    cmd = ser.readline().decode()
    cmd = cmd.replace('\r\n', '')

    print('-=' * 15)

    if cmd == 'wp':
        participants = []

        for i in range(4):
            name = input(f'Partipant {i + 1}: ').strip()
            participants.append(name)

        print('-=' * 20)

        participants_string = ','.join(participants)
        ser.write((participants_string + '\n').encode())

        cmd = ser.readline().decode()
        cmd = cmd.replace('\r\n', '')


main()
