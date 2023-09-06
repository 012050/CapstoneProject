from flask import Flask, request
from ImageProcess import *

app = Flask(__name__)

test_code = "hello world"

# 사진을 저장할 폴더
img_dir = "Images"

UPLOAD_FOLDER = img_dir
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

@app.route('/upload/<board>', methods=['POST'])
def upload_photo(board):
    received = request
    result = 0
    if received.files:
        file  = received.files['imageFile']
        save_img(file, board, img_dir=app.config['UPLOAD_FOLDER'])
        result = read_img(board)
        print(f"[SUCCESS] Image Received, result: {result}")
        # return f"[SUCCESS] Image Received, result: {result}", 201
    else:
        print("[FAILED] Image Not Received")
        return "[FAILED] Image Not Received", 204

    return str(test_code)
    # return str(result), 200

@app.route('/test/<code>')
def motor_command(code):
    global test_code
    if code == "on":
        test_code = "hello world"
    elif code == "off":
        test_code = "bye world"

    print(test_code)
    return str(test_code)



if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000, debug=True)
