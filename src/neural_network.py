import pandas as pd
import numpy as np
import sys
import os
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Dropout
from tensorflow.keras.initializers import Constant

def data():
    
    # Load the dataset
    df = pd.read_csv("./input/heart.csv")

    # Separate features and target
    X = df.drop("target", axis=1)
    y = df["target"]

    # Standardize the features
    scaler = StandardScaler()
    X_train_scaled = scaler.fit_transform(X)

    return X_train_scaled, y

def neuralnet(training_data, parameters):

    # Extract the weights and biases from the parameters array
    layer1_weights = parameters[0]["weights"]
    layer1_biases = parameters[0]["biases"]
    layer2_weights = parameters[1]["weights"]
    layer2_biases = parameters[1]["biases"]
    output_weights = parameters[2]["weights"]
    output_biases = parameters[2]["biases"]
    
    # print(f"layer1_weights:\n {layer1_weights}")
    # print(f"layer1_biases:\n {layer1_biases}")
    # print(f"layer2_weights:\n {layer2_weights}")
    # print(f"layer2_biases:\n {layer2_biases}")
    # print(f"output_weights:\n {output_weights}")
    # print(f"output_biases:\n {output_biases}")
          
    # Build the neural network model with weight initialization from the big arrays
    model = Sequential()
    model.add(Dense(64, activation='relu', input_shape=(training_data.shape[1],), kernel_initializer=Constant(value=layer1_weights), bias_initializer=Constant(value=layer1_biases)))
    model.add(Dropout(0.25))
    model.add(Dense(32, activation='relu', kernel_initializer=Constant(value=layer2_weights), bias_initializer=Constant(value=layer2_biases)))
    model.add(Dropout(0.25))
    model.add(Dense(1, activation='sigmoid', kernel_initializer=Constant(value=output_weights), bias_initializer=Constant(value=output_biases)))

    # Compile the model
    model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])

    return model

def main(parameters, layer_sizes): 

    # Print input
    # print(f"parameters:\n {parameters}")
    # print(f"layer_sizes:\n {layer_sizes}")

    # Format parameters
    formatted_params = format_nn_parameters(parameters, layer_sizes)
    
    # print(formatted_params)
    
    # Load data
    X_train_scaled, y = data()
    
    # Create model
    model = neuralnet(X_train_scaled, formatted_params)
    
    # Evaluate the model on the test set
    test_loss, test_acc = model.evaluate(X_train_scaled, y)
    
    # print(f"\nTest Loss: {test_loss}")
    # print(f"Test Accuracy: {test_acc}\n")
    
    return test_loss, test_acc


def format_nn_parameters(parameters, layer_sizes):
    formatted_params = []
    param_index = 0

    for i in range(len(layer_sizes) - 1):
        layer_params = {
            "weights": [],
            "biases": []
        }

        # Extract weights for the current layer
        for _ in range(layer_sizes[i]):
            layer_params["weights"].append(parameters[param_index])
            param_index += 1

        # Extract biases for the next layer
        layer_params["biases"] = parameters[param_index]
        param_index += 1

        formatted_params.append(layer_params)

    return formatted_params
