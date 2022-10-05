#include "testPerformance.h"
#include <jni.h>

extern "C" {
	JavaVM* jvm;
	jint jniVersion;
	
	jmethodID consumerAccept;
	jobject printFuncJava = NULL;
	
	JNIEXPORT void JNICALL
	Java_com_dreg_test_MainActivity_setTestPrintFunc(JNIEnv* env, jclass c, jobject printFuncConsumer) {
		env->GetJavaVM(&jvm);
		jniVersion = env->GetVersion();
		
		if (printFuncJava != NULL)
			env->DeleteGlobalRef(printFuncJava);
		
		printFuncJava = env->NewGlobalRef(printFuncConsumer);
		consumerAccept = env->GetMethodID(env->FindClass("java/util/function/Consumer"),
			"accept", "(Ljava/lang/Object;)V");
		
		setTestPrintFunc([](const char* msg) {
			JNIEnv* env;
			jvm->GetEnv((void**) &env, jniVersion);
			env->CallVoidMethod(printFuncJava, consumerAccept, env->NewStringUTF(msg));
		});
	}
	
	JNIEXPORT jboolean JNICALL
	Java_com_dreg_test_MainActivity_runTest(JNIEnv* env, jclass c) {
		TestsConfig config;
		loadDefaultConfig(&config);
		
		bool ret = allTests(&config);
		deleteTestsConfig(&config);
		return ret;
	}
}