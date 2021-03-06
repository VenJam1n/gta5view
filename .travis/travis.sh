#!/bin/bash

# Install lua
if [ "${TRAVIS_OS_NAME}" == "linux" ]; then
	sudo apt-get update -qq && \
	sudo apt-get install -qq lua5.2
elif [ "${TRAVIS_OS_NAME}" == "osx" ]; then
	brew install lua
fi

if [ `git name-rev --tags --name-only $(git rev-parse HEAD)` == "undefined" ]; then export APPLICATION_VERSION=`lua -e 'for line in io.lines("config.h") do local m = string.match(line, "#define GTA5SYNC_APPVER \"(.+)\"$"); if m then print(m); os.exit(0) end end'`; else export APPLICATION_VERSION=`git name-rev --tags --name-only $(git rev-parse HEAD)`; fi
export PACKAGE_VERSION=$(grep -oE '^[^\-]*' <<< $APPLICATION_VERSION)
export PACKAGE_BUILD=$(grep -oP '\-\K.+' <<< $APPLICATION_VERSION)
export EXECUTABLE_VERSION=${PACKAGE_VERSION}${PACKAGE_BUILD}
if [[ ! ${PACKAGE_BUILD} ]]; then export PACKAGE_BUILD=1; fi
export PROJECT_DIR=$(pwd)

.travis/${BUILD_SCRIPT}
