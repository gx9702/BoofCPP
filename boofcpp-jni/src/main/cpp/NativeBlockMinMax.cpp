#include <jni.h>
#include <binary_ops.h>
#include <threshold_block_filters.h>
#include "JNIBoofCPP.h"

using namespace boofcv;

extern "C" {
JNIEXPORT void JNICALL Java_org_boofcpp_threshold_NativeBlockMinMax_nativeinit(
    JNIEnv *env, jobject obj, jobject jregionWidth,
    jdouble scale, jboolean down , jdouble minimumSpread, jboolean thresholdFromLocalBlocks ) {

    jclass objClass = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(objClass, "isInteger", "Z");
    jboolean isInteger = env->GetBooleanField(obj, fid);

    boofcv::ConfigLength region_width = extractConfigLength(env,jregionWidth);

    jlong ptr;
    if( isInteger ) {
        ptr = (jlong)new ThresholdBlockMinMax<U8>((float)minimumSpread,region_width,(bool)thresholdFromLocalBlocks,(float)scale,(bool)down);
    } else {
        ptr = (jlong)new ThresholdBlockMinMax<F32>((float)minimumSpread,region_width,(bool)thresholdFromLocalBlocks,(float)scale,(bool)down);
    }
    fid = env->GetFieldID(objClass, "nativePtr", "J");
    env->SetLongField(obj, fid, ptr);
}

JNIEXPORT void JNICALL Java_org_boofcpp_threshold_NativeBlockMinMax_nativedestroy(JNIEnv *env, jobject obj) {

    jclass objClass = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(objClass, "isInteger", "Z");
    jboolean isInteger = env->GetBooleanField(obj, fid);
    fid = env->GetFieldID(objClass, "nativePtr", "J");
    jlong nativePtr = env->GetLongField(obj, fid);

    if( isInteger ) {
        delete (ThresholdBlockMinMax<U8> *) nativePtr;
    } else {
        delete (ThresholdBlockMinMax<F32> *) nativePtr;
    }
}

JNIEXPORT void JNICALL Java_org_boofcpp_threshold_NativeBlockMinMax_nativeprocess
    (JNIEnv *env, jobject obj, jobject jinput, jobject joutput) {

    // Get the pointer to the thresholding algorithm
    jclass objClass = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(objClass, "isInteger", "Z");
    jboolean isInteger = env->GetBooleanField(obj, fid);
    fid = env->GetFieldID(objClass, "nativePtr", "J");
    if( env->ExceptionCheck() )
        return;
    jlong nativePtr = env->GetLongField(obj, fid);
    if( env->ExceptionCheck() )
            return;

    ImageAndInfo<Gray<U8>,JImageInfo> output = wrapCriticalGrayU8(env,joutput);

    try {
//        printf("   alg.threshold %d\n",alg->threshold);
        if( isInteger ) {
            ImageAndInfo<Gray<U8>,JImageInfo> input = wrapCriticalGrayU8(env,jinput);
            input.image.data = (U8*)env->GetPrimitiveArrayCritical((jarray)input.info.jdata, 0);
            output.image.data = (U8*)env->GetPrimitiveArrayCritical((jarray)output.info.jdata, 0);
            ((ThresholdBlockMinMax<U8>*)nativePtr)->process(input.image, output.image);
            env->ReleasePrimitiveArrayCritical((jarray)input.info.jdata, input.image.data, JNI_ABORT);
        } else {
            ImageAndInfo<Gray<F32>,JImageInfo> input = wrapCriticalGrayF32(env,jinput);
            input.image.data = (F32*)env->GetPrimitiveArrayCritical((jarray)input.info.jdata, 0);
            output.image.data = (U8*)env->GetPrimitiveArrayCritical((jarray)output.info.jdata, 0);
            ((ThresholdBlockMinMax<F32> *)nativePtr)->process(input.image, output.image);
            env->ReleasePrimitiveArrayCritical((jarray)input.info.jdata, input.image.data, JNI_ABORT);
        }
    } catch( ... ) {
        printf("Exception!!\n");
        // TODO do something here
    }

    // Release the arrays
    env->ReleasePrimitiveArrayCritical((jarray)output.info.jdata, output.image.data, 0);
}

}