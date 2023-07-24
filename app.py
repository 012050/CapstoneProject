from flask import Flask, request
from ImageProcess import save_img, read_img

app = Flask(__name__)

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

    return str(result), 200
