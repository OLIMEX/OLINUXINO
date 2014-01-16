/*
 * UART.h
 *
 *  Created on: 16 Sep 2013
 *      Author: Stefan Mavrodiev @ OLIMEX LTD
 */

#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <android/log.h>
#include <fcntl.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "UART", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"UART",__VA_ARGS__)
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "UART", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "UART", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "UART", __VA_ARGS__)


extern "C"{

	JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_UART_serialOpen(JNIEnv *, jobject, jstring, jint);
	JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_UART_serialClose(JNIEnv *, jobject, jint);
	JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_UART_serialWrite(JNIEnv *, jobject, jint, jstring);
	JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_UART_serialRead(JNIEnv *, jobject, jint, jcharArray);
};


#endif /* UART_H_ */
