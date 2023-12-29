import numpy as np
import sys
import pandas as pd

#from tensorflow.keras.utils import to_categorical
#from tensorflow.keras import Model, layers

def data():
    
    training_data = pd.read_csv("./input/train.csv")

    X_train = training_data.iloc[:,1:].values.astype('float32')
    X_train = X_train.reshape(X_train.shape[0], 28, 28, 1)

    # Normalize pixel values to [0, 1]
    X_train = X_train / 255.0

    # Labels
    y_train = training_data.iloc[:, 0].values.astype('int32')

    # Convert labels to one-hot encoded format
    y_train = to_categorical(y_train, num_classes = 10)  # Classes for the numbers 0 to 9

    return X_train, y_train

def neuralnet():

    images = layers.Input(shape=(28, 28, 1), name='input')

    # Convolution 1
    conv_1_1 = layers.Conv2D(filters=16, kernel_size=(3, 3), strides=1, padding='same', name='conv_1_1')(images)
    conv_1_1 = layers.BatchNormalization(name='conv_1_1_batchnorm')(conv_1_1)
    conv_1_1 = layers.ReLU(name='conv1_1_relu')(conv_1_1)

    conv_1_2 = layers.Conv2D(filters=32, kernel_size=(3, 3), padding='same', name='conv1_2')(conv_1_1)
    conv_1_2 = layers.BatchNormalization(name='conv1_2_batchnorm')(conv_1_2)
    conv_1_2 = layers.ReLU(name='conv1_2_relu')(conv_1_2)

    conv_1_3 = layers.MaxPool2D(pool_size=(2, 2), name='conv1_3')(conv_1_2)


    # Convolution 2
    conv_2_1 = layers.Conv2D(filters=48, kernel_size=(3, 3), padding='same', name='conv2_1')(conv_1_3)
    conv_2_1 = layers.BatchNormalization(name='conv2_1_batchnorm')(conv_2_1)
    conv_2_1 = layers.ReLU(name='conv2_1_relu')(conv_2_1)

    conv_2_2 = layers.Conv2D(filters=64, kernel_size=(3, 3), padding='same', name='conv2_2')(conv_2_1)
    conv_2_2 = layers.BatchNormalization(name='conv2_2_batchnorm')(conv_2_2)
    conv_2_2 = layers.ReLU(name='conv2_2_relu')(conv_2_2)

    conv_2_3 = layers.MaxPool2D(pool_size=(2, 2), name='conv2_3')(conv_2_2)


    # Flatten
    flatten = layers.Flatten()(conv_2_3)


    # Dense 1
    dense_1 = layers.Dense(128, activation='relu', name='dense_1')(flatten)
    dense_1 = layers.Dropout(0.5)(dense_1)  # Add dropout here


    # Dense 2
    dense_2 = layers.Dense(10, activation='softmax', name='dense_2')(dense_1)


    # Final Model
    model = Model(inputs=images, outputs=dense_2)

    return model

if __name__ == "__main__":

    # get input 
    # if len(sys.argv) != 2:
    #     print("Usage: python cnn.py <input>")
    #     sys.exit(1)
    # else:
    #     input = sys.argv[1]

    print("Hello")

    # Load data
    X_train, y_train = data()

    # Create model
    #model = neuralnet()

    # # Manually set random weights
    # for layer in model.layers:
    #     if hasattr(layer, 'kernel_initializer') and layer.kernel_initializer == 'random_normal':
    #         weights = [np.random.randn(*w.shape) for w in layer.get_weights()]
    #         layer.set_weights(weights)

    # Compile model
    #model.compile(loss = "categorical_crossentropy",
    #            optimizer = tf.keras.optimizers.Adam(learning_rate = 0.001),
    #            metrics = ['accuracy'])
    
    # Model predict
    #predictions = model.predict(X_train)

