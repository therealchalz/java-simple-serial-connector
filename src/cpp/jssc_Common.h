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

#include <sys/time.h> 

#define SP_EXCEPTION_TYPE_READ_INTERRUPTED              1
#define SP_EXCEPTION_TYPE_NO_MEMORY                     2
#define SP_EXCEPTION_TYPE_PARAMETER_IS_NOT_CORRECT      3
#define SP_EXCEPTION_TYPE_PORT_NOT_OPENED               4
#define SP_EXCEPTION_TYPE_UNKNOWN                       5

/*
 * Calls System.out.println(String msg) with the given message.
 */
void println(JNIEnv *env, const char* msg) ;

/*
 * Get the number of micro seconds since some epoch.  Uses System.nanoTime(). 
 * May be negative. 
 */
long getTimePreciseMicros(JNIEnv *env) ;

/*
 * Updates the supplied timeval struct with the next timeout to use for a select call.
 * timeoutDeadline is a point on the time scale provided by System.nanoTime(), but in micro-
 * seconds.  It may be negative, 0, or positive.
 * pollPeriodMillis is how often the thread should check its interrupted status, or 0 if
 * the thread should never check.
 * deadlineValid is an indicator to show if timeoutDeadline is valid or not, since any possible
 * value for timeoutDeadline would be acceptable.  Set deadlineValid to 0 to indicate timeoutDeadline
 * should be ignored; set to non-zero otherwise.
 * Note that setting the deadlineValid parameter to 0 and the pollPeriodMillis to 0 is an invalid
 * combination, since there is no way to set up the time struct to block forever; this has to be 
 * done through the select call itself.  In this case the function returns 1 to indicate error.
 * Returns 1 on error (and thus the time* is undefined), returns 0 if the time struct has been 
 * filled with the next unblock timeout for select().
 */
int getNextTimeout(JNIEnv *env, struct timeval *time, char deadlineValid, long timeoutDeadline, long pollPeriodMillis) ;

/*
 * Throws a java SerialPortTimeoutException with the provided parameters.
 */
void throwTimeoutException(JNIEnv *env, const char* portName, const char* methodName, jlong timeoutMillis) ;

/*
 * Throws a java SerialPortException with the provided parameters.
 */
void throwSerialException(JNIEnv *env, const char* portName, const char* methodName, int exceptionType) ;

