//
// Created by Administrator on 2022/12/28.
//

#include <jni.h>
#include <android/log.h>
#include <stdio.h>

#define LOG_TAG "C++"

// 定义LOGD类型
#define LOG(...)  \
    do { \
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__); \
    }while(0); \

#define ClassName "com/lindong/jnidemo1/JniInterface"

/*
jstring转char*
*/
char* jstringToChar(JNIEnv *env, jstring jstr) {
    jboolean iscopy;
    char *cstr = (char *)env->GetStringUTFChars(jstr, &iscopy);
    return cstr;
}

/*
char*转jstring
*/
jstring charTojstring(JNIEnv *env, char *cstr) {
    return env->NewStringUTF(cstr);
}

/*
空方法可以不用传任何字段
也可以传这两个参数：void onLoadTest(JNIEnv*env,jobject obj);两个参数含义和用javah生成的一致。
*/
void onLoadTest() {
    LOG("onLoadTest！！！");
}

/*
如果有参数，那么需要加上前面两个参数，不然会导致参数不对应。参数含义和javah生成的头文件中参数含义一致。
*/
jstring onloadTest1(JNIEnv *env, jobject instance, jobject obj) {
    jclass pCls = env->GetObjectClass(obj); // 获取目标对象的类名
    jfieldID nameFid = env->GetFieldID(pCls, "name", "Ljava/lang/String;"); // 获取fieldID
    jstring name = (jstring) env->GetObjectField(obj, nameFid);
    char *cname = jstringToChar(env, name);
    char *tmp = new char[100];
    sprintf(tmp, "我来自Native，我叫：%s", cname);
    jstring result = charTojstring(env, tmp);
    return result;
}

/*
在{}里面进行方法映射编写，
第一个是java层对应的方法名称，
第二个是该方法的返回值类型和参数类型，
第三个是c文件native中对应的函数指针
JNINativeMethod的定义如下
typedef struct {
    const char* name;     // java层对应的方法名称
    const char* signature;// 该方法的返回值类型和参数类型
    void*       fnPtr;    // native中对应的函数指针
} JNINativeMethod;
*/
static const JNINativeMethod g_Methods[] = {
    {
        "jniOnLoadTest", // 上层调用的native修饰的方法名
        "()V", // 参数和返回值，括号内是参数，括号右边是返回值
        (void*) onLoadTest // 对应的底层的函数指针，括号内表示方法返回值
    },
    {
        "jniOnload1",
        "(Lcom/lindong/jnidemo1/Person;)Ljava/lang/String;",
        (jstring*)onloadTest1
    }
};

/*
动态注册函数映射，上层应用层加载so文件到的时候会首先调用JNI_OnLoad方法，一般有两种情况，
第一种是官方推荐的方法，通过javah生成头文件，然后自己实现对应的cpp文件，这种情况不需要重写JNI_OnLoad方法
第二种是很多开源项目中使用的，扩展性强，但是IDE不会进行代码提示和跳转提示，需要自己重写JNI_OnLoad方法。
*/
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOG("JNI_OnLoad");
    JNIEnv *pEnv = NULL;
    //获取环境
    jint ret = vm->GetEnv((void**) &pEnv, JNI_VERSION_1_6);
    if (ret != JNI_OK) {
        LOG("jni_replace JVM ERROR:GetEnv");
        return -1;
    }

    //获取类名
    jclass cls = pEnv->FindClass(ClassName); // native修饰的方法名所在的类
    if (cls == NULL) {
        LOG("FindClass Error");
        return -1;
    }

    //动态注册本地方法 第一个是方法对应的类，第二个是方法映射，第三个是映射方法的个数
    ret = pEnv->RegisterNatives(cls, g_Methods, sizeof(g_Methods) / sizeof(g_Methods[0]));
    if (ret != JNI_OK) {
        LOG("Register Error");
        return -1;
    }

    //返回java版本，写JNI_VERSION_1_2  JNI_VERSION_1_4  JNI_VERSION_1_6没什么区别，
    //但是别好别写JNI_VERSION_1_1,注意和上方获取环境的方法GetEnv里第三个参数的值保持一致
    return JNI_VERSION_1_6;
}

/*
动态反注册方法，和JNI_OnLoad对应，用来释放资源
*/
JNIEXPORT void JNI_OnUnload(JavaVM *jvm, void *reserved)
{
    LOG("JNI_OnUnload");
}
