#kalman_filter with graph made in chan

from mpl_toolkits import mplot3d

import numpy as np
import matplotlib.pyplot as plt

#def measurement_update(estimated_mean, estimated_var, measurement_mean, measurement_var):
#    new_mean = (measurement_var * estimated_mean + estimated_var * measurement_mean) / (estimated_var + measurement_var)
#    new_var = estimated_var * measurement_var/(estimated_var + measurement_var)
#    return new_mean, new_var

#def state_prediction(estimated_mean, estimated_var, motion_control_mean, motion_control_var):
#    new_mean = estimated_mean + motion_control_mean
#    new_var  = estimated_var + motion_control_var
#    return new_mean, new_var

##1차원
#if __name__ == "__main__":

#    measurement_var = 2
#    motion_control_var = 1

#    mu = 0;
#    sig = 1000;

#    stand = np.linspace(0,100,100)
#    kalman = np.linspace(0,0,100)
#    conrol = np.linspace(1,1,100)

#    grountTruth = np.linspace(0,100,100)#y = t그래프
#    noise_m = np.random.normal(0,measurement_var,grountTruth.shape)#*(정규분포 평균,표준편차,갯수)
#    grountNoise = grountTruth + noise_m#원본에 노이즈 추가
#    noise_c = np.random.normal(0,motion_control_var,conrol.shape)#*(정규분포 평균,표준편차,갯수)
#    conrolNoise = conrol + noise_c#원본에 노이즈 추가


   
#    for i in range(100):
#        mu, sig = state_prediction(mu, sig, conrolNoise[i], motion_control_var)##
#        mu, sig = measurement_update(mu, sig, grountNoise[i], measurement_var)##
#        kalman[i] = mu
    
#    plt.plot(stand, color='b')
#    plt.plot(grountNoise, color='r')
#    plt.plot( kalman, label='measurement', color='g')

#    plt.xlabel('time')
#    plt.ylabel('distance')
   
#    plt.title('Q1_Xcoordinate')

#    plt.show()









### Multi dimensional Kalman filter

#measurement_var = 3.
#motion_control_var = 1.

#conrol = np.linspace(1,1,100)
#kalman_X = np.linspace(0,0,100)
#kalman_Y = np.linspace(0,0,100)
#time = np.linspace(0,100,100)


#grountTruth = np.linspace(0,100,100)#y = t그래프

#noise_X = np.random.normal(0,measurement_var,grountTruth.shape)#*(정규분포 평균,표준편차,갯수)
#noise_Y = np.random.normal(0,measurement_var,grountTruth.shape)#*(정규분포 평균,표준편차,갯수)
#noise_cx = np.random.normal(0,motion_control_var,conrol.shape)
#noise_cy = np.random.normal(0,motion_control_var,conrol.shape)

#grountNoise_X = grountTruth + noise_X#원본에 노이즈 추가
#grountNoise_Y = grountTruth*2 + noise_Y

#conrolNoise_X = conrol + noise_cx#원본에 노이즈 추가
#conrolNoise_Y = conrol + noise_cy#원본에 노이즈 추가




#mu = np.matrix([[0.], [0.]])  # initial state (location and velocity)
#sig = np.matrix([[1000., 0.],
#                 [0., 1000.]])  # initial uncertainty
#u = np.matrix([[0.], [0.]])  # external motion(전이행렬)

#C = np.matrix([[1., 1.]])  # measurement function
#Q = np.matrix([[measurement_var]])  # measurement uncertainty
#I = np.eye(2)
#B = np.eye(2)*motion_control_var

#A = np.matrix([[1, 0],
#               [0, 1]])  # next state function




#for n in range(100): # enumerate(100):
#    u = (conrolNoise_X[n],conrolNoise_Y[n])

#    # prediction
#    mu_bar = A * mu + B * u
#    sig_bar = A * sig * A.transpose() + Q

#    # measurement update
#    s = C * sig_bar * C.transpose() + Q
#    K = sig_bar * C.transpose() * np.linalg.inv(s)

#    z = np.matrix([grountNoise_X[n], grountNoise_Y[n]])
#    mu = mu_bar + K * (z - C * mu_bar)
#    sig = (I - K * C) * sig_bar
    
