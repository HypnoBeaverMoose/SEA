/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//
//static const char gVertexShader[] = 
//    "attribute vec4 vPosition;\n"
//    "void main() {\n"
//    "  gl_Position = vPosition;\n"
//    "}\n";
//
//static const char gFragmentShader[] = 
//    "precision mediump float;\n"
//    "void main() {\n"
//    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
//    "}\n";




//
//GLuint gProgram;
//GLuint gvPositionHandle;
//
//bool setupGraphics(int w, int h) {
//    printGLString("Version", GL_VERSION);
//    printGLString("Vendor", GL_VENDOR);
//    printGLString("Renderer", GL_RENDERER);
//    printGLString("Extensions", GL_EXTENSIONS);
//
//    LOGI("setupGraphics(%d, %d)", w, h);
//    gProgram = createProgram(gVertexShader, gFragmentShader);
//    if (!gProgram) {
//        LOGE("Could not create program.");
//        return false;
//    }
//    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
//    checkGlError("glGetAttribLocation");
//    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
//            gvPositionHandle);
//
//    glViewport(0, 0, w, h);
//    checkGlError("glViewport");
//    return true;
//}

//const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
//        0.5f, -0.5f };

//void renderFrame() {
//    static float grey;
//    grey += 0.01f;
//    if (grey > 1.0f) {
//        grey = 0.0f;
//    }
//    glClearColor(grey, grey, grey, 1.0f);
//    checkGlError("glClearColor");
//    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//    checkGlError("glClear");
//
//    glUseProgram(gProgram);
//    checkGlError("glUseProgram");
//
//    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
//    checkGlError("glVertexAttribPointer");
//    glEnableVertexAttribArray(gvPositionHandle);
//    checkGlError("glEnableVertexAttribArray");
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    checkGlError("glDrawArrays");
//}

//extern "C" 
//{
//    JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_init(JNIEnv * env, jobject obj,  jint width, jint height);
//    JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_step(JNIEnv * env, jobject obj);
//};
//
//JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_init(JNIEnv * env, jobject obj,  jint width, jint height)
//{
//    setupGraphics(width, height);
//}
//
//JNIEXPORT void JNICALL Java_com_android_plantgen_PlantGenLib_step(JNIEnv * env, jobject obj)
//{
//    renderFrame();
//}
