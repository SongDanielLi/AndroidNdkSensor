package com.song.ndksensor

import android.annotation.SuppressLint
import android.hardware.Sensor
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.TextView

class MainActivity : AppCompatActivity() {

    private lateinit var ndkSensor: NdkSensor

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val text = findViewById<TextView>(R.id.sample_text)

        ndkSensor = NdkSensor()
        ndkSensor.registerSensor(
            intArrayOf(Sensor.TYPE_ACCELEROMETER),
            1000000,
            object: NdkSensor.NdkSensorListener{
                @SuppressLint("SetTextI18n")
                override fun onSensorChanged(event: NdkSensorEvent) {
                    text.text = "type: ${event.type}, ts: ${event.timestamp}\n, value: ${event.value?.joinToString("\n")}"
                }
            }
        )
    }

    override fun onDestroy() {
        super.onDestroy()
        ndkSensor.unregister()
    }
}