#    kalman_X[n] = mu[0,0]
#    kalman_Y[n] = mu[1,1]

#    mu[0,1] = 0
#    mu[1,0] = 0


#    print(mu, '\n')

#fig = plt.figure()
#ax = plt.axes(projection="3d")
#ax.plot( time, grountTruth, grountTruth*2, color='b', );
#ax.plot(time, grountNoise_X,grountNoise_Y, color='r');
#ax.plot(time, kalman_X,  kalman_Y, color='g');

#ax.set_ylabel('X distance(m)')
#ax.set_xlabel('time(s)')
#ax.set_zlabel('Y distance(m)')
#plt.title('Q2_XYcoordinate')
#plt.show()





##acceleration

def acceleration(acc, time, shift=0):
    stand = np.linspace(0,0,time)
    for i in range(100):
        stand[i] = acc*2*(i+shift)**2
    return stand
 
def move_distance(acc, time):
    dis = acceleration(acc, time, 1) - acceleration(acc, time) 
    dis[0] = 0
    return dis

measurement_var = 3000.
motion_control_var = 100.
acc_X = 1
acc_Y = 2


conrol_X = move_distance(acc_X, 100)
conrol_Y = move_distance(acc_Y, 100)
grountTruth_X = acceleration(acc_X, 100)
grountTruth_Y = acceleration(acc_X, 100)
kalman_X = np.linspace(0,0,100)
kalman_Y = np.linspace(0,0,100)
time = np.linspace(0,100,100)


noise_cx = np.random.normal(0,motion_control_var,conrol_X.shape)
noise_cy = np.random.normal(0,motion_control_var,conrol_Y.shape)
noise_X = np.random.normal(0,measurement_var,grountTruth_X.shape)#*(정규분포 평균,표준편차,갯수)
noise_Y = np.random.normal(0,measurement_var,grountTruth_Y.shape)#*(정규분포 평균,표준편차,갯수)

grountNoise_X = grountTruth_X + noise_X#원본에 노이즈 추가
grountNoise_Y = grountTruth_Y + noise_Y
conrolNoise_X = conrol_X + noise_cx #원본에 노이즈 추가
conrolNoise_Y = conrol_Y + noise_cy#원본에 노이즈 추가




mu = np.matrix([[0.], [0.]])  # initial state (location and velocity)
sig = np.matrix([[1000., 0.],
                 [0., 1000.]])  # initial uncertainty
u = np.matrix([[0.], [0.]])  # external motion(전이행렬)

C = np.matrix([[1., 1.]])  # measurement function
Q = np.matrix([[measurement_var]])  # measurement uncertainty
I = np.eye(2)
B = np.eye(2)

A = np.matrix([[1, 0],
               [0, 1]])  # next state function
R = np.matrix([[motion_control_var]])



for n in range(100): # enumerate(100):
    u = (conrolNoise_X[n],conrolNoise_Y[n])

    # prediction
    mu_bar = A * mu + B * u
    sig_bar = A * sig * A.transpose() + R

    # measurement update
    s = C * sig_bar * C.transpose() + Q
    K = sig_bar * C.transpose() * np.linalg.inv(s)

    z = np.matrix([grountNoise_X[n], grountNoise_Y[n]])
    mu = mu_bar + K * (z - C * mu_bar)
    sig = (I - K * C) * sig_bar
    
    kalman_X[n] = mu[0,0]
    kalman_Y[n] = mu[1,1]
   
    #Q = sig[0,0] + sig[1,1]
    #Q = sig[1,0] + sig [0,1]
    sig[1,0] = sig [0,1] = 0
    mu[1,0]= mu[0,1] = 0


    print(mu, '\n')


fig = plt.figure()
acc = plt.axes(projection="3d")
acc.plot(time, grountTruth_X, grountTruth_Y, color='b', label='grountTruth');
acc.plot(time, grountNoise_X, grountNoise_Y, color='r', label='measurement');
acc.plot(time, kalman_X,  kalman_Y, color='g', label='kalmanFiterResult');

plt.title('Q3_acceleration')
acc.set_ylabel('X distance(m)')
acc.set_xlabel('time(s)')
acc.set_zlabel('Y distance(m)')
plt.show()