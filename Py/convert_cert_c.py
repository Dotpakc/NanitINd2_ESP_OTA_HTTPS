import os

PATH_DATA_FOLDER = os.path.join(os.path.dirname(__file__), 'data')
PATH_CERT_PEM = os.path.join(PATH_DATA_FOLDER, 'github.pem')

print(PATH_CERT_PEM)

text ='const char* rootCACertificate PROGMEM = \\'

with open(PATH_CERT_PEM, 'r') as f:
    for line in f:
        text += '\n'
        text += '    "' + line.strip() + '\\n" \\'
    
text = text[:-2]
text += ';'

with open(os.path.join(PATH_DATA_FOLDER, 'github.h'), 'w') as f:
    f.write(text)