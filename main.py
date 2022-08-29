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

        if cmd == 'pa':
            print('Participants added')
            print('-=' * 20)

        while True:
            prompt = input('Request result? [y/n]: ').strip().lower()
            print('-=' * 20)

            if prompt == 'y':
                ser.write(b'gv\n')

                cmd = ser.readline().decode()
                cmd = cmd.replace('\r\n', '')

                if cmd == 'gv':
                    for i in range(4):
                        participant = ser.readline().decode()
                        participant = participant.replace('\r\n', '')

                        name, votes = participant.split('=')

                        print(f'{name}: {votes}')

            print('-=' * 20)


main()
