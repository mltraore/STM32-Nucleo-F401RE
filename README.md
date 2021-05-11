# STM32-Nucleo-F401RE
<!-- 

## Install System Workbench for [STM32](https://www.youtube.com/watch?v=kOvkFgpfI8Q)
## Install [STM32CubeMX](https://www.youtube.com/watch?v=FGneBNKR93k)
## Solution for [gksu](https://askubuntu.com/questions/1030054/how-to-install-an-application-that-requires-gksu-package-on-ubuntu-18-04) on ubuntu 18.04 LTS
```shell
wget http://mirrors.kernel.org/ubuntu/pool/main/libg/libgtop2/libgtop-2.0-10_2.32.0-1_amd64.deb
sudo apt install ./libgtop-2.0-10_2.32.0-1_amd64.deb

wget https://mirrors.edge.kernel.org/ubuntu/pool/universe/libg/libgksu/libgksu2-0_2.0.13~pre1-6ubuntu8_amd64.deb
sudo apt install ./libgksu2-0_2.0.13~pre1-6ubuntu8_amd64.deb

wget http://mirrors.kernel.org/ubuntu/pool/universe/g/gksu/gksu_2.0.2-9ubuntu1_amd64.deb
sudo apt install ./gksu_2.0.2-9ubuntu1_amd64.deb
```  
**Test** `gksu`

```shell
gksu-properties # check that it has "Authentication mode" to "sudo"
gksu date
gksudo date
```  
## Make desktop app for STM32CubeMX
Save to ```~/.local/share/applications/stm32cubemx.desktop```  
```shell
[Desktop Entry]
Name=STM32CubeMX
GenericName=STM32CubeMX
Comment=Graphical tool for configuration of STM32 Microcontrollers
Exec=/home/mohamedlassine/STM32CubeMX/STM32CubeMX
Terminal=false
X-MultipleArgs=false
Type=Application
Icon=/home/mohamedlassine/.local/share/applications/icons/stm32cubemx.png
StartupWMClass=STM32CubeMX
StartupNotify=true
```  
for [more](https://help.ubuntu.com/community/UnityLaunchersAndDesktopFiles#:~:text=Unity%20Launchers%20are%20actually%20files,access%20from%20the%20Unity%20Launcher.)


-->
