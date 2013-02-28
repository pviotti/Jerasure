#include "eu_vandertil_jerasure_jni_Decoding.h"
#include "jerasure.h"

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

/*
* Class:     eu_vandertil_jerasure_jni_Decoding
* Method:    decodeWithMatrix
* Signature: (III[IZ[I[[B[[BI)Z
*/
JNIEXPORT jboolean JNICALL Java_eu_vandertil_jerasure_jni_Decoding_decodeWithMatrix(JNIEnv *env, jclass clazz, jint numDataDevices, jint numCodingDevices, jint wordSize, 
																					jintArray matrix, jboolean firstRowInMatrixIsAllOnes, jintArray erasedDevices, 
																					jobjectArray dataDevices, jobjectArray codingDevices, jint size)
{
	int i;
	int result;
	jint* matrixElements = env->GetIntArrayElements(matrix, NULL);
	jint* erasures = env->GetIntArrayElements(erasedDevices, NULL);

	jobject* dataDevicesElements = talloc(jobject, numDataDevices);
	jbyte** data_ptrs = talloc(jbyte*, numDataDevices);

	for(i = 0; i < numDataDevices; ++i) {
		dataDevicesElements[i] = env->GetObjectArrayElement(dataDevices, i);
		data_ptrs[i] = env->GetByteArrayElements((jbyteArray)dataDevicesElements[i], NULL);
	}

	jobject* codingDevicesElements = talloc(jobject, numCodingDevices);
	jbyte** coding_ptrs = talloc(jbyte*, numCodingDevices);

	for(i = 0; i < numCodingDevices; ++i)
	{
		codingDevicesElements[i] = env->GetObjectArrayElement(codingDevices, i);
		coding_ptrs[i] = env->GetByteArrayElements((jbyteArray)codingDevicesElements[i], NULL);
	}

	result = jerasure_matrix_decode(numDataDevices, numCodingDevices, wordSize, (int*)matrixElements, firstRowInMatrixIsAllOnes, (int*)erasures, (char**)data_ptrs, (char**)coding_ptrs, size);

    // Start of cleanup
	env->ReleaseIntArrayElements(matrix, matrixElements, 0);
	env->ReleaseIntArrayElements(erasedDevices, erasures, 0);

	for(i = 0; i < numDataDevices; ++i)
	{
		env->ReleaseByteArrayElements((jbyteArray)dataDevicesElements[i], data_ptrs[i], NULL);
	}

	for(i = 0; i < numCodingDevices; ++i)
	{
		env->ReleaseByteArrayElements((jbyteArray)codingDevicesElements[i], coding_ptrs[i], NULL);
	}

	free(codingDevicesElements);
	free(dataDevicesElements);
	free(data_ptrs);
	free(coding_ptrs);
	// Cleanup done.

	if(result == 0)
		return JNI_TRUE;
	else 
		return JNI_FALSE;
}


