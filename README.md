# ForegroundSegmentation
To develop a basic foreground segmentation algorithm to detect moving objects on a stationary background; To improve the performance of the basic algorithm; To evaluate the performance of foreground segmentation algorithms using standard measures 


--------
- task1:
  - Generation of foreground segmentation mask for each video frame using “frame difference” 
  - Progressive update of background model through selective running average (blind and selective update)
  - Suppression of stationary objects that appear or are removed

- task2:
  - Implementation of a shadow suppression module using the OpenCV library
    - Suppression of shadows based on the chromaticy-based method

- task3:
  - Implementation of advanced background subtraction algorithms
    - Single Gaussian
    - Gaussian mixture model
    
    
- task4:
  - Evaluate your algorithm using the [ChangeDetection datasets](http://jacarini.dinf.usherbrooke.ca/dataset2012/)
