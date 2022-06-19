#!/usr/bin/env bash

VERSION=0.0.3

# move to build directory
pushd /tmp || exit

# Get the artifact
if [ "${SKIP_DOWNLOAD}" == "" ]
then
  echo "Downloading Benchmarkr Version ${VERSION}..."
  wget https://github.com/tnweiss/Benchmarkr-cpp/releases/download/${VERSION}/ubuntu.tar.gz
else
    echo "Skipping Benchmarkr download"
fi

# Extract the resources
echo "Extracting installation resources..."
tar -xzvf ubuntu.tar.gz

# install under /opt
echo "Installing resources"
mv install /opt/benchmarkr

echo "Installing headers"
mv /opt/benchmarkr/include/* /usr/include/

# configure the lib directory
echo "Configuring libraries"
echo "/opt/benchmarkr/lib" > /etc/ld.so.conf.d/benchmarkr.conf

# add benchmarkr to local bin
echo "Creating executable symlink..."
ln -s /opt/benchmarkr/bin/benchmarkr /usr/local/bin/benchmarkr
ldconfig

# cleanup
echo "Cleaning..."
rm -rf ubuntu.tar.gz

echo "Done"

# move back to old dir
popd || exit
