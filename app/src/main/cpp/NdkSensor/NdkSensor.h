#ifndef NDKSENSOR_H
#define NDKSENSOR_H

#include <android/sensor.h>
#include <vector>
using namespace std;

/**
 * @sample
 * // declare your own SensorEventListener
 * class MyListener: public NdkSensorEventListener{
 * public:
 *      void onSensorChanged(ASensorEvent *event) override{ // do something }
 * }
 *
 * NdkSensorManager* manager = NdkSensorManager();
 * manager->setListener(new MyListener());
 * // register accelerometer with 50 Hz.
 * manager->registerSensor({ASENSOR_TYPE_ACCELEROMETER}, 20000);
 *
 * // unregister sensors
 * manager->unregister();
 */

class NdkSensorEventListener{
public:
    virtual void onSensorChanged(ASensorEvent *event){};
};

class NdkSensorManager{
public:
    NdkSensorManager();
    inline void setListener(NdkSensorEventListener *listener){_listener = listener;}
    void registerSensor(const vector<int>& sensorIDs, int32_t usec);
    void unregister();

    ASensorEventQueue *_sensorEventQueue{};
    NdkSensorEventListener* _listener = nullptr;
private:
    ASensorManager *_sensorManager;
    ALooper *_ndkSensorLooper;
    vector<pair<int, ASensor*>> enable_sensor;
};

#endif
