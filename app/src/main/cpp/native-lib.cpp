#include <jni.h>
#include <string>
#include <android/log.h>
#include "NdkSensor.h"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, "JniSensor", __VA_ARGS__)

NdkSensorManager *sensorManager;
JNIEnv *jEnv;
jobject jListener;
jclass jListenerClz;
jclass jEventClz;

class MyListener: public NdkSensorEventListener{
public:
    void onSensorChanged(ASensorEvent *event) override{
        // initial NdkSensorEvent
        if(jEventClz == nullptr){
            LOGI("jEvent class null");
            return;
        }
        jmethodID jEventInit = jEnv->GetMethodID(jEventClz, "<init>", "(I)V");
        jobject jEvent = jEnv->NewObject(jEventClz, jEventInit, event->type);
        if(jEvent == nullptr){
            LOGI("jEvent null");
            return;
        }

        // set timestamp and values
        jmethodID setter = jEnv->GetMethodID(jEventClz, "setTimestamp", "(J)V");
        jEnv->CallVoidMethod(jEvent, setter, event->timestamp);
        setter = jEnv->GetMethodID(jEventClz, "setValue", "([F)V");
        jfloatArray jValues = jEnv->NewFloatArray(16);
        jEnv->SetFloatArrayRegion(jValues, 0, 16, event->data);
        jEnv->CallVoidMethod(jEvent, setter, jValues);

        if(jListener == nullptr){
            LOGI("jListener null");
            return;
        }
        // update
        jmethodID jListenerMethod = jEnv->GetMethodID(jListenerClz, "onSensorChanged", "(Lcom/song/ndksensor/NdkSensorEvent;)V");
        jEnv->CallVoidMethod(jListener, jListenerMethod, jEvent);
    }
};

extern "C"
JNIEXPORT void JNICALL
Java_com_song_ndksensor_NdkSensor_ndkSensorInit(JNIEnv *env, jobject thiz) {
    sensorManager = new NdkSensorManager();
    sensorManager->setListener(new MyListener());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_song_ndksensor_NdkSensor_ndkSensorSetListener(JNIEnv *env, jobject thiz,
                                                       jobject listener) {
    jEnv = env;
    jListener = env->NewGlobalRef(listener);
    jListenerClz = (jclass)env->NewGlobalRef(env->GetObjectClass(listener));
    jEventClz = (jclass)env->NewGlobalRef(env->FindClass("com/song/ndksensor/NdkSensorEvent"));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_song_ndksensor_NdkSensor_ndkSensorRegister(JNIEnv *env, jobject thiz, jintArray types,
                                                    jlong delay_in_us) {
    int *cTypes = env->GetIntArrayElements(types, nullptr);
    jsize len = env->GetArrayLength(types);
    std::vector<int> registerTypes(cTypes, cTypes+len);
    sensorManager->registerSensor(registerTypes, (int32_t)delay_in_us);
    env->ReleaseIntArrayElements(types, cTypes, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_song_ndksensor_NdkSensor_unregister(JNIEnv *env, jobject thiz) {
    if(sensorManager) sensorManager->unregister();
}


