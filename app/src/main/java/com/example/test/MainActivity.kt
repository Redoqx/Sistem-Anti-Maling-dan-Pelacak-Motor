package com.example.test

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.*
import androidx.core.view.get
import kotlin.random.Random

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val roolButton = findViewById<Button>(R.id.rollButton)
        val resultTextView = findViewById<TextView>(R.id.resultTextView)
        val Angka = findViewById<EditText>(R.id.Angka)

        roolButton.setOnClickListener {
            val rand = java.util.Random().nextInt(Angka.text.toString().toInt())+1
            resultTextView.text = rand.toString()
        }
    }
}
