#!/bin/sh

pushd FreeType2-2.4.12

p4 edit $THIRD_PARTY_CHANGELIST lib/Mac/...
p4 edit $THIRD_PARTY_CHANGELIST lib/IOS/...

# compile Mac

pushd Builds/mac
xcodebuild -sdk macosx clean
xcodebuild -sdk macosx
cp build/Release/libfreetype.a ../../lib/Mac/libfreetype2412.a

# compile IOS (NOTE the lib rename below when we update versions)
pushd Builds/IOS
xcodebuild -sdk iphoneos clean
xcodebuild -sdk iphoneos
cp build/Release-iphoneos/libfreetype.a ../../lib/IOS/Device/libfreetype2412.a
xcodebuild -sdk iphonesimulator clean
xcodebuild -sdk iphonesimulator
cp build/Release-iphonesimulator/libfreetype.a ../../lib/IOS/Simulator/libfreetype2412.a
popd

popd
