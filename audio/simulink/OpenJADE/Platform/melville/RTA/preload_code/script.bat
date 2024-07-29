"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\easm21k.exe" -proc ADSP-21569 -si-revision any -char-size-8 -swc -o ./app_IVT.doj ../startup_ldf/app_IVT.s
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\easm21k.exe" -proc ADSP-21569 -si-revision any -char-size-8 -swc -o ./app_startup.doj ../startup_ldf/app_startup.s
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\cc21k.exe" -o ./app_heaptab.doj -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100 ../startup_ldf/app_heaptab.c
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\cc21k.exe" -o ./2156x_InitPreload_code.doj -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100 ./2156x_InitPreload_code.c
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\cc21k.exe" -o ./adi_clockrates_2156x_config.doj -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100 ./adi_clockrates_2156x_config.c
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\cc21k.exe" -o ./adi_dmc.doj -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100 ./adi_dmc.c
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\cc21k.exe" -o ./adi_dmc_2156x_config.doj -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100 ./adi_dmc_2156x_config.c
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\cc21k.exe" -o ./adi_pwr_2156x_config.doj -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100 ./adi_pwr_2156x_config.c
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\cc21k.exe" -o ./adi_smpu_2156x_config.doj -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100 ./adi_smpu_2156x_config.c
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\cc21k.exe" -o ./heapstack_table.doj -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100 ./heapstack_usage.c
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\cc21k.exe" -o ./ddr_adi_dmc.doj -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100 ./ddr_adi_dmc.c
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\cc21k.exe" -o ./ddr3_dmc_2156x_config0.doj -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100 ./ddr3_dmc_2156x_config0.c
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\cc21k.exe" -o ./ddr3_pwr_2156x_config.doj -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100 ./ddr3_pwr_2156x_config.c
"C:\Analog Devices\CrossCore Embedded Studio 2.9.1\cc21k.exe" -o ./preload.doj -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100 ./preload.c
"C:/Analog Devices/CrossCore Embedded Studio 2.9.1/cc21k.exe" -proc ADSP-21569 -si-revision any -T../startup_ldf/app.ldf -no-mem -flags-link -ip -flags-link -MDCORE0,-MDRELEASE -flags-link -e -o preload.dxe *.doj

rm *.doj *.xml
cd ..
mv ./preload_code/preload.dxe ./



