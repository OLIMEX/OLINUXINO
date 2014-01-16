#include <jni.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "I2C.h"

JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_I2C_init(JNIEnv *env, jobject obj,
		jstring file) {

	int fd;
	char filename[64];
	const char *name;
	jboolean iscopy;

	name = env->GetStringUTFChars(file, &iscopy);
	if (name == NULL) {
		return -1;
	}

	sprintf(filename, "%s", name);
	env->ReleaseStringUTFChars(file, name);

	fd = open(filename, O_RDWR);

	if(fd < 0)
		return -1;

	return fd;
}
JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_I2C_close(JNIEnv *env, jobject obj,
		jint fd) {
	if (close(fd) < 0)
		return errno;
	return 0;
}
JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_I2C_open(JNIEnv *env, jobject obj,
		jint fd, jint address) {

	if (ioctl(fd, I2C_SLAVE_FORCE, address) < 0)
		return -1;

	return 0;
}
JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_I2C_write(JNIEnv *env, jobject obj,
		jint fd, jintArray buffer, jint len) {

	jboolean isCopy;
	jbyte bufByte[len];
	jint *arr = env->GetIntArrayElements(buffer, &isCopy);
	env->ReleaseIntArrayElements(buffer, arr, 0);
	for (int i = 0; i < len; i++)
		bufByte[i] = arr[i] & 0xFF;

	if (write(fd, bufByte, len) < 0)
		return -1;

	return 0;
}
JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_I2C_read(JNIEnv *env, jobject obj,
		jint fd, jintArray buffer, jint len) {

	unsigned char BufByte[len];
	jint BufInt[len];

	if (read(fd, BufByte, len) < 0)
		return -1;

	for (int i = 0; i < len; i++) {
		BufInt[i] = (int) BufByte[i];
	}
	env->SetIntArrayRegion(buffer, 0, len, BufInt);
	return 0;
}
jint JNI_OnLoad(JavaVM* vm, void* reserverd) {
	JNIEnv* env;
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK)
		return -1;

	return JNI_VERSION_1_6;

}
