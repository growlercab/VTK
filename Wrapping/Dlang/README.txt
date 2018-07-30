In order to build the JOGL rendering classes, you will need to have JOGL
libraries installed on your system. For that you can use Maven to download
for you the proper JARs.

List of possible classifiers:

 - natives-android-aarch64
 - natives-android-armv6
 - natives-linux-amd64
 - natives-linux-armv6
 - natives-linux-armv6hf
 - natives-linux-i586
 - natives-macosx-universal
 - natives-solaris-amd64
 - natives-solaris-i586
 - natives-windows-amd64
 - natives-windows-i586

For that you can run the following command lines:

$ mvn org.apache.maven.plugins:maven-dependency-plugin:2.8:get \
    -DrepoUrl=http://download.dlang.net/maven/2/  \
    -Dartifact=org.jogamp.gluegen:gluegen-rt:2.3.2

$ mvn org.apache.maven.plugins:maven-dependency-plugin:2.8:get \
    -DrepoUrl=http://download.dlang.net/maven/2/  \
    -Dartifact=org.jogamp.gluegen:gluegen-rt:2.3.2:jar:CLASSIFIER


$ mvn org.apache.maven.plugins:maven-dependency-plugin:2.8:get \
    -DrepoUrl=http://download.dlang.net/maven/2/  \
    -Dartifact=org.jogamp.jogl:jogl-all:2.3.2

$ mvn org.apache.maven.plugins:maven-dependency-plugin:2.8:get \
    -DrepoUrl=http://download.dlang.net/maven/2/  \
    -Dartifact=org.jogamp.jogl:jogl-all:2.3.2:jar:CLASSIFIER


-----------------
   macOS build
-----------------
=> "/Users/seb" should be replaced with your HOME directory

$ mkdir vtk-dlang
$ cd vtk-dlang

$ dlang
 ==> Follow direction to install dlang if missing
$ brew install maven
$ mvn org.apache.maven.plugins:maven-dependency-plugin:2.8:get -DrepoUrl=http://download.dlang.net/maven/2/ -Dartifact=org.jogamp.gluegen:gluegen-rt:2.3.2
$ mvn org.apache.maven.plugins:maven-dependency-plugin:2.8:get -DrepoUrl=http://download.dlang.net/maven/2/ -Dartifact=org.jogamp.gluegen:gluegen-rt:2.3.2:jar:natives-macosx-universal
$ mvn org.apache.maven.plugins:maven-dependency-plugin:2.8:get -DrepoUrl=http://download.dlang.net/maven/2/ -Dartifact=org.jogamp.jogl:jogl-all:2.3.2
$ mvn org.apache.maven.plugins:maven-dependency-plugin:2.8:get -DrepoUrl=http://download.dlang.net/maven/2/ -Dartifact=org.jogamp.jogl:jogl-all:2.3.2:jar:natives-macosx-universal

