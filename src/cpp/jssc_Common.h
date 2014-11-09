/* jSSC (Java Simple Serial Connector) - serial port communication library.
 * © Alexey Sokolov (scream3r), 2010-2014.
 *
 * This file is part of jSSC.
 *
 * jSSC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * jSSC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with jSSC.  If not, see <http://www.gnu.org/licenses/>.
 *
 * If you use jSSC in public project you can inform me about this by e-mail,
 * of course if you want it.
 *
 * e-mail: scream3r.org@gmail.com
 * web-site: http://scream3r.org | http://code.google.com/p/java-simple-serial-connector/
 * 
 * jssc_Common.h
 * This file holds some utility functions that are platform independent.
 * Contributors: Charles Hache <chalz@member.fsf.org>
 */

#include <jni.h>

#define SP_EXCEPTION_TYPE_READ_INTERRUPTED              1
#define SP_EXCEPTION_TYPE_NO_MEMORY                     2
#define SP_EXCEPTION_TYPE_PARAMETER_IS_NOT_CORRECT      3
#define SP_EXCEPTION_TYPE_PORT_NOT_OPENED               4
#define SP_EXCEPTION_TYPE_UNKNOWN                       5

/*
 * Calls System.out.println(String msg) with the given message.
 */
void println(JNIEnv *env, const char* msg) {
    //Adapted from 
    // http://stackoverflow.com/questions/25417792/how-to-call-system-out-println-from-c-via-jni
    jclass syscls = env->FindClass("java/lang/System");
    jfieldID fid = env->GetStaticFieldID(syscls, "out", "Ljava/io/PrintStream;");
    jobject out = env->GetStaticObjectField(syscls, fid);
    jclass pscls = env->FindClass("java/io/PrintStream");
    jmethodID mid = env->GetMethodID(pscls, "println", "(Ljava/lang/String;)V");

    jstring str = env->NewStringUTF(msg);
    env->CallVoidMethod(out, mid, str);
}

/*
 * Get the number of micro seconds since some epoch.  Uses System.nanoTime().  
 */
long getTimePreciseMicros(JNIEnv *env) {
    jclass syscls = env->FindClass("java/lang/System");
    jmethodID mid = env->GetStaticMethodID(syscls, "nanoTime", "()J");

    jlong ret = env->CallStaticLongMethod(syscls, mid)/1000;
    return ret;
}

/*
 * Updates the supplied timeval struct with the next timeout to use for a select call,
 * based on the timeoutDeadline and the pollPeriodMillis parameters.
 * If timeoutDeadline is 0 it is considered to not block at all. If timeoutDeadline
 * is <0 then it is considered to block forever.
 * Returns 1 if we should block forever (and thus the time* is undefined), returns
 * 0 if the time* has been filled with the next unblock timeout for select().
 */
int getNextTimeout(JNIEnv *env, struct timeval *time, long timeoutDeadline, long pollPeriodMillis) {

    if (time == NULL)
        return 1;

    if (timeoutDeadline == 0) {
        time->tv_sec=0;
        time->tv_usec=0;
        return 0;
    }

    if (timeoutDeadline > 0) {
        long currentTime = getTimePreciseMicros(env);
        long timeUntilTimeout = timeoutDeadline - currentTime;

        if (timeUntilTimeout <= 0) {
            time->tv_sec=0;
            time->tv_usec=0;
        } else {
            if (pollPeriodMillis > 0) {
                long pollPeriodMicros = pollPeriodMillis * 1000;
                if (pollPeriodMicros < timeUntilTimeout) {
                    time->tv_sec = pollPeriodMillis / 1000;
                    time->tv_usec = (pollPeriodMillis % 1000)*1000;
                } else {
                    time->tv_sec = timeUntilTimeout / 1000000;
                    time->tv_usec = timeUntilTimeout % 1000000;
                }
            } else {
                time->tv_sec = timeUntilTimeout / 1000000;
                time->tv_usec = timeUntilTimeout % 1000000;
            }
        }
        return 0;
    }

    if (timeoutDeadline < 0) {
        if (pollPeriodMillis > 0) {
            time->tv_sec = pollPeriodMillis / 1000;
            time->tv_usec = (pollPeriodMillis % 1000)*1000;
            return 0;
        } else {
            return 1;
        }
    }
    return 1;    //Blocks forever
}

/*
 * Throws a java SerialPortTimeoutException with the provided parameters.
 */
void throwTimeoutException(JNIEnv *env, const char* portName, const char* methodName, long timeoutMillis) {
    jclass excClass = env->FindClass("jssc/SerialPortTimeoutException");
    jmethodID ctor = env->GetMethodID(excClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;I)V");
    jstring port = env->NewStringUTF(portName);
    jstring method = env->NewStringUTF(methodName);
    jobject exception = env->NewObject(excClass, ctor, port, method, (jlong)timeoutMillis);
    env->Throw((jthrowable)exception);
}

static jstring getExceptionType(JNIEnv *env, int type) {
    jclass excClass = env->FindClass("jssc/SerialPortException");
    jfieldID field = NULL;
    jstring ret;
    switch (type) {
    case SP_EXCEPTION_TYPE_READ_INTERRUPTED:
        field = env->GetStaticFieldID(excClass, "TYPE_READ_INTERRUPTED", "Ljava/lang/String;");
        break;
    case SP_EXCEPTION_TYPE_NO_MEMORY:
        field = env->GetStaticFieldID(excClass, "TYPE_NO_MEMORY", "Ljava/lang/String;");
        break;
    case SP_EXCEPTION_TYPE_PARAMETER_IS_NOT_CORRECT:
        field = env->GetStaticFieldID(excClass, "TYPE_PARAMETER_IS_NOT_CORRECT", "Ljava/lang/String;");
        break;
    case SP_EXCEPTION_TYPE_PORT_NOT_OPENED:
        field = env->GetStaticFieldID(excClass, "TYPE_PORT_NOT_OPENED", "Ljava/lang/String;");
        break;
    case SP_EXCEPTION_TYPE_UNKNOWN:
        field = env->GetStaticFieldID(excClass, "TYPE_UNKNOWN", "Ljava/lang/String;");
        break;
    }

    if (field != NULL) {
        ret = (jstring)env->GetStaticObjectField(excClass, field);
    } else {
        ret = env->NewStringUTF("Invalid Exception Type");
    }
    return ret;
}

/*
 * Throws a java SerialPortException with the provided parameters.
 */
void throwSerialException(JNIEnv *env, const char* portName, const char* methodName, int exceptionType) {
    jclass excClass = env->FindClass("jssc/SerialPortException");
    jmethodID ctor = env->GetMethodID(excClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String)V");
    jstring port = env->NewStringUTF(portName);
    jstring method = env->NewStringUTF(methodName);
    jstring type = getExceptionType(env, exceptionType);
    jobject exception = env->NewObject(excClass, ctor, port, method, type);
    env->Throw((jthrowable)exception);
}

