rm -rf output

cd buttonstokb && make clean && make && cd ..
cd touchtomouse && make clean && make && cd ..

mkdir output
mkdir output/keyboard
mkdir output/mouse

cp readme.md output/
cp ip.txt output/

cp buttonstokb/keyboard.py output/keyboard
cp touchtomouse/mouse.py output/mouse

cp buttonstokb/buttonstokb.3dsx output/keyboard/
cp touchtomouse/touchtomouse.3dsx output/mouse/

cp buttonstokb/config.json output/keyboard/

cd output
zip -r con3troller.zip .
cd ..