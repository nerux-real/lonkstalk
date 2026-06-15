cp build/scores.db scores.db
cp build/settings.ini settings.ini
sudo rm -rf build
mkdir build
cp -r beatmaps build/beatmaps
cp -r skins build/skins
cp scores.db build/scores.db
cp settings.ini build/settings.ini
cd build
cmake ..
make 
./Lonkstalk
