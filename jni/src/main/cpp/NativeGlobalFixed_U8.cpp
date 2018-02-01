#include <jni.h>
#include <binary_ops.h>
#include "JNIBoofCPP.h"

using namespace boofcv;

extern "C" {
JNIEXPORT void JNICALL Java_org_boofcpp_threshold_NativeGlobalFixed_1U8_nativeinit(
    JNIEnv *env, jobject obj, jdouble threshold, jboolean down) {

    jclass objClass = env->GetObjectClass(obj);

    GlobalFixedBinaryFilter<U8> *alg = new GlobalFixedBinaryFilter<U8>((U8)threshold,(bool)down);

    jfieldID fid = env->GetFieldID(objClass, "nativePtr", "J");
    env->SetLongField(obj, fid, (jlong)alg);
}

JNIEXPORT void JNICALL Java_org_boofcpp_threshold_NativeGlobalFixed_1U8_nativedestroy(JNIEnv *env, jobject obj) {

    jclass objClass = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(objClass, "nativePtr", "J");
    jlong nativePtr = env->GetLongField(obj, fid);

    delete (GlobalFixedBinaryFilter<U8> *)nativePtr;
}

JNIEXPORT void JNICALL Java_org_boofcpp_threshold_NativeGlobalFixed_1U8_nativeprocess
    (JNIEnv *env, jobject obj, jobject jinput, jobject joutput) {

    // Get the pointer to the thresholding algorithm
    jclass objClass = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(objClass, "nativePtr", "J");
    if( env->ExceptionCheck() )
        return;
    auto * alg = (GlobalFixedBinaryFilter<U8> *)env->GetLongField(obj, fid);
    if( env->ExceptionCheck() )
            return;

    JImageInfoU8 inputInfo = extractInfoU8(env,jinput);
    JImageInfoU8 outputInfo = extractInfoU8(env,joutput);

    Gray<U8> input((U8*)inputInfo.data,(uint32_t)inputInfo.dataLength,
                   (uint32_t)inputInfo.width,(uint32_t)inputInfo.height,
                   (uint32_t)inputInfo.offset,(uint32_t)inputInfo.stride);

    Gray<U8> output((U8*)outputInfo.data,(uint32_t)outputInfo.dataLength,
                   (uint32_t)outputInfo.width,(uint32_t)outputInfo.height,
                   (uint32_t)outputInfo.offset,(uint32_t)outputInfo.stride);

    try {
//        printf("   alg.threshold %d\n",alg->threshold);
        alg->process(input, output);
    } catch( ... ) {
        printf("Exception!!\n");
        // TODO do something here
    }

    // Release the arrays
    env->ReleaseByteArrayElements((jbyteArray)inputInfo.jdata, inputInfo.data, 0);
    env->ReleaseByteArrayElements((jbyteArray)outputInfo.jdata, outputInfo.data, 0);
}

}