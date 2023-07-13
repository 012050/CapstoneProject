from flask import Flask, request, jsonify

app = Flask(__name__)

# @app.route("/object_detection/<userdevice>")
@app.route("/device/<board>", methods=['GET', 'POST'])
def object_detection(board):
    return board

# @app.route("/device/<board>", methods=['POST'])
# def count(board):
#     data = request.get_json()
#     print(data)
#     count = data.get('count')
#     fin = data.get('check')
#     if fin == "fin":
#         print("데이터 전송 완료")
#     else:
#         print(f"\n\n\n\n{count}")
#     return jsonify({'message': 'Success'})
