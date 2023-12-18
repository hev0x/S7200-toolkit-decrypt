import optparse 

parser = optparse.OptionParser()
parser.add_option('-f', '--file', action="store", dest="file", help="Base binary file: CPU200.bin")

options, args = parser.parse_args()

if not options.file:

    print('[+] Specify an file target')
    print('[+] Example usage: S7200decrypt.py -f CPU200.bin')
    print('[+] Example help usage: S7200decrypt.py -h')
    exit()

char_map = {
    '8A': ' ','8B': '!','88': '“','89': '#','8E': '$',
    '8F': '%','8C': '&','8D': '’','82': '(', '83': ')',
    '80': '*','81': '+','86': ',','87': '-','84': '.',
    '85': '/','9A': '0','9B': '1','98': '2','99': '3',
    '9E': '4','9F': '5','9C': '6','9D': '7','92': '8',
    '93': '9','90': ':','91': ';','96': '<','97': '=',
    '94': '>','95': '?','EA': '@','EB': 'A','E8': 'B',
    'E9': 'C','EE': 'D','EF': 'E','EC': 'F','ED': 'G',
    'E2': 'H','E3': 'I','E0': 'J','E1': 'K','E6': 'L',
    'E7': 'M','E4': 'N','E5': 'O','FA': 'P','FB': 'Q',
    'F8': 'R','F9': 'S','FE': 'T','FF': 'U','FC': 'V',
    'FD': 'W','F2': 'X','F3': 'Y','F0': 'Z','F1': '[',
    'F6': '\\','F7': ']','F4': '^','F5': '_','CA': '`',
    'CB': 'a','C8': 'b','C9': 'c','CE': 'd','CF': 'e',
    'CC': 'f','CD': 'g','C2': 'h','C3': 'i','C0': 'j',
    'C1': 'k','C6': 'l','C7': 'm','C4': 'n','C5': 'o',
    'DA': 'p','DB': 'q','D8': 'r','D9': 's','DE': 't',
    'DF': 'u','DC': 'v','DD': 'w','D2': 'x','D3': 'y',
    'D0': 'z','D1': '{','D6': '|','D7': '}','D4': '~',
    'D5': 'D5','6A': 'А','6B': 'Б','68': 'В','69': 'Г',
    '6E': 'Д','6F': 'Е','6C': 'Ж','6D': 'З','62': 'И',
    '63': 'Й','60': 'К','61': 'Л','66': 'М','67': 'Н',
    '64': 'О','65': 'П','7A': 'Р','7B': 'С','78': 'Т',
    '79': 'У','7E': 'Ф','7F': 'Х','7C': 'Ц','7D': 'Ч',
    '72': 'Ш','73': 'Щ','70': 'Ъ','71': 'Ы','76': 'Ь',
    '77': 'Э','74': 'Ю','75': 'Я','4A': 'а','4B': 'б',
    '48': 'в','49': 'г','4E': 'д','4F': 'е','4C': 'ж',
    '4D': 'з','42': 'и','43': 'й','40': 'к','41': 'л',
    '46': 'м','47': 'н','44': 'о','45': 'п','5A': 'р',
    '5B': 'с','58': 'т','59': 'у','5E': 'ф','5F': 'х',
    '5C': 'ц','5D': 'ч','52': 'ш','53': 'щ','50': 'ъ',
    '51': 'ы','56': 'ь','57': 'э','54': 'ю','55': 'я',
    '02': 'Ё','13': '№','12': 'ё'
}

#...
def decrypt_password(binary_data):

    try:
        protectionLevel = binary_data[0x1E75]
        rawBytes = binary_data[0x1E76:0x1E7E]

        rawXOR1 = binary_data[0x1E76] ^ binary_data[0x1E78]
        rawXOR2 = binary_data[0x1E77] ^ binary_data[0x1E79]
        rawXOR3 = binary_data[0x1E78] ^ binary_data[0x1E7A]
        rawXOR4 = binary_data[0x1E79] ^ binary_data[0x1E7B]
        rawXOR5 = binary_data[0x1E7A] ^ binary_data[0x1E7C]
        rawXOR6 = binary_data[0x1E7B] ^ binary_data[0x1E7D]

        XOR1 = char_map[f'{rawXOR1:02X}']
        XOR2 = char_map[f'{rawXOR2:02X}']
        XOR3 = char_map[f'{rawXOR3:02X}']
        XOR4 = char_map[f'{rawXOR4:02X}']
        XOR5 = char_map[f'{rawXOR5:02X}']
        XOR6 = char_map[f'{rawXOR6:02X}']

        decryptPasswd = [char_map[f'{rawBytes[0]:02X}'], char_map[f'{rawBytes[1]:02X}'], XOR1,XOR2, XOR3, XOR4, XOR5, XOR6]

        return protectionLevel, rawBytes, decryptPasswd
    
    except Exception as e:
        return None, None, str(e)

#File Handling
input_file = options.file


try:
    with open(input_file, 'rb') as file:
        binary_data = file.read()
except FileNotFoundError:
    print(f"The file '{input_file}' was not found.")
    exit(1)

protectionLevel, rawBytes, decryptPasswd = decrypt_password(binary_data)

if protectionLevel is not None:
    print('[-] S7-200 Decrypt password\r\n')
    print(f"Protection level: {protectionLevel}")
    print(f"Raw bytes read: {' '.join([f'{byte:02X}' for byte in rawBytes])}")
    print("Password decrypted: " , *decryptPasswd)
        
else:
    print("Error decrypting password.")


