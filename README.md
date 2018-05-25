# BaslerTakePicture
Taking picture using Basler camera in python

This project create a lib to be used in scripts python to take pictures using basler camera.

# Requirements
- Ubuntu
- cmake >= 2.8
- Qt >=5.9
- python
- OpenCv (C++ and python)
- Pylon
- Boost 1.6

# Setup
- include on .bashrc

export PKG_CONFIG_PATH=/opt/Qt/5.9.1/gcc_64/lib/pkgconfig/
export LD_LIBRARY_PATH=/usr/local/lib:/opt/Qt/5.9.1/gcc_64/lib/
export PATH=/opt/Qt/5.9.1/gcc_64/bin:$PATH
export PYLON_ROOT=/opt/pylon5
export BOOST_ROOT=/usr/local/boost_1_61_0:$PATH

# Compile
- mkdir build
- cd build
- cmake ../src
- make -j4

# Execute
- chmod a+x test.py
- PYTHONPATH=build ./test.py
