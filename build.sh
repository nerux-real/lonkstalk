cd ..
sudo rm -rf build
mkdir build
cp -r beatmaps build/beatmaps
cd build
cmake ..
make 
./Lonkstalk
