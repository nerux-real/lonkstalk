cp build-linux/scores.db gamestuff/scores.db
cp build-linux/settings.ini gamestuff/settings.ini
sudo rm -rf build-linux
mkdir build-linux
cp -r gamestuff/beatmaps build-linux/beatmaps
cp -r gamestuff/skins build-linux/skins
cp gamestuff/scores.db build-linux/scores.db
cp gamestuff/settings.ini build-linux/settings.ini
cd build-linux
cmake ..
make 
if [[ " $* " == *" --run "* ]]; then
	./Lonkstalk
fi