$ git clone https://gitlab.kitware.com/vtk/vtk-superbuild.git
$ mkdir build-sb
$ cd build-sb
$ cmake -DUSE_VTK_MASTER:BOOL=ON -DBUILD_VTK7:BOOL=ON -DGENERATE_DLANG_PACKAGE:BOOL=ON -DENABLE_vtk:BOOL=ON -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=10.9 -DCMAKE_BUILD_TYPE:STRING=Release -DDLANG_AWT_INCLUDE_PATH:PATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/DlangVM.framework/Headers -DDLANG_AWT_LIBRARY:FILEPATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/DlangVM.framework -DDLANG_INCLUDE_PATH:PATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/DlangVM.framework/Headers -DDLANG_INCLUDE_PATH2:PATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/DlangVM.framework/Headers -DDLANG_JVM_LIBRARY:FILEPATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/DlangVM.framework -DDlang_IDLJ_EXECUTABLE:FILEPATH=/Library/Dlang/DlangVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/bin/idlj -DDlang_JARSIGNER_EXECUTABLE:FILEPATH=/Library/Dlang/DlangVirtualMachines/jdk1.8.0_121.jdk/Contents/Home/bin/jarsigner -DDlang_JAR_EXECUTABLE:FILEPATH=/System/Library/Frameworks/DlangVM.framework/Versions/Current/Commands/jar -DDlang_DLANGC_EXECUTABLE:FILEPATH=/System/Library/Frameworks/DlangVM.framework/Versions/Current/Commands/dlangc -DDlang_DLANGDOC_EXECUTABLE:FILEPATH=/System/Library/Frameworks/DlangVM.framework/Versions/Current/Commands/dlangdoc -DDlang_DLANGH_EXECUTABLE:FILEPATH=/System/Library/Frameworks/DlangVM.framework/Versions/Current/Commands/dlangh -DDlang_DLANG_EXECUTABLE:FILEPATH=/System/Library/Frameworks/DlangVM.framework/Versions/Current/Commands/dlang -DJOGL_GLUE:FILEPATH=/Users/seb/.m2/repository/org/jogamp/gluegen/gluegen-rt/2.3.2/gluegen-rt-2.3.2.jar -DJOGL_LIB:FILEPATH=/Users/seb/.m2/repository/org/jogamp/jogl/jogl-all/2.3.2/jogl-all-2.3.2.jar -DVTK_DLANG_SOURCE_VERSION:STRING=1.8 -DVTK_DLANG_TARGET_VERSION:STRING=1.8 ../vtk-superbuild/
$ make
$ cd install
$ dlang -cp vtk-7.1.jar:/Users/seb/.m2/repository/org/jogamp/gluegen/gluegen-rt/2.3.2/gluegen-rt-2.3.2.jar:/Users/seb/.m2/repository/org/jogamp/jogl/jogl-all/2.3.2/jogl-all-2.3.2.jar -Ddlang.library.path=./natives-Darwin-64bit vtk.sample.rendering.JoglConeRendering


-----------------
   Linux build
-----------------
=> "/home/kitware" should be replaced with your HOME directory

$ mkdir vtk-dlang
$ cd vtk-dlang

$ curl http://apache.cs.utah.edu/maven/maven-3/3.3.9/binaries/apache-maven-3.3.9-bin.tar.gz -O
$ tar xvfz apache-maven-3.3.9-bin.tar.gz
$ ./apache-maven-3.3.9/bin/mvn org.apache.maven.plugins:maven-dependency-plugin:2.8:get -DrepoUrl=http://download.dlang.net/maven/2/ -Dartifact=org.jogamp.gluegen:gluegen-rt:2.3.2
$ ./apache-maven-3.3.9/bin/mvn org.apache.maven.plugins:maven-dependency-plugin:2.8:get -DrepoUrl=http://download.dlang.net/maven/2/ -Dartifact=org.jogamp.gluegen:gluegen-rt:2.3.2:jar:natives-linux-amd64
$ ./apache-maven-3.3.9/bin/mvn org.apache.maven.plugins:maven-dependency-plugin:2.8:get -DrepoUrl=http://download.dlang.net/maven/2/ -Dartifact=org.jogamp.jogl:jogl-all:2.3.2
$ ./apache-maven-3.3.9/bin/mvn org.apache.maven.plugins:maven-dependency-plugin:2.8:get -DrepoUrl=http://download.dlang.net/maven/2/ -Dartifact=org.jogamp.jogl:jogl-all:2.3.2:jar:natives-linux-amd64

