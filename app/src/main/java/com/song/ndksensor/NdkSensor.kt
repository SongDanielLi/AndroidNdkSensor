package com.song.ndksensor

class NdkSensor {

    init {
        ndkSensorInit()
    }

    fun registerSensor(types: IntArray, delay_in_us: Long, listener: NdkSensorListener){
        ndkSensorSetListener(listener)
        ndkSensorRegister(types, delay_in_us)
    }

    interface NdkSensorListener{
        fun onSensorChanged(event: NdkSensorEvent)
    }

    private external fun ndkSensorInit()
    private external fun ndkSensorSetListener(listener: NdkSensorListener)
    private external fun ndkSensorRegister(types: IntArray, delay_in_us: Long)
    external fun unregister()

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}

class NdkSensorEvent(val type: Int){
    var timestamp: Long = 0L
    var value: FloatArray? = null
}