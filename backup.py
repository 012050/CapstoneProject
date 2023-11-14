from flask import Flask, request

app = Flask(__name__)

motor_status = "stand"

@app.route('/test/<code>')
def motor_command(code):
    global motor_status
    if code == "on":
        motor_status = "stand"
    elif code == "off":
        motor_status = "sit"

    print(motor_status)
    return str(motor_status)

@app.route('/command')
def motor_controle():
    return str(motor_status)

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000, debug=True)
