#!/usr/bin/env bash

VERSION=0.0.3

# move to build directory
pushd /tmp || exit

# Get the artifact
if [ "${SKIP_DOWNLOAD}" == "" ]
then
  echo "Downloading Benchmarkr Version ${VERSION}..."
  wget https://github.com/tnweiss/Benchmarkr-cpp/releases/download/${VERSION}/macos.tar.gz
else
    echo "Skipping Benchmarkr download"
fi

# Extract the resources
echo "Extracting installation resources..."
tar -xzvf macos.tar.gz

# install under /opt
echo "Installing resources"
mv install /opt/benchmarkr

echo "Installing headers"
mv /opt/benchmarkr/include/* /usr/local/include/

## configure the lib directory
install_name_tool -add_rpath "/opt/benchmarkr/lib" /opt/benchmarkr/bin/benchmarkr
install_name_tool -change /usr/local/opt/ossp-uuid/lib/libuuid.16.dylib /opt/benchmarkr/lib/libuuid.16.dylib /opt/benchmarkr/bin/benchmarkr

# add benchmarkr to local bin
echo "Creating executable symlink..."
ln -s /opt/benchmarkr/bin/benchmarkr /usr/local/bin/benchmarkr

# cleanup
echo "Cleaning..."
rm -rf macos.tar.gz

echo "Done"

# move back to old dir
popd || exit
