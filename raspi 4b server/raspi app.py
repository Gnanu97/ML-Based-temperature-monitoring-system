from flask import Flask, request, jsonify
import numpy as np
from tensorflow.keras.models import load_model

app = Flask(__name__)

# Load the trained neural network model
model = load_model("nn_model.h5")

@app.route('/data', methods=['POST'])
def predict():
    try:
        data = request.get_json()
        temperature = float(data['temperature'])
        humidity = float(data['humidity'])

        # Prepare data and make prediction
        input_data = np.array([[temperature, humidity]])
        prediction = model.predict(input_data)[0][0]

        return jsonify({
            'predicted_temperature': round(prediction, 2)
        })

    except Exception as e:
        return jsonify({'error': str(e)}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
