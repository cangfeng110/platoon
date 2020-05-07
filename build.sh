mkdir pgbuild
cd pgbuild/
rm -r *
cmake -Dpegasus-build=ON -DCMAKE_TOOLCHAIN_FILE=/home/igs/zsworkspace/atd_environment/nvidia/cmake/Toolchain-V5L.cmake -DVIBRANTE_PDK:STRING=/home/igs/zsworkspace/atd_environment/nvidia/nvidia_sdk/DRIVE/Linux/5.0.13.2/SW/DriveSDK/drive-t186ref-linux ..
#cmake -DCMAKE_TOOLCHAIN_FILE=./cmake/Toolchain-V5L.cmake -DVIBRANTE_PDK:STRING=/usr/local/Beta30/VibranteSDK/vibrante-t186ref-linux ..
#cmake -DCMAKE_TOOLCHAIN_FILE=./cmake/Toolchain-V5L.cmake -DVIBRANTE_PDK:STRING=/home/saic/nvidia/nvidia_sdk/DRIVE/Linux/5.0.13.2/SW/DriveSDK/drive-t186ref-linux ..
make -j8 -l8
#make install
