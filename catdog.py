from cv2 import imread, resize 
import numpy as np
import os
import random 
from tqdm import tqdm
from tkinter import Tk     
from tkinter.filedialog import askopenfilename 
import matplotlib.pyplot as plt
from time import sleep 
import tensorflow as tf 

# initialize paths for test and train folders  
test_dir = "C:/Users/hp/Desktop/dataset/test"
train_dir = "C:/Users/hp/Desktop/dataset/train"

# create variables for model name, image size and learning rate  
model_name = "my_model" 
image_size = 150 
learning_rate = 0.0001      

# function to get image label 
def cat_or_dog(image): 
    
    # get label 
    label = image.split(".")[0]
    
    # return 0 for cat, 1 for dog, otherwise return nothing
    if label == "cat":
        return 0 
    elif label == "dog":
        return 1 
    else:
        return None 
    
def process_or_train(operation): 
    
    # create lists for train and test data 
    train_data, test_data = [], []
    
    # initialize directory with an empty string   
    directory = "" 
    
    # get diretory 
    if operation == "train":
        directory = train_dir
    elif operation == "process": 
        directory = test_dir
    else: 
        return None 
    
    # process each image and store it in corresponding list 
    for image in tqdm(os.listdir(directory)): 
        
        # get each image's path 
        image_path = os.path.join(directory, image)  
        
        # image processing 
        new_image = resize(imread(image_path), (image_size, image_size)) 
        
        # append each image and its label to list 
        if operation == "train": 
            label = cat_or_dog(image)
            train_data.append([new_image, label])
            
        # append each image to list 
        else: 
            image_num = image.split(".")[0]
            test_data.append(new_image)
    # return train_data 
    if operation == "train": 
        return train_data 
    
    # return test_data 
    else:  
        return test_data  
    
# list of train data 
train_data = process_or_train("train") 

# list of test data 
test_data = process_or_train("process") 

# create a variable for the samples  
samples = 1000 

# negate its value 
samples_neg = samples * -1

# create variables for train and test 
train = train_data[:samples_neg]  
test = train_data[samples_neg:] 

# Samples for train 
X = np.array([item[0] for item in train])
Y = np.array([item[1] for item in train]) 

# Samples for test 
X_test = np.array([item[0] for item in test])
Y_test = np.array([item[1] for item in test]) 

# get model path 
model_path = "C:/Users/hp/Desktop/my_model" 

# get desktop path 
desktop_path = "C:/Users/hp/Desktop"

# if saved model is not found in target directory  
if model_name not in os.listdir(desktop_path):  
    
    # create the model 
    Model = tf.keras.models.Sequential([
            tf.keras.layers.Conv2D(128,kernel_size=(3,3),activation='relu',input_shape=(image_size,image_size,3)),
            tf.keras.layers.Conv2D(64,kernel_size=(3,3),activation='relu'),
            tf.keras.layers.MaxPool2D(4,4),

            tf.keras.layers.Conv2D(128,kernel_size=(3,3),activation='relu'),    
            tf.keras.layers.Conv2D(64,kernel_size=(3,3),activation='relu'),
            tf.keras.layers.MaxPool2D(4,4),

            tf.keras.layers.Flatten() ,    
            tf.keras.layers.Dense(512,activation='relu') ,    
            tf.keras.layers.Dropout(rate=0.3),
            tf.keras.layers.Dense(1, activation='sigmoid'),    
            ])
    
    # compile with given metrics
    Model.compile(optimizer = tf.keras.optimizers.Adam(learning_rate = learning_rate), loss='binary_crossentropy', metrics=['accuracy']) 
    
    # train the model 
    history = Model.fit(X, Y, epochs = 20)  
    
    # save the model 
    Model.save("my_model")
    
# load the model if it has already been saved 
else:
    Model = tf.keras.models.load_model(model_path) 
    
# function to make predictions 
def predict(): 
    
    # get uploaded image path 
    Tk().withdraw()  
    image = askopenfilename() 
    
    # create a list for prediction 
    prediction_list = [] 
    
    # image processing 
    final_image = resize(imread(image), (image_size, image_size)) 
        
    # turn it into a numpy array 
    prediction_list = np.array([final_image]) 

    # get prediction 
    prediction = Model.predict(prediction_list) 
    
    # draw the image z 
    plt.imshow(prediction_list[0])   
    plt.axis('off') 
    
    # get title based on the prediction 
    title = "Cat" if prediction[0][0] < 0.5 else "Dog"  
    plt.title(title)  
    
    # wait 
    plt.pause(0.0001) 
    
    # return nothing    
    return None  

# function to predict a random image from the dataset         
def predict_auto():
    
    # create a list for the prediction 
    prediction_list = []
    
    # get a random number 
    random_number = np.random.randint(len(test_data))
    
    # get a random image 
    random_image = test_data[random_number]
    
    # turn it into a numpy array     
    prediction_list = np.array([random_image]) 

    # make a prediction 
    prediction = Model.predict(prediction_list) 
    
    # draw the image 
    plt.imshow(prediction_list[0])   
    plt.axis('off') 
    
    # get title based on the prediction 
    title = "Cat" if prediction[0][0] < 0.5 else "Dog"  
    plt.title(title)   
    
    # wait
    plt.pause(0.0001) 

    # return nothing     
    return None  

# function to ask user to choose an option     
def ask(): 
    
    # infinite loop    
    while True: 
        
        # options
        print("Choose one of the options below: ") 
        print("1- I want to upload an image to be predicted.")  
        print("2- I want program to predict an image from dataset.") 
        print("3- Exit")    
        
        # initialize response with an empty string
        response = "" 
        
        # get input 
        while True : 
            try: 
                response = int(input("Select: "))
                if response in range(1, 4):
                    break 
                print("Choose either 1, 2 or 3")  
                continue  
            except:
                print("Input must be integer.")  
        
        # call function 
        if response == 1: 
            predict()   
            
            # wait for 1 second
            sleep(1)
            
        # call function
        elif response == 2: 
            predict_auto() 
            
            # wait for 1 second
            sleep(1)
                
        # terminate program
        else: 
            print("Program terminated. " ) 
            break           

# evaluate model 
evaluate = Model.evaluate(X_test, Y_test) 

# get loss and accuracy 
loss = evaluate[0] 
accuracy = evaluate[1] 

# print loss and accuracy 
print(f"Loss: {loss}\nAccuracy: {accuracy}") 

# call function    
ask() 
