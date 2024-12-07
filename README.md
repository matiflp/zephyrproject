# Zephyr App

Zephyr application created using West and using T2 topology.

## Create workspace

1. Update SO
    sudo apt update
    sudo apt upgrade
2. Install West
    pip install west -> Reset console    
3. Inicializar Workspace
    west init -m https://github.com/zephyrproject-rtos/zephyr.git --mr v3.6.0 .
4. Update West
    west update -> Install all modules presents in Zephyr/west.yml
    west update cmsis hal_nordic -> Install only cmsis and hal_nordic modules.
    Another alternative is to modify west.yaml by deleting all the projects you don't want and run west update
5. IMPORTANT: Install nrf command line tools to include j-link segger
    wget https://nsscprodmedia.blob.core.windows.net/prod/software-and-other-downloads/desktop-software/nrf-command-line-tools/sw/versions-10-x-x/10-24-2/nrf-command-line-tools_10.24.2_amd64.deb
    sudo dpkg -i nrf-command-line-tools_10.24.2_amd64.deb
    sudo apt install /opt/nrf-command-line-tools/share/JLink_Linux_V794e_x86_64.deb --fix-broken
6. In order to enable shell completion, you will need to obtain the corresponding completion script and have it sourced. Using the completion scripts:
    west completion bash > ~/west-completion.bash; echo "source ~/west-completion.bash" >> ~/.bashrc
7. Export a Zephyr CMake package. This allows CMake to automatically load boilerplate code required for building Zephyr applications.
    west zephyr-export
8. Zephyr’s scripts/requirements.txt file declares additional Python dependencies. Install them with pip.
    pip install -r ~/zephyrproject/zephyr/scripts/requirements.txt
9. Download and verify the Zephyr SDK bundle:
    cd ~
    wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.17.0/zephyr-sdk-0.17.0_linux-x86_64.tar.xz
    O ->  wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.9/zephyr-sdk-0.16.9_linux-x86_64.tar.xz
    wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.17.0/sha256.sum | shasum --check --ignore-missing
    O -> wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.9/sha256.sum | shasum --check --ignore-missing
10. Extract the Zephyr SDK bundle archive:
    tar xvf zephyr-sdk-0.17.0_linux-x86_64.tar.xz
    O -> tar xvf zephyr-sdk-0.16.9_linux-x86_64.tar.xz
11. Run the Zephyr SDK bundle setup script:
    cd zephyr-sdk-0.17.0
    ./setup.sh
    You only need to run the setup script once after extracting the Zephyr SDK bundle.
    You must rerun the setup script if you relocate the Zephyr SDK bundle directory after the initial setup.
12. Install udev rules, which allow you to flash most Zephyr boards as a regular user:
    sudo cp ~/zephyr-sdk-0.17.0/sysroots/x86_64-pokysdk-linux/usr/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d
    sudo udevadm control --reload

## Set Zephyr variables:

``` bash

export ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk/zephyr-sdk-0.16.9
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr

```

## T2 Topologie:

``` bash

zephyr-project/
├─App
│    ├─ CMakeLists.txt
│    ├─ west.yml
│    ├─ src/
│    │  ├─ main.c
├─zephyr_3.6.0
├─zphyr_3.7.0

```

### Switch between Zephyr

#### Select zephy v3.6.0

``` bash

cd zephyr-project/zephyr/zephyr_3.6.0
source zephyr-env.sh

```

#### Select zephy v3.7.0

```bash 

cd zephyr-project/zephyr/zephyr_3.7.0
source zephyr-env.sh

```
## Connecting USB devices - WSL:

``` bash

usbipd list
usbipd bind --busid 1-2
usbipd attach --wsl --busid 1-2
usbipd detach --busid 1-2

```


