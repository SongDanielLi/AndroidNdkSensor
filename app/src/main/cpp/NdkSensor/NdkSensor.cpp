#include "NdkSensor.h"
#include <android/log.h>

const int kLooperId = 3;
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, "NdkSensor", __VA_ARGS__)

/** data can be passed from ASensorManager_createEventQueue() */
static int onSensorChanged(int fd, int events, void* data){
    auto *mSensorManager = (NdkSensorManager *)data;
    ASensorEvent event;
    while (ASensorEventQueue_getEvents(mSensorManager->_sensorEventQueue, &event, 1) > 0){
        mSensorManager->_listener->onSensorChanged(&event);
    }
    return 1;
}

NdkSensorManager::NdkSensorManager(){
    _sensorManager = ASensorManager_getInstance();
    _ndkSensorLooper = ALooper_forThread();
}

void NdkSensorManager::registerSensor(const vector<int> &sensorIDs, int32_t usec) {
    _sensorEventQueue = ASensorManager_createEventQueue(_sensorManager, _ndkSensorLooper, kLooperId,
                                                        onSensorChanged, this);
    for(auto type: sensorIDs){
        auto *sensor = const_cast<ASensor *>(ASensorManager_getDefaultSensor(_sensorManager, type));
        if(sensor != nullptr){
            auto status = ASensorEventQueue_enableSensor(_sensorEventQueue, sensor);
            assert(status >= 0);
            status = ASensorEventQueue_setEventRate(_sensorEventQueue, sensor, usec);
            assert(status >= 0);
            enable_sensor.emplace_back(type, sensor);
            LOGI("Register: %d", type);
        } else {
            LOGI("Sensor id : %d == null", type);
        }
    }

}

void NdkSensorManager::unregister() {
    for(auto sensor : enable_sensor){
        if(sensor.second != nullptr && _sensorEventQueue != nullptr){
            ASensorEventQueue_disableSensor(_sensorEventQueue, sensor.second);
            sensor.second = NULL;
        }
    }
    enable_sensor.clear();
}

