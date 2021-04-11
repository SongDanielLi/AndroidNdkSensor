# NDK Sensor
This is an example of reading Android sensor data via native c++ API, and a simple implementation from JNI.

## Pre-requisites
[NDK](https://developer.android.com/ndk/)

## Example
```C++
 // declare your own SensorEventListener
 class MyListener: public NdkSensorEventListener{
 public:
      void onSensorChanged(ASensorEvent *event) override{ // do something }
 }

 NdkSensorManager* manager = NdkSensorManager();
 manager->setListener(new MyListener());
 // register accelerometer with 50 Hz.
 manager->registerSensor({ASENSOR_TYPE_ACCELEROMETER}, 20000);

 // unregister sensors
 manager->unregister();
```

## Reference
[NDK Sensor](https://developer.android.com/ndk/reference/group/sensor)
[NDK sample](https://github.com/android/ndk-samples/tree/master/sensor-graph/)

## License
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
