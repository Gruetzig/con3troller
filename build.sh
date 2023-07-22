rm -rf output

cd kblistener && make clean && make && cd ..
cd mouselistener && make clean && make && cd ..

mkdir output
mkdir output/keyboard
mkdir output/mouse

cp readme.md output/
cp ip.txt output/

cp kblistener/keyboard.py output/keyboard
cp mouselistener/mouse.py output/mouse

cp kblistener/kblistener.3dsx output/keyboard/
cp mouselistener/mouselistener.3dsx output/mouse/

cp kblistener/config.json output/keyboard/

cd output
zip -r con3troller.zip .
cd ..