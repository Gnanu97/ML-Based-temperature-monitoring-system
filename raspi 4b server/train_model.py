import numpy as np
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.models import load_model

# Input data: [temperature, humidity]
X = np.array([
    [30, 60],
    [35, 70],
    [25, 40],
    [28, 55],
    [33, 65],
])

# Output data: desired AC temperature
y = np.array([24, 20, 26, 25, 22])

# Define a simple neural network
model = Sequential([
    Dense(8, input_dim=2, activation='relu'),
    Dense(8, activation='relu'),
    Dense(1)  # Regression output
])

model.compile(optimizer='adam', loss='mean_squared_error')

# Train the model
model.fit(X, y, epochs=200, verbose=0)

# Save the trained model
model.save("nn_model.h5")

print("Neural network model trained and saved as nn_model.h5")
