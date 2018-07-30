/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDlangUtil.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef vtkDlangUtil_h
#define vtkDlangUtil_h

#include "vtkSystemIncludes.h"
#include <jni.h>
#include "vtkCommand.h"
#include "vtkWrappingDlangModule.h"

#include <string>

extern VTKWRAPPINGDLANG_EXPORT jlong q(JNIEnv *env,jobject obj);

extern VTKWRAPPINGDLANG_EXPORT void *vtkDlangGetPointerFromObject(JNIEnv *env,jobject obj);
extern VTKWRAPPINGDLANG_EXPORT char *vtkDlangUTFToChar(JNIEnv *env, jstring in);
extern VTKWRAPPINGDLANG_EXPORT bool vtkDlangUTFToString(JNIEnv *env, jstring in, std::string &out);
extern VTKWRAPPINGDLANG_EXPORT jstring vtkDlangMakeDlangString(JNIEnv *env, const char *in);

extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfFloatFromFloat(JNIEnv *env,
             const float *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfDoubleFromFloat(JNIEnv *env,
             const float *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfDoubleFromDouble(JNIEnv *env,
             const double *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfShortFromShort(JNIEnv *env, const short *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfIntFromInt(JNIEnv *env, const int *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfIntFromIdType(JNIEnv *env, const vtkIdType *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfIntFromLongLong(JNIEnv *env, const long long *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfIntFromSignedChar(JNIEnv *env, const signed char *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfLongFromLong(JNIEnv *env, const long *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfByteFromUnsignedChar(JNIEnv *env, const unsigned char *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfByteFromChar(JNIEnv *env, const char *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfCharFromChar(JNIEnv *env, const char *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfUnsignedCharFromUnsignedChar(JNIEnv *env, const unsigned char *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfUnsignedIntFromUnsignedInt(JNIEnv *env, const unsigned int *arr, int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfUnsignedShortFromUnsignedShort(JNIEnv *env, const unsigned short *ptr,int size);
extern VTKWRAPPINGDLANG_EXPORT jarray vtkDlangMakeJArrayOfUnsignedLongFromUnsignedLong(JNIEnv *env, const unsigned long *arr, int size);

// this is the void pointer parameter passed to the vtk callback routines on
// behalf of the Dlang interface for callbacks.
struct vtkDlangVoidFuncArg
{
  DlangVM *vm;
  jobject  uobj;
  jmethodID mid;
} ;

extern VTKWRAPPINGDLANG_EXPORT void vtkDlangVoidFunc(void *);
extern VTKWRAPPINGDLANG_EXPORT void vtkDlangVoidFuncArgDelete(void *);

class VTKWRAPPINGDLANG_EXPORT vtkDlangCommand : public vtkCommand
{
public:
  static vtkDlangCommand *New() { return new vtkDlangCommand; };

  void SetGlobalRef(jobject obj) { this->uobj = obj; };
  void SetMethodID(jmethodID id) { this->mid = id; };
  void AssignDlangVM(JNIEnv *env) { env->GetDlangVM(&(this->vm)); };

  void Execute(vtkObject *, unsigned long, void *);

  DlangVM *vm;
  jobject  uobj;
  jmethodID mid;
protected:
  vtkDlangCommand();
  ~vtkDlangCommand();
};

#endif
// VTK-HeaderTest-Exclude: vtkDlangUtil.h
