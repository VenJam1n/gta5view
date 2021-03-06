#!/bin/bash

apt-get update -qq && \
apt-get install -qq nsis && \

# Creating folders
cd ${PROJECT_DIR} && \
echo "gta5view build version is ${APPLICATION_VERSION}" && \
mkdir -p build && \
mkdir -p assets && \

# Starting build
cd build && \
qmake ${QMAKE_FLAGS} DEFINES+=GTA5SYNC_BUILDTYPE_REL "DEFINES+=GTA5SYNC_BUILDCODE=\\\\\\\"GitHub\\\\\\\"" "DEFINES+=GTA5SYNC_APPVER=\\\\\\\"${APPLICATION_VERSION}\\\\\\\"" DEFINES+=GTA5SYNC_TELEMETRY "DEFINES+=GTA5SYNC_TELEMETRY_WEBURL=\\\\\\\"https://dev.syping.de/gta5view-userstats/\\\\\\\"" DEFINES+=GTA5SYNC_QCONF DEFINES+=GTA5SYNC_INLANG='\\\"RUNDIR:SEPARATOR:lang\\\"' DEFINES+=GTA5SYNC_LANG='\\\"RUNDIR:SEPARATOR:lang\\\"' DEFINES+=GTA5SYNC_PLUG='\\\"RUNDIR:SEPARATOR:plugins\\\"' ../gta5view.pro && \
make -j 4 && \
cd ${PROJECT_DIR}/assets && \
makensis -NOCD ${PROJECT_DIR}/.travis/gta5view.nsi
