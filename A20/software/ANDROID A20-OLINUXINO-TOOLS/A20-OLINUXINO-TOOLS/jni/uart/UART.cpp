#include <jni.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "UART.h"

JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_UART_serialOpen(JNIEnv *env, jobject obj, jstring port, jint baud){

	struct termios serialConfiguration;
	const char *tty = env->GetStringUTFChars(port, NULL);
	speed_t speed;
	int fd;

	memset(&serialConfiguration, 0, sizeof(serialConfiguration));


	fd = open(tty, O_RDWR);
	env->ReleaseStringUTFChars(port, tty);
	if(fd < 0)
		return -1;

	switch(baud){
		case 4800:
			speed = B4800;
			break;
		case 9600:
			speed = B9600;
			break;
		case 19200:
			speed = B19200;
			break;
		case 38400:
			speed = B38400;
			break;
		case 57600:
			speed = B57600;
			break;
		case 115200:
			speed = B115200;
			break;
		default:
			speed = B9600;
			LOGW("Warning... Invalid speed. Set to default 9600!");
			break;
	}

	// Getting current configuration
	if(tcgetattr(fd, &serialConfiguration) != 0)
		return -1;

	// Setting new configuration
	cfsetispeed(&serialConfiguration, speed);
	cfsetospeed(&serialConfiguration, speed);
	cfmakeraw(&serialConfiguration);

	// Make as non blocking
	serialConfiguration.c_cc[VTIME] = 0.001;	//Make timeout 1ms
	serialConfiguration.c_cc[VMIN] = 1;			// Make blocking read

	// Let the configuration to take effect
	if(tcsetattr(fd, TCSANOW, &serialConfiguration) != 0)
		return -1;

	return fd;
}

JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_UART_serialClose(JNIEnv *env, jobject obj, jint fd){
	if(close(fd) < 0){
		LOGE(strerror(errno));
		return -1;
	}
	return 0;
}

JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_UART_serialWrite(JNIEnv *env, jobject obj, jint fd, jstring text){
	const char *buffer = env->GetStringUTFChars(text, NULL);
	jsize len = env->GetStringUTFLength(text);

	int result;
	result = write(fd, buffer, len);
	env->ReleaseStringUTFChars(text, buffer);

	if(result < 0){
		LOGE(strerror(errno));
		return -1;
	}

	return 0;
}

JNIEXPORT jint JNICALL Java_olimex_a20_tools_jni_UART_serialRead(JNIEnv *env, jobject obj, jint fd, jcharArray buff){

	char buffer[256];
	int len;

	len = read(fd, buffer, 256);
	jchar *jbuffer = (jchar *)calloc(sizeof(jchar), len);

	for(int i = 0; i < len; i++)
		jbuffer[i] = (jchar) buffer[i];

	env->SetCharArrayRegion(buff, 0, len, jbuffer);
	free(jbuffer);
	return len;
}
jint JNI_OnLoad(JavaVM* vm, void* reserverd){
	JNIEnv* env;
	if(vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK)
		return -1;


	return JNI_VERSION_1_6;

}
