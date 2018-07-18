/*
 * I2C.h
 *
 *  Created on: June 5, 2013
 *      Author: root
 */

#ifndef I2C_H_
#define I2C_H_


#include <android/log.h>
#include <stdio.h>
#include <android/log.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <memory.h>
#include <malloc.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "I2C", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"I2C",__VA_ARGS__)
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "I2C", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "I2C", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "I2C", __VA_ARGS__)

extern "C"{
	JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_I2C_init(JNIEnv *, jobject, jstring);
	JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_I2C_close(JNIEnv *, jobject, jint);
	JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_I2C_open(JNIEnv *, jobject, jint, jint);
	JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_I2C_write(JNIEnv *, jobject, jint, jintArray, jint);
	JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_I2C_read(JNIEnv *, jobject, jint, jintArray, jint);


};

#endif /* I2C_H_ */