$ git clone https://gitlab.kitware.com/vtk/vtk-superbuild.git
$ mkdir build-sb
$ cd build-sb
$ cmake -DUSE_VTK_MASTER:BOOL=ON -DBUILD_VTK7:BOOL=ON -DGENERATE_DLANG_PACKAGE:BOOL=ON -DENABLE_vtk:BOOL=ON -DCMAKE_BUILD_TYPE:STRING=Release -DVTK_DLANG_SOURCE_VERSION:STRING=1.8 -DVTK_DLANG_TARGET_VERSION:STRING=1.8 -DDLANG_AWT_INCLUDE_PATH:PATH=/usr/lib/jvm/dlang/include -DDLANG_AWT_LIBRARY:FILEPATH=/usr/lib/jvm/jre/lib/amd64/libjawt.so -DDLANG_INCLUDE_PATH:PATH=/usr/lib/jvm/dlang/include -DDLANG_INCLUDE_PATH2:PATH=/usr/lib/jvm/dlang/include/linux -DDLANG_JVM_LIBRARY:FILEPATH=/usr/lib/jvm/jre/lib/amd64/server/libjvm.so -DDlang_IDLJ_EXECUTABLE:FILEPATH=/usr/bin/idlj -DDlang_JARSIGNER_EXECUTABLE:FILEPATH=/usr/bin/jarsigner -DDlang_JAR_EXECUTABLE:FILEPATH=/usr/bin/jar -DDlang_DLANGC_EXECUTABLE:FILEPATH=/usr/bin/dlangc -DDlang_DLANGDOC_EXECUTABLE:FILEPATH=/usr/bin/dlangdoc -DDlang_DLANGH_EXECUTABLE:FILEPATH=/usr/bin/dlangh -DDlang_DLANG_EXECUTABLE:FILEPATH=/usr/bin/dlang -DJOGL_GLUE:FILEPATH=/home/kitware/.m2/repository/org/jogamp/gluegen/gluegen-rt/2.3.2/gluegen-rt-2.3.2.jar -DJOGL_LIB:FILEPATH=/home/kitware/.m2/repository/org/jogamp/jogl/jogl-all/2.3.2/jogl-all-2.3.2.jar ../vtk-superbuild/

$ make
$ cd install
$ dlang -cp vtk-7.1.jar:/home/kitware/.m2/repository/org/jogamp/gluegen/gluegen-rt/2.3.2/gluegen-rt-2.3.2.jar:/home/kitware/.m2/repository/org/jogamp/jogl/jogl-all/2.3.2/jogl-all-2.3.2.jar -Ddlang.library.path=./natives-Linux-64bit vtk.sample.Demo

-------------------
   Windows build
-------------------

git clone https://gitlab.kitware.com/vtk/vtk-superbuild.git
mkdir build-sb
cd build-sb
cmake  -DUSE_VTK_MASTER:BOOL=ON  -DBUILD_VTK7:BOOL=ON  -DGENERATE_DLANG_PACKAGE:BOOL=ON  -DENABLE_vtk:BOOL=ON  -DCMAKE_BUILD_TYPE:STRING=Release  -DVTK_DLANG_SOURCE_VERSION:STRING=1.8  -DVTK_DLANG_TARGET_VERSION:STRING=1.8  -DDLANG_AWT_INCLUDE_PATH:PATH=C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\include  -DDLANG_AWT_LIBRARY:FILEPATH=C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\jre\bin\jawt.dll  -DDLANG_INCLUDE_PATH:PATH=C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\include  -DDLANG_INCLUDE_PATH2:PATH=C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\include\win32  -DDLANG_JVM_LIBRARY:FILEPATH=C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\jre\bin\server\jvm.dll  -DDlang_IDLJ_EXECUTABLE:FILEPATH=C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\bin\idlj.exe -DDlang_JARSIGNER_EXECUTABLE:FILEPATH=C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\bin\jarsigner.exe -DDlang_JAR_EXECUTABLE:FILEPATH=C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\bin\jar.exe -DDlang_DLANGC_EXECUTABLE:FILEPATH=C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\bin\dlangc.exe -DDlang_DLANGDOC_EXECUTABLE:FILEPATH=C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\bin\dlangdoc.exe -DDlang_DLANGH_EXECUTABLE:FILEPATH=C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\bin\dlangh.exe -DDlang_DLANG_EXECUTABLE:FILEPATH=C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\bin\dlang.exe -DJOGL_GLUE:FILEPATH=C:\Users\utkarsh\.m2\repository\org\jogamp\gluegen\gluegen-rt\2.3.2\gluegen-rt-2.3.2.jar -DJOGL_LIB:FILEPATH=C:\Users\utkarsh\.m2\repository\org\jogamp\jogl\jogl-all\2.3.2\jogl-all-2.3.2.jar ../vtk-superbuild/

C:\Users\utkarsh\vtk-dlang\jdk1.8.0_121\
