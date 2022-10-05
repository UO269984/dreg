package com.dreg.test;

import java.util.function.Consumer;

import androidx.appcompat.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ScrollView;
import android.widget.TextView;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {
	
	public static native void setTestPrintFunc(Consumer<String> printFunc);
	public static native boolean runTest();
	
	private ScrollView consoleScroll;
	private TextView console;
	
	static {
		System.loadLibrary("testApi");
	}
	
	private void consoleLog(String msg) {
		this.console.append(msg);
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		this.consoleScroll = (ScrollView) findViewById(R.id.consoleScroll);
		this.console = (TextView) findViewById(R.id.consoleTx);
		this.console.setText("");
		
		String[] librayPath = getApplicationInfo().nativeLibraryDir.split("/");
		consoleLog("Libray type: " + librayPath[librayPath.length - 1] + "\n");
		setTestPrintFunc(this::consoleLog);
		
		((Button) findViewById(R.id.runTestBt)).setOnClickListener((View view) -> {
			runTest();
			consoleScroll.post(() -> consoleScroll.fullScroll(View.FOCUS_DOWN));
		});
	}
}