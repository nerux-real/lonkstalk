cp build-linux/scores.db scores.db
cp build-linux/settings.ini settings.ini
sudo rm -rf build-linux
mkdir build-linux
cp -r beatmaps build-linux/beatmaps
cp -r skins build-linux/skins
cp scores.db build-linux/scores.db
cp settings.ini build-linux/settings.ini
cd build-linux
cmake ..
make 
./Lonkstalk
