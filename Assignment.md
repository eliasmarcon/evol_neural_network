Please use the galib examples 1-26 problems (also provided GALib-mpi library) to create an agent, that optimizes the parameters of a NeuralNet.

You can use any NN-library you like (python, C/C++) and any nontrivial Training/Test datasets, but keep in mind, that training takes time. I suggest to do something very simple like a classification of a subset (5 numbers) from the classical MNIST "handwritten numbers" dataset.

As fitness function you can use any of the following metrics. (just one is sufficient):

1. Accuracy and Loss (recommended)
Accuracy and loss are two common metrics used to measure how well a neural network predicts the correct outputs for a given set of inputs. Accuracy is the percentage of correct predictions, while loss measures how far off the predictions are from the actual values. These metrics can be used to compare different neural network architectures, hyperparameters, and training methods for a specific dataset.

2. Validation and Test Sets (recommended)
Validation and test sets are subsets of data not used for training the neural network but for evaluating its generalization ability. Validation sets are used to optimize hyperparameters and select the best model, while test sets are used to measure the final performance and prevent overfitting. You should always split your data into training, validation, and test sets before applying a neural network.

3. Confusion Matrix and ROC Curve (complex)
The confusion matrix and ROC curve are two visual tools for analyzing the performance of a neural network in a classification task. A confusion matrix shows the number of True Positives, False Positives, True Negatives, and False Negatives for each class, while an ROC curve represents the trade-off between the True Positive Rate and the False Positive Rate for different thresholds. These tools can be used to identify the strengths and weaknesses of your classifier and optimize its sensitivity and specificity.

4. Learning Curves and Early Stopping (complex)
Learning curves and early stopping are two techniques to monitor and control the learning process of a neural network. A learning curve plots the accuracy and loss values for the training and validation sets over epochs, while early stopping is a method to end training when the validation loss no longer improves or starts to increase. These techniques can be used to check for convergence and prevent overfitting or underfitting of your neural network.

5. Cross-Validation and Bootstrapping (complex)
Cross-validation and bootstrapping are two methods for estimating the reliability and variability of a neural network's performance on a limited or noisy dataset. Cross-validation is a technique where the dataset is divided into k folds, and each fold is used as a test set while the rest are trained, while bootstrapping is a technique where the dataset is sampled with replacement, and the training and testing process is repeated. These methods can be used to reduce the bias and variance of your performance estimates and increase their confidence intervals.