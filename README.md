# Zephyr App

Zephyr application created using West and using T2 topology.

## Homework task 2

### Create workspace

1. Update SO
``` bash
    sudo apt update
    sudo apt upgrade
```
2. Install West
``` bash
    pip install west # Reset console
``` 
4. Initialize Workspace
``` bash
    west init -m https://github.com/zephyrproject-rtos/zephyr.git --mr v3.6.0 . # For raw workspace that include all modules.
    west init -l # For custom west.yml file 
```
5. Update West
``` bash
    west update # Install all modules presents in Zephyr/west.yml or west.yml custom file.
```
7. IMPORTANT: Install nrf command line tools to include j-link segger
``` bash
    wget https://nsscprodmedia.blob.core.windows.net/prod/software-and-other-downloads/desktop-software/nrf-command-line-tools/sw/versions-10-x-x/10-24-2/nrf-command-line-tools_10.24.2_amd64.deb
    sudo dpkg -i nrf-command-line-tools_10.24.2_amd64.deb
    sudo apt install /opt/nrf-command-line-tools/share/JLink_Linux_V794e_x86_64.deb --fix-broken
```
9. obtain the corresponding completion script and have it sourced to enable shell completion. Using the completion scripts:
``` bash
    west completion bash > ~/west-completion.bash; echo "source ~/west-completion.bash" >> ~/.bashrc
```
10. Export a Zephyr CMake package. This allows CMake to automatically load boilerplate code required for building Zephyr applications.
``` bash
    west zephyr-export
```
12. Zephyr’s scripts/requirements.txt file declares additional Python dependencies. Install them with pip.
    pip install -r ~/zephyrproject/zephyr/scripts/requirements.txt
13. Download and verify the Zephyr SDK bundle:
``` bash
    cd ~
    wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.17.0/zephyr-sdk-0.17.0_linux-x86_64.tar.xz
    O ->  wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.9/zephyr-sdk-0.16.9_linux-x86_64.tar.xz
    wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.17.0/sha256.sum | shasum --check --ignore-missing
    O -> wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.9/sha256.sum | shasum --check --ignore-missing
```
15. Extract the Zephyr SDK bundle archive:
``` bash
    tar xvf zephyr-sdk-0.17.0_linux-x86_64.tar.xz
    O -> tar xvf zephyr-sdk-0.16.9_linux-x86_64.tar.xz
```
16. Run the Zephyr SDK bundle setup script:
``` bash
    cd zephyr-sdk-0.17.0
    ./setup.sh
    # You only need to run the setup script once after extracting the Zephyr SDK bundle.
    # You must rerun the setup script if you relocate the Zephyr SDK bundle directory after the initial setup.
```
17. Install udev rules, which allow you to flash most Zephyr boards as a regular user:
``` bash
    sudo cp ~/zephyr-sdk-0.17.0/sysroots/x86_64-pokysdk-linux/usr/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d
    sudo udevadm control --reload
```
18. Configure zephyr ariables:
``` bash

export ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk/zephyr-sdk-0.16.9
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr

```

#### Connecting USB devices - WSL:

``` bash

usbipd list
usbipd bind --busid 1-2
usbipd attach --wsl --busid 1-2
usbipd detach --busid 1-2

```

## Homework task 3

#### T2 Topologie:
``` bash

zephyrworkspace/
├─zephyrproject
│    ├─ CMakeLists.txt
│    ├─ west.yml
│    ├─ prj.conf
│    ├─ src/
│    │  ├─ main.c

```

## Homework task 4

### Switch between Zephyr versions

#### Select zephy v3.6.0

``` bash

export ZEPHYR_BASE=~/zephyrworkspace/zephyr

```

#### Select zephy v3.7.0

```bash 

export ZEPHYR_BASE=~zephyr-project/zephyr-v3.7.0

```

## Homework task 5

### Steps

1. First enable and modify the options you want using menu/guiconfig

```bash 

west build -b native_posix_64 --build-dir build -t menuconfig

```

2. Then a backup file will be automatically created in build/zephyr/.config.old

```bash 

zephyrworkspace/
├─zephyrproject
│    ├─ CMakeLists.txt
│    ├─ build
│    │  ├─ zephyr
│    │  │    ├─ .config
│    │  │    ├─ .config.old
│    ├─ west.yml
│    ├─ prj.conf
│    ├─ src/
│    │  ├─ main.c

```

3. Comapare both files using:

```bash 

diff --changed-group-format='%>' --unchanged-group-format='' .config .config.old > mydiff.conf # this command compare .config .config.old and then save the differences between files in mydiff.confi

```